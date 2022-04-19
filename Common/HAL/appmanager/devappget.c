/** @file devappget.c is an inxwar HAL file used by app management components and devman
 *
 * @author: Dr Pierre Drezet
 * @version: $Revision: 2780 $
 * @date: $Date: 20011-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx Ltd, 2012. All rights reserved.
 */

/* @todo we need some global mutexing, at least on the default and temp file directories to avoid collisions of app downloads */

#include "ehs_types.h"

#include "hal-api.h" // for logger
#include "hal_devman.h"
/* Use a generic archive library */
#include <archive.h>
#include <archive_entry.h>

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

/* Zero copy URL reader and archive writer */
/* Pole the incoming buffer for data and decode tar info as we write files directly to the file system */
//static long int
static ssize_t write_archive_to_disk(struct archive *a, void *client_data, const void **buff) //(struct archive *ar, struct archive *aw)
{
  AppGet_URL_data_buffer_Type *mydata = client_data;
  mydata->data_ready=EHS_FALSE;/* Don't continue until we have some data & signal URL getter to send some */
  while (mydata->data_ready == EHS_FALSE) { //wait for data.
  		EhsSleep(EHS_TIME_us(10000)); /* 10 ms wait for data to be read */
  		//@todo Add a timeout here.
  }
  /*got a data packet so continue */
  *buff=mydata->buffer; //assign the buffer to the data
//@todo Should  we copy the data here rather than reference??
  //printf("archiver: Got a chunk of data (%d), flagging file writer\n",mydata->size_read);
   //mydata->data_ready=EHS_FALSE;// Don't set this untill we are called again for more data. flags ready for more data
  return (mydata->size_read);   // returns 0 if no more left so we always get another call back
}

/* CURL callback Retrieve data and put in a data buffer */
static size_t AppGet_URL_data_CB(void *ptr, size_t size, size_t nmemb, void *userdata) {

	ehs_uint32 amount = size * nmemb;
	EHSH_LOG_INFO("AppGet_URL_data_CB amount=%d",amount);
	AppGet_URL_data_buffer_Type * datastruct=(AppGet_URL_data_buffer_Type *)userdata;
	datastruct->buffer=ptr; /* get the buffer ready */
	datastruct->size_read=amount; /* set zise to non-zero to trigger read */
	datastruct->data_ready = EHS_TRUE; //trigger the writer to write and wait for it signal finished.
	while (datastruct->data_ready != EHS_FALSE) { //wait for the writer thread to write all the data.
		/* untill the buffer is ready to be read waiting for the archive reader to poll for */
			EhsSleep(EHS_TIME_us(10000)); /* 10 ms wait for data to be read */
			/* @todo Add a timeout here. */
	}
	/* must be done - so we can exit now */
	 //printf("LIBCURL: Got a chunk of data (%d), flagging file writer\n",amount);
	return amount;
}

/* Public HAL functions */

/* Start loop that reads from URL
 *
 * This reads and writes only to the app getter
 *
 * Must be run as a thread
 *
 * */
EhsThreadFuncReturnType HAL_appGetWaitForURLDataAndWrite(void *XObjData)
{
  struct archive *a; // for reading.
  struct archive *aw; //for writing to
  struct archive *ext;
  struct archive_entry *entry;
  appgetObj *ObjData= (appgetObj *)XObjData;
  ehs_uint16 i;
  ehs_bool appget_success=EHS_FALSE;
  ehs_bool headers_done=EHS_FALSE;
//@todo The following are arrays to leave gaps between them. The def if size_t seems to be different in EHS than libarchive and the pointers point to different size objects..."
  //size_t size;
  //off_t offset;
  //
size_t size[2];
#ifdef __LA_INT64_T
  __LA_INT64_T offset[2]; //This is the actual data type used in the header file
#else
 off_t offset[2];
#endif
  ehs_char szfilename[EHS_TD_FILES_MAX_PATH];
  const void *databuf; // this is get pointed to the data buffer by the read function
  size_t entrysize;
  //appgetObj *ObjData = (appgetObj*)EHS_FB_INIT_CONTEXT; // this needs
  AppGet_URL_data_buffer_Type *mydata = &(ObjData->AppGet_write_data_buffer_struct);
  ehs_sint32 flags,r;
  flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;
  /* Create a file to write to */
   mydata->data_ready= EHS_FALSE;
   aw = archive_write_disk_new();
   if (!aw) EHSH_LOG_ERROR("ERROR: Could not open new archive writer");
   archive_write_disk_set_options(aw, flags);
   archive_write_disk_set_standard_lookup(aw);
  /* Create the stream object to read from */
  a = archive_read_new();
  archive_read_support_compression_all(a);
  archive_read_support_format_all(a);

  r=archive_read_open(a, &ObjData->AppGet_write_data_buffer_struct, NULL, write_archive_to_disk, NULL);
  if (r != ARCHIVE_OK) {
	  EHSH_LOG_ERROR("Could not open networked archive\n");
	  goto error;
  }
  else {
	 /* Opened File OK */
  }
  EhsTF_rmdir(ObjData->szpDownloadDir); /* in case it exists is a directory*/
  EhsHRemove(ObjData->szpDownloadDir); /* in case it exists as a file*/
  //if (EhsTF_mkdir(ObjData->szpDownloadDir)) { /* Don't need this - libarchive creates the directory */
  //	  printf("Created download Directory %s OK",ObjData->szpDownloadDir);
  // }
  for (;;) { // loop through all the files
		r = archive_read_next_header(a, &entry); //assume this waits until the call back has provided enough data.
		if (r == ARCHIVE_EOF) {
			//EHSH_LOG_INFO("Found end of archive reading headers");
			headers_done = EHS_TRUE; // This causes the loop to break;
			break; //but lets just break!
		} else if (r != ARCHIVE_OK) {
			//EHSH_LOG_ERROR("Read header error %s [%s]",archive_error_string(a), archive_error_string(aw));
			//if (r < ARCHIVE_WARN)
			EHSH_LOG_ERROR("Critical error in appget - giving up");
			appget_success = EHS_FALSE;
			//goto error; //if we exit here before trying to read all the data then the URL getter may get locked up..
			break;
		} else if (r != ARCHIVE_EOF) { //read the data and write the file
			if (ObjData->szpDownloadDir[1] == ':') EhsStrcpy(szfilename, &ObjData->szpDownloadDir[2]); /* libcurl does someting weird with ":"s so skip them*/
			else EhsStrcpy(szfilename, ObjData->szpDownloadDir);
			EhsStrcat(szfilename, EHS_TD_FILES_SEPARATOR_STR);
			EhsStrcat(szfilename, archive_entry_pathname(entry));
			archive_entry_set_pathname(entry, szfilename);// ObjData->szpDownloadDir);
			r = archive_write_header(aw, entry);
			if (r != ARCHIVE_OK) {
				EHSH_LOG_ERROR("Couldn't Write File from tar header");
				appget_success = EHS_FALSE;
				//goto error;
				break;
			}
			entrysize = archive_entry_size(entry);
			if (entrysize > 0) {
				for (;;) {
					r = archive_read_data_block(a, &databuf, &size[0], (off_t *)&offset[0] );/* tod work out hat is going on here with data types off_t*/
					appget_success = EHS_FALSE;
					if (r == ARCHIVE_EOF) {
						appget_success = EHS_TRUE; /* at least one file is OK */
						break; // - Seems we do break - EOF means no more data.
					} else if (r != ARCHIVE_OK && r != ARCHIVE_RETRY) {
						EHSH_LOG_ERROR("Error in File write %s",archive_error_string(a));
						appget_success = EHS_FALSE;
						break;
					}
					if (r == ARCHIVE_OK || r == ARCHIVE_EOF) { /* Only try and write of we have an OK read */
						//printf("DataBuff[%zd]=%.*s\n",size,size,(char*)databuf);
						//printf("XXXDataBuff[%d]=%.50s\n",(int)size,databuf);
						r = archive_write_data_block(aw, databuf, size[0],
								offset[0]); //trust ar is initialised

						if (r != ARCHIVE_OK && r != ARCHIVE_RETRY) {// && r != ARCHIVE_WARN ) {
							EHSH_LOG_ERROR(	"Archive write error %s\n errno =%s\n",
									archive_error_string(aw), strerror(errno));
							appget_success = EHS_FALSE;
							//goto error;
							break;
						}
					}
					if (appget_success == EHS_TRUE) {
						//appget_success=EHS_FALSE; //need a flag for each file and one for entire archive.
						break; //goto good.
					}
				}
			}
		}
		if (headers_done == EHS_TRUE)
			break;
	}
	/* write OK so flip the active application - not use the download directory - not the canpnical name*/
  good: /* Gets here too */
  error:
  //@todo archive_read_free and archive_write_free is deprecated in version 3
  if (a) archive_read_finish(a);
  if (aw) archive_write_finish(aw);

  if (appget_success == EHS_TRUE) {
		EhsAppSetDownloadOKToken(ObjData->szpDownloadDir); // make it live.

		EhsAppCheckAndSwitchDownloadDir(ObjData->szAppLocalName); /* Check for download dir and shuffle if required - do this here in case don't run immediately, we don't want app folder left with '_dl' suffix */

		// if this is flagged as a default app then write  to file
		if (ObjData->bSetAsDefaultApp == EHS_TRUE) {
			EhsAppSetDefaultApp(ObjData->szAppLocalName);
			EHSH_LOG_INFO("New App set as default\n");
		}

		if (ObjData->bRunWhenDone == EHS_TRUE) {
			for (i = 0; i < 10; i++) { //try a few times in case there are other things going on
				EHSH_LOG_INFO("Running New App");
				//printf("Running New App\n");
				// this is 'app to run next', don't write to app2run file - just to EhsHMetaSetNextAppToRun for when EHS restarts
				EhsHMetaSetNextAppToRun(ObjData->szAppLocalName);

				if (EhsHFSMSetInternallyRequestedCommand(EHS_RELOAD_EHS_FROM_FILE) == EHS_TRUE) {
					break;
				}
				EhsSleep(EHS_TIME_us(1000)); /* = 10ms wait at longest - the give up */
			}
		}
  }
  ObjData->bBusy = EHS_FALSE; /* this may signal a dealloc of ObjData; */
  if (ObjData->bFreeWhenDone ) {
	  curl_easy_cleanup(ObjData->curl);
	  EhsHMem_permFree(ObjData);
  }
}

#define UNWISE_MAX(a, b) ((a) < (b) ? (b) : (a))

/* this thread should/will always exit before HAL_appGetWaitForURLDataAndWrite exits */

EhsThreadFuncReturnType HAL_AppGetRead_data(void *XObjData)
{
	//appgetObj *ObjData = (appgetObj*) EHS_FB_RUN_CONTEXT;
	long ret32;
	ehs_bool ret=EHS_FALSE;
	appgetObj *ObjData= (appgetObj *)XObjData;
	ehs_char szTempString[UNWISE_MAX(EHS_MAXDEVMANNAMELEN,EHS_SYS_MAXPATHLENGTH)];
	ehs_char * post_data = EhsHMem_permAlloc(EHS_POST_STRING_LENGTH_MAX); //*Can't use tempAlloc outside run app context: EhsHMem_tempAlloc(EHS_POST_STRING_LENGTH_MAX); ?*/
	if (NULL == post_data) goto error;
	post_data[0]='\0';
	ehs_bool appget_success=EHS_FALSE;
	CURLcode success;
	AppGet_URL_data_buffer_Type * buffer_struct;
	/* Read in the appname if there is one */
	/* Invariant initialisation */
	ObjData->curl = curl_easy_init(); /* We can call this multiple times apparently *///@todo check that curl_global_init() is called before threads start
	if (!ObjData->curl) {
		EHSH_LOG_ERROR("Error: Could not init CURL - Skipping call\n");
		goto error;
	}
#define EHS_USE_CURLS_LAME_SIGNAL_AVOIDANCE
	//- THIS IS NEEDED APPARENTLY!
#ifdef EHS_USE_CURLS_LAME_SIGNAL_AVOIDANCE
	/* Some system specific configs @todo review the x-platform validity*/
	curl_easy_setopt(ObjData->curl, CURLOPT_NOSIGNAL, 1); /* avoid long jump crash - remove signal handlers @todo may have DNS look up time out problem).
	 @todo Consider building libcurl with c-ares support to enable asynchronous DNS lookups*/
	/* There is not time out used yet - perhaps use low data rate instead as this doesn't needCURLOPT_NOSIGNAL */
#endif
	/* Add any ssl certificates */
	//EhsHSetUpClientTlsCertificate(ObjData->curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY, NULL /* combined in PEM */, NULL);
#warning "put the above ack in"
	EhsHSetUpCaTlsCertificate(ObjData->curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CA_CERTIFICATE);

	EhsHSetUpServerSecurity(ObjData->curl,&ObjData->server_info);
	EhsHSetUpLocalProxy(ObjData->curl); // this picks up the global values for the device
	/* Reset the buffer struct so it reads */
	ObjData->AppGet_write_data_buffer_struct.size_read = 0; //get it ready to go.
	/* but overwrite the data handler call-back */
	success = curl_easy_setopt(ObjData->curl, CURLOPT_WRITEFUNCTION, AppGet_URL_data_CB);
	if (CURLE_OK != success) {
		EHSH_LOG_ERROR("HAL URL: CURLOPT_WRITEFUNCTION option set failed");
		goto error;
	}
	success = curl_easy_setopt(ObjData->curl, CURLOPT_WRITEDATA,
			(void*) &(ObjData->AppGet_write_data_buffer_struct)); //We'll write XML into a buffer in here data directly here
	if (CURLE_OK != success) {
		EHSH_LOG_ERROR("HAL URL:  NOT_WRITE_DATA: option set failed");
		goto error;
	}
	post_data[0] = '\0';
	EhsHCreateQueryString(ObjData->curl, post_data, "AppCanonicalName", ObjData->szAppCanonicalName,EHS_POST_STRING_LENGTH_MAX);
	//EhsHCreatePostString(NULL,PostString, "DEVICEID",ObjData->szAppCanonicalName ,EHS_POST_STRING_LENGTH_MAX); // null because we don't encode
 //todo put in array length checking
	EhsStrcpy(szTempString,ObjData->szUrl);
	if (EhsStrlen(ObjData->szUrlAppGetExtPath)) {
		EhsStrcat(szTempString,EHS_TD_FILES_SEPARATOR_STR);
		EhsStrcat(szTempString,ObjData->szUrlAppGetExtPath); /* This will often be blank, but is included where the struct is used for other devman paths */
	}
	success = curl_easy_setopt(ObjData->curl, CURLOPT_URL, szTempString);

	if (CURLE_OK != success) {
			EHSH_LOG_ERROR("ERROR: CURLOPT_URL CURL=[%x] post=%s", ObjData->curl,szTempString);
		} else {
		success = curl_easy_setopt(ObjData->curl, CURLOPT_POST, 1);
		if (CURLE_OK != success) {
			EHSH_LOG_ERROR("ERROR: CURLOPT_POST");
		}
		else {
			success = curl_easy_setopt(ObjData->curl, CURLOPT_POSTFIELDS, post_data);//PostString);
			if (CURLE_OK != success) {
				EHSH_LOG_ERROR("ERROR: CURLOPT_POSTFIELDS");
			}
			else {
				if (CURLE_OK == success) {
					//printf("Trying to retrieve archive from |%s|\n",szTempString);
					success = curl_easy_perform(ObjData->curl);
					/* Must signal the archive writer that all is done, by faking reading 0 length. */
					ObjData->AppGet_write_data_buffer_struct.size_read=0; /* set zise to non-zero to trigger read */
					ObjData->AppGet_write_data_buffer_struct.data_ready = EHS_TRUE; //trigger the writer to write and wait for it signal finished.
					//do the call
					if (CURLE_OK == success) {
						curl_easy_getinfo(ObjData->curl, CURLINFO_RESPONSE_CODE, &ret32); //we call this as we would have at least read the server file and need to check it there.

						if (ret32 == 200) {
							/* All good! */
							EHSH_LOG_INFO("app URL %s got OK",szTempString);
							//printf("app URL %s got OK\n",szTempString);
							appget_success=EHS_TRUE;
						} else {
							EHSH_LOG_ERROR("Could not access URL %s = error=%d",szTempString,ret32);
							//printf("Could not access URL %s = error=%d\n",szTempString,ret32);
							appget_success=EHS_FALSE;
						}
					} else {
						EHSH_LOG_ERROR("ERROR: CURLINFO_RESPONSE_CODE [%s] = %d",szTempString,success);
					}
				} else {
					EHSH_LOG_ERROR("ERROR: appget get url perform failed ");
				}
			}
		}
	}
	//curl_easy_cleanup(ObjData->curl);
	if (appget_success) {
		ret=EHS_TRUE;
		goto end_done;
	}
	else {
		goto error;
		ret=EHS_FALSE;
	}
	error:
	//EhsAppMakeDownloadString(szTempString,ObjData->szAppCanonicalNameX);
	EhsTF_rmdir(ObjData->szpDownloadDir); /* download broken so remove download directory*/
	//EHS_FB_FINISH(EHS_GETAPP_GETAPP_ERROR_EO);
	end_done: /* tidy up */
	if (post_data)  EhsHMem_permFree(post_data);
	/* The following must be done when all is tidy and de-alloced - ideally! */
	return ret;
}


/* @todo implement global blocking flag */
ehs_bool EHSHAL_bAppGetLocked =EHS_FALSE;
/*
 * Use this function to kick off an application download for the default application.
 *
 * This blocks until it is complete if block flag is set
 */

ehs_bool start_appget_getapp(const ehs_char * appname, const ehs_char * localname, ehs_char * server_base, ehs_bool block, ehs_bool runFlag, ehs_bool defaultAppFlag){
	//appgetObj *ObjData = (appgetObj*)EHS_FB_RUN_CONTEXT;
	appgetObj *ObjData;
#ifdef EHS_ANDROID
	EHSH_LOG_INFO("start_appget_getapp");
#else //EHS_ANDROID
	block=EHS_FALSE;// don't allow this for now.
#endif
	ehs_char * cPathSafeLocalName = EhsHMem_tempAlloc(EHS_MAXPATHLENGTH);
	ehs_char * s;

	// first trim white spaces from end, causing a problem with path concatenation otherwise
	EhsStrcpy(cPathSafeLocalName,localname);
	s = cPathSafeLocalName;
	int end = strlen(s) - 1;
	while (isspace(s[end]) && (end >= 0)) {
		end--;
	}
	s[end+1] = '\0';
/* - unnecessary, file system can cope with white spaces in filename
	// replace white spaces with underscores
	while (*s != '\0') {
		if (*s == ' ') {
			*s = '_';
		}
		s++;
	}
*/

	CURL * curl=curl_easy_init(); /*@todo for some reason assigning this straight into the structure causes it to screw up - why */
	if (EHSHAL_bAppGetLocked == EHS_FALSE) {
		ObjData=EhsHMem_permAlloc(sizeof(appgetObj));
		if (ObjData) {
			EhsStrcpy(ObjData->szUrl,server_base); /* Use this server to look for apps */
			EhsStrcpy(ObjData->szUrlAppGetExtPath,"");// you might want to put an extendor in here - but assuming the server_base is a full path
			ObjData->bBusy = EHS_FALSE;
			ObjData->curl = curl; /* We can call this multiple times apparently *///@todo check that curl_global_init() is called before threads start
			//@Note this is where the struct gets nobbled by the curl ptr - printf("XXXXXXXXX3 Server = [%x] %s\n",(int)ObjData->szUrl, ObjData->szUrl);
			if (!ObjData->curl) {
				EHSH_LOG_ERROR("Could not initialise CURL");
				goto error;
			}
			ObjData->server_info.http_username[0] = '\0';
			ObjData->server_info.http_password[0] = '\0';
			ObjData->server_info.authentication = 0;
			ObjData->URL_write_data_buffer_struct = NULL;
			ObjData->bBusy = EHS_TRUE;
			EhsStrcpy(ObjData->szAppCanonicalName,appname);
			EhsStrcpy(ObjData->szAppLocalName,cPathSafeLocalName);

			ObjData->bRunWhenDone = runFlag;
			ObjData->bSetAsDefaultApp = defaultAppFlag;
			if (block) ObjData->bFreeWhenDone = EHS_FALSE; /* we keep this lying about for function blocks! */
			else ObjData->bFreeWhenDone = EHS_TRUE;
			/* Set up download location */
			EhsAppMakeDownloadString(ObjData->szpDownloadDir,ObjData->szAppLocalName);

			//EHSH_LOG_ERROR("ObjData->szAppCanonicalName=[%s]\n",ObjData->szAppCanonicalName);
			//EHSH_LOG_ERROR("ObjData->szAppLocalName=[%s]\n",ObjData->szAppLocalName);
			//EHSH_LOG_ERROR("ObjData->szpDownloadDir=[%s]\n",ObjData->szpDownloadDir);

			/* Start URL get thread to pump data into a buffer */
			EhsHThread_execute( HAL_AppGetRead_data, (void*) ObjData,-95) ;
			/* Start Incremental archive decoder/writer */
			EhsHThread_execute(HAL_appGetWaitForURLDataAndWrite, (void*) ObjData,-95) ; /* this thread will deallocate ObjData when done */
			if (block) {

				while (ObjData->bBusy) {EhsSleep(EHS_TIME_us(10000));}
				curl_easy_cleanup(ObjData->curl);
				//@todo timeout should be included
				 EhsHMem_permFree(ObjData);
				 ObjData=NULL;
			}
			else  EHSH_LOG_INFO("NOT Waiting for app get to finish");

			/* Not here... EhsHMem_permFree(ObjData); */
		} else {
			goto error;
		}
	} else {
		goto error;
	}
	good: return EHS_TRUE;
	error:
	if (ObjData) EhsHMem_permFree(ObjData);
	return EHS_FALSE;
}

