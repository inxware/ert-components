/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/** @file devman_mon.c
 * This file provides the definitions to run devman monitor
 *
 * @author: inx limited
 *
 */
/****************************************************************/
/* Included files */
// @todo this should be moved to the hall

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

#include "hal-api.h"
#include "hal_devman.h"
#include "app_data.h"
#include "globals.h"
#if EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT
#include "devman_mon_mqtt.h"
#include "devman_mon_ota.h"
#endif

#ifdef EHS_DEVMAN_LUA_VERSION
#ifdef EHS_LUA_SUPPORT
//#include "lua.h" done in hal_lua.h
#include "lualib.h"
#include "lauxlib.h"
#endif
#endif



#define EHS_DEVMANMON_POLL_RATE_SECONDS 20


/* Note these files shouldn't be used */
#ifndef EHS_DEVMAN_COREMONURLS
#define EHS_DEVMAN_COREMONURLS	"plugins/0/DEVMANMONURL"
#endif

#ifndef EHS_DEVMAN_CORE_MAX_SERVER_URLS
#define EHS_DEVMAN_CORE_MAX_SERVER_URLS 10
#endif


/*****************************************************************************/
/* Function definitions */

/********************* @todo FUNCTIONS TO BE MOVED TO A NEW DEVMAN HAL MODULE @todo ***************************/

/*
 * always returns a URL - if URL not found in filesystem then use #def baseURL
 *
 */
void EhsHDevmanGetCoreURL(ehs_char * szUrl)
{
    if (!EhsHDevmanGetURL(szUrl, EHS_DEVMAN_COREURLS, EHS_MAXDEVMANNAMELEN,0))   /* We don't have any fail over system for apps so index is 0 */
    {
        EhsStrcpy(szUrl, EHS_DEVMAN_DEFAULTBASEURL); /*todo this should be a parameter */
    }
}

/*
 * @brief Returns a valid URL from the list or returns an error number or status number.
 * The file name passed is in fact appended with the index number and seperate files are used for each to
 * avoid potential for file corruption.
 *
 * @note Leaves the url string pointer content unchanged if there are no URLs specified.
 * */

ehs_bool EhsHDevmanGetURL(ehs_char * URL, ehs_char * list_file,  ehs_uint16 maxlen, ehs_uint16 index)
{
    ehs_bool retval = EHS_FALSE;
    ehs_FILE *file;
    ehs_char szTemp[EHS_DEVMAN_FILE_PATH_LENGTH];
    ehs_uint16 end;
    URL[0]='\0';
    if (index < 0 || index > 4000 )
    {
        EHSH_LOG_INFO("Incorrect index to getURL data");
        retval = EHS_FALSE;
    }
    else
    {
        EhsSprintf(szTemp, "%s.%03d", list_file, index);
        file = Ehs_DevmanFopen(szTemp, "r"); //fopen();

        if (file)
        {
            EhsFgets(URL, maxlen, file);
            EhsFclose(file);

            end = EhsStrlen(URL); /* trim off any crud */

            if (end > 0)
            {
                if (URL[end] == '\r' || URL[end] == '\n')
                    URL[end] = '\0';
                if (URL[end - 1] == '\r' || URL[end - 1] == '\n')
                    URL[end - 1] = '\0';
                retval = EHS_TRUE;
                /*} Why would we want to do this here?
                 *  else { / * Use a hardwired default * /
                 EhsStrcpy(URL,EHS_DEVMAN_DEFAULTBASEURL);//Default if nothing is returned.
                 retval = EHS_FALSE;
                 */
            }
            else retval = EHS_FALSE;
        }
        else     /* No file - leave URL in tact */
        {
            retval = EHS_FALSE;
        }
    }
    return retval;
}

/* Changes the Devman URL config file and checks for new Certificate files if they exist */

ehs_bool EhsHDevmanAddURLtoHeadList(ehs_char * ehs_path, ehs_char * URL)
{
    //@todo
    ehs_uint16 i;
    ehs_sint16 j; //j must be signed so for loop works..
    ehs_FILE *file;
    ehs_bool ret=EHS_FALSE;
    ehs_char * szTemp2;
    ehs_char * szTemp = EhsHMem_tempAlloc(EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp == NULL)
        return EHS_FALSE;
    szTemp2 = EhsHMem_tempAlloc(EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp2 == NULL)
        return EHS_FALSE; //lazy escape..
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_DEVMAN_DIR,ehs_path,EHS_TRUE);

    for (i = 0; i < 999; i++)   // loop to find out how many we have in the list. Gaps are the end of the list!
    {
        EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, i); // test if i index file exists.
        if (EhsTF_exists(szTemp) != 1)
        {
            break;
        }
    }
    for (j = i; j >= 0; j--)   // increment name of any files before gap in list, so .000 becomes .001, .001 becomes .002 etc
    {
        EhsSprintf(szTemp2, "%s.%03d", szCanonicalFilePath, j); // this file should exist (this is the old name)
        if (i > 0)   // if we have an existing one make new name for it and rename
        {
            EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, j+1); // this is the to name (one index up)
            EhsHRename(szTemp2, szTemp);
        }
    }
    //} else { // szTemp is in tact - file doesn't exist so write it.
    //}
    // now update the entry in .000
    EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, 0); // this is the to name (one index up)
    file = EhsFopen(szTemp,"w");
    if (file && URL)
    {
        EhsFwrite(URL, 1, EhsStrlen(URL), file); // and write the data
        EhsFclose(file);
        /* Remove duplicates so we don't push nice ones off the end of the list */
        EhsHDevmanRemoveSpecificURLFromList(ehs_path,URL, 1);
        ret=EHS_TRUE;
    }
    else
    {
        ret=EHS_FALSE;
    }
    EhsHMem_tempFree(szTemp);
    EhsHMem_tempFree(szTemp2);
    return ret;
}

// Removes all URLS larger than a particular index 0-based).
//@todo we also need one that will find the URL string in the list and remove it.
#define EHS_DEVMAN_MAXLIST_GAP 3
ehs_bool EhsHDevmanRemoveURLSFromList(ehs_char * ehs_path, ehs_uint16 index)
{
    //@todo
    ehs_uint16 i,maxgapcount=0;
    ehs_char *szTemp = EhsHMem_tempAlloc(EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp == NULL)
        return EHS_FALSE; //lazy escape..
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_DEVMAN_DIR,ehs_path,EHS_TRUE);

    // remove the file
    for (i = index; i < 999; i++)   // loop to find out how many we have in the list. Gaps are the end of the list!
    {
        EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, (int) i); // test if 0 index file exists.
        if (EhsTF_exists(szTemp) != 1)
        {
            if (maxgapcount++ > EHS_DEVMAN_MAXLIST_GAP)
                break;
        }
        else     // remove!
        {
            EhsHRemove(szTemp);
        }
    }
    EhsHMem_tempFree(szTemp);
    if (i == index)
        return EHS_FALSE; // we didn't find one
    else
        return EHS_TRUE;
}

/*
 * This only remove the head element if there is more than 1 element in the list
 * Returns error otherwise.
 *
 * Don't need this..
 * @todo this is untested code
 */
ehs_bool EhsHDevmanRemoveHeadURLFromList(ehs_char * ehs_path)
{
    //@todo move code to the target specific implementation
    ehs_sint16 i;

    ehs_char * szTemp2;
    ehs_char *szTemp = EhsHMem_tempAlloc(EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp == NULL)
        return EHS_FALSE;
    szTemp2= EhsHMem_tempAlloc(EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp2 == NULL)
        return EHS_FALSE;
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_DEVMAN_DIR,ehs_path,EHS_TRUE);

    EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, 1); // test if There is a second server listed
    if (EhsTF_exists(szTemp) != 1)   //@todo should check the content looks like a URL?
    {
        EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, 0);
        EhsHRemove(szTemp);
        for (i = 1; i < 999; i++)   // shift all the others up
        {
            EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, i);
            EhsSprintf(szTemp2, "%s.%03d", szCanonicalFilePath, i-1);
            if (EhsTF_exists(szTemp) != 1)
            {
                break;
            }
            else
            {
                EhsHRename(szTemp, szTemp2);
            }
        }
    }

    EhsHMem_tempFree(szTemp);
    EhsHMem_tempFree(szTemp2);
    return EHS_TRUE;
}

/* remove entries of the given string if it is in the list at or after the startposition value */
ehs_bool EhsHDevmanRemoveSpecificURLFromList(ehs_char * ehs_path, ehs_char * URL, ehs_uint16 startposition)
{
    //@todo move code to the target specific implementation
    ehs_bool bRemovedFile = EHS_FALSE;
    ehs_sint16 i;
    ehs_uint16 j,maxlen; //j must be signed so for loop works..
    ehs_FILE *file;
    ehs_char * szTemp2;
    ehs_char *szTemp = EhsHMem_tempAlloc(EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp == NULL)
        return EHS_FALSE;
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_DEVMAN_DIR,ehs_path,EHS_TRUE);
    maxlen = EhsStrlen(URL);
    szTemp2 = EhsHMem_tempAlloc(maxlen>EHS_DEVMAN_FILE_PATH_LENGTH?(sizeof(char)*maxlen):EHS_DEVMAN_FILE_PATH_LENGTH);
    if (szTemp2 == NULL)
        return EHS_FALSE;

    for (i = startposition; i < 999; i++)   // loop to find out how many we have in the list. Gaps are the end of the list!
    {
        EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, i);
        if (EhsTF_exists(szTemp) == 1)
        {
            file = EhsFopen(szTemp, "r"); //fopen();
            if (file)
            {
                szTemp2[0] = '\0';
                //for (i = 0; i < index; i++) {
                EhsFgets(szTemp2, maxlen+1, file);
                EhsFclose(file);
                if (strcmp(URL, szTemp2) == 0)
                {
                    EhsHRemove(szTemp);
                    bRemovedFile = EHS_TRUE;
                    break; //we have a match. We'll only look for one.
                }
            }
        }
        else
        {
            return EHS_FALSE; // not found - don't do gap foregiveness?
        }
    }

    if (bRemovedFile)  	//removed file index i
    {
        for (j = i+1; j < 999; j++)   // shift all the others up
        {
            EhsSprintf(szTemp, "%s.%03d", szCanonicalFilePath, j);
            EhsSprintf(szTemp2, "%s.%03d", szCanonicalFilePath, j - 1); // this is the target file name
            if (EhsTF_exists(szTemp) != 1)
            {
                break;
            }
            else
            {
                EhsHRename(szTemp, szTemp2);
            }
        }
    }
    EhsHMem_tempFree(szTemp);
    EhsHMem_tempFree(szTemp2);
    return EHS_TRUE;
    return EHS_TRUE;
}


/*
 * Gets the BASE URL or a failover to inx if there isn't one.
 * This function might one day try multiple locations for fined grained control of URLs - but this has been removed for now....
 *
 * */

ehs_bool GetDevmanBASEURL(ehs_char * szUrl)
{
    /* This might be useful for the app get version ... */
#ifdef EHS_ALLOW_DEVMAN_URL_VARIATIONS_NOT
    if (!EhsHDevmanGetURL(szUrl, "plugins/0/DEVMANAPPURL", EHS_MAXDEVMANNAMELEN,0))   /* We don't have any fail over system for apps so index is 0 */
    {
        // else try the DEVMANMON base URL and extend
        if (!EhsHDevmanGetURL(szUrl, "plugins/0/DEVMANMONURL",EHS_MAXDEVMANNAMELEN, 0))   /* We don't have any fail over system for apps so index is 0 */
        {
            if (!EhsHDevmanGetURL(szUrl, EHS_DEVMAN_COREURLS,EHS_MAXDEVMANNAMELEN, 0))   /* We don't have any fail over system for apps so index is 0 */
            {
                EhsStrcpy(szUrl, EHS_DEVMAN_APPGETDEFAULTURL); //todo this should be a parameter
            }
            else     /* we need to append the default URI path */
            {
                EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY); //todo this should be a parameter
            }
            EhsStrcpy(szUrl, EHS_DEVMAN_APPGETDEFAULTURL); //todo this should be a parameter
        }
        else     /* we need to append the default URI path */
        {
            EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY); //todo this should be a parameter
        }
    }
#else

    if (!EhsHDevmanGetURL(szUrl, EHS_DEVMAN_COREURLS, EHS_MAXDEVMANNAMELEN,0))   // We don't have any fail over system for apps so index is 0
    {
        EhsStrcpy(szUrl, EHS_DEVMAN_DEFAULTBASEURL); /*todo this should be a parameter */
    }
    else     /* we DONT  append anything this is for base bits */
    {
        //EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY); /*todo this should be a parameter */
    }
#endif
    return EHS_TRUE;
}

extern EhsMetaDataType EhsMetaData;
extern EhsApplicationMetaDataType EhsApplicationMetaData;


/***************************************************************************************/

#if EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT

/* Devman mon mqtt subscription callback handler */
ehs_bool EhsMqttDevmanMonSubscriptionCallback(struct inx_mqtt_subscribe_state* pState, char* payload, ehs_sint32 payloadSize)
{
    //inx_mqtt_client_state_type_mine* pMqttClient = EhsMqttDevmanMonMqttClient();
    if(pState == NULL){
        return EHS_FALSE;
    }

    ehs_sint16 topic_id = EhsSubscriptionTopicID(pState->topic);
    switch(topic_id){
        /* handle ota subscriptions */
        case EHS_MQTT_SUB_TOPIC_START_OTA_ID:{
            payload[payloadSize] = '\0'; // make sure payload is NULL terminated
            EhsOtaDevmanMonStart(payload, payloadSize);
            break;
        }
        case EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA_ID:{
            EhsOtaDevmanMonWrite(payload, payloadSize);
            break;
        }
        default:
            // unknown
            break;
    }

    return EHS_TRUE;
}

/* request next OTA chunk */
void EhsOtaDevmanMonNext(const ehs_char* payload)
{
    EhsMqttDevmanMonPublish(EHS_MQTT_PUB_TOPIC_GET_CHUNK_OTA_ID, payload);
}

/* avoid blocking this function! It's a callback which gets called from mqtt clinet loop. */
void* DevmanMonThread(void* arg)
{
    inx_mqtt_client_state_type_mine* pMqttClient = EhsMqttDevmanMonMqttClient();

    switch(EhsGetMqttDevmanMonState())
    {
        case MQTT_DEVMAN_MON_INIT:
        {
            // populate mqtt clinet broker details
            pMqttClient->host[0]='\0';
            // read mqtt url from devman core file
            GetDevmanBASEURL(pMqttClient->host); /* Get our default URL */

            // read and set clinet id
            const char* clientid = EhsHMetaGetHWID();
            printf("Devman mon mqtt clinet id = %s \n", clientid);
            EhsStrcpy(pMqttClient->clientid, clientid);

            // read and set username and password
            pMqttClient->username[0]='\0';
            if (EhsHDevmanGetURL(pMqttClient->username, "core/config/uname", EHS_MAXDEVMANNAMELEN, 0) == EHS_FALSE){
                EHSH_LOG_ERROR("Failed to read user name config file for Devman mon MQTT clinet. \n");
            }
            pMqttClient->password[0]='\0';
            if (EhsHDevmanGetURL(pMqttClient->password, "core/config/passw", EHS_MAXDEVMANNAMELEN, 0) == EHS_FALSE){
                EHSH_LOG_ERROR("Failed to read password config file for Devman mon MQTT clinet. \n");
            }
            #if EHS_DEVMAN_MQTT_CLIENT_TLS
            printf("Devman mon mqtt clinet TLS enabled \n");
            // set expected mqtt broker certificate file names
            EhsStrcpy(pMqttClient->clientCertFileName, "client.crt");
            EhsStrcpy(pMqttClient->clientKeyFileName, "client.key");
            EhsStrcpy(pMqttClient->rootCAFileName, "ca.crt");

            pMqttClient->tls = EHS_TRUE;
            pMqttClient->port = 8883;
            #else
            printf("Devman mon mqtt clinet TLS disabled \n");
            pMqttClient->clientCertFileName[0]='\0';
            pMqttClient->clientKeyFileName[0]='\0';
            pMqttClient->rootCAFileName[0]='\0';
            pMqttClient->tls = EHS_FALSE;
            pMqttClient->port = 1883;
            #endif

            EhsSetMqttDevmanMonState(MQTT_DEVMAN_MON_CONNECT);
            break;
        }
        case MQTT_DEVMAN_MON_CONNECT:
        {
            pMqttClient->connect=EHS_TRUE;
            EhsSetMqttDevmanMonState(MQTT_DEVMAN_MON_CONNECTING);
            break;
        }
        case MQTT_DEVMAN_MON_CONNECTING:
        {
            if(EhsGetMqttDevmanMonConnected()){ // check if mqtt clinet has connected
                EhsSetMqttDevmanMonState(MQTT_DEVMAN_MON_PUB_SUB_INIT);
            }
            break;
        }
        case MQTT_DEVMAN_MON_PUB_SUB_INIT:
        {
            if(EhsGetMqttDevmanMonConnected()){
                // register subscription topics
                EhsMqttDevmanMonRegisterSub(EhsMqttDevmanMonSubscriptions(EHS_MQTT_SUB_TOPIC_START_OTA_ID), EHS_MQTT_SUB_TOPIC_START_OTA, pMqttClient->clientid);
                EhsMqttDevmanMonRegisterSub(EhsMqttDevmanMonSubscriptions(EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA_ID), EHS_MQTT_SUB_TOPIC_POST_CHUNK_OTA, pMqttClient->clientid);
                
                // register publish topics
                EhsMqttDevmanMonRegisterPub(EhsMqttDevmanMonPublications(EHS_MQTT_PUB_TOPIC_GET_CHUNK_OTA_ID), EHS_MQTT_PUB_TOPIC_GET_CHUNK_OTA, pMqttClient->clientid);
                EhsMqttDevmanMonRegisterPub(EhsMqttDevmanMonPublications(EHS_MQTT_PUB_TOPIC_DIDCONNECT_ID), EHS_MQTT_PUB_TOPIC_DIDCONNECT, pMqttClient->clientid);

                EhsSetMqttDevmanMonState(MQTT_DEVMAN_MON_RUNNING);
            }else{
                EHSH_LOG_ERROR("MQTT clinet is not connected in MQTT_DEVMAN_MON_SUBSCIBE state \n");
            }
            break;
        }
        case MQTT_DEVMAN_MON_RUNNING:
        {
            if(EhsGetMqttDevmanMonConnected()){
                EhsMqttDevmanMonHandleConnected();
            }else{
                EhsMqttDevmanMonHandleDisconnected();
                // @TODO - change state, if needed ? Seems to be handled by mqtt state.
            }
            // process any pending states of the OTA
            EhsOtaDevmanMonProcess();
            break;
        }
        default:
        {
            break; // unknown state
        }
    };

    return NULL;
}

#else // EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_CURL
/* Note this doesn't use an XML parser - to reduce core dependencies */

ehs_bool ParseDevmanMonitorXML(ehs_char * returndata)
{
    char * element_start;
    char * attrib;
    ehs_char * temp;
    char element_cropped[EHS_STRING_LENGTH_MAX];
    if (returndata == NULL|| EhsStrcmp(returndata, "") == 0) return EHS_TRUE; // always succeeds?
    ehs_char szpDownloadDir[EHS_MAXPATHLENGTH]; /* @todo malloc this */

    // got some pairing info
    if ( (element_start = Ehs_ReadXMLTag(returndata, "PairedOrganisationID")))
    {
        Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
        EhsHSysUpdatePairedOrganisation((ehs_uint32)atoi(element_cropped)); /* if the data wasn't valid or we get 0 then - this means not paired */
    }
    else   // if we are pending data and we got none then assert no-pairing.
    {
        /*special values:  0: no pairing data updates, 1 pairing data valid, 2 pairing data pending, 3 paring data invalid*/
        if (EhsHSysGetPairedOrganisationStatus() == 2)  EhsHSysUpdatePairedOrganisation(0xFFFF);/* set to invalid data */
    }
    if ( (element_start = Ehs_ReadXMLTag(returndata, "ChangeDevmanURL")))
    {
        Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE);
        EhsHDevmanAddURLtoHeadList(EHS_DEVMAN_COREURLS,element_cropped);
    }
#if 0
    if ((element_start = Ehs_ReadXMLTag(returndata, "SetStaticIpAddr")))
    {
        Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE); // expects a JSON string:{ ipaddr="x.x.x.x", gateway="x.x.x.x", mask="x.x.x.x", dns1="x.x.x.x" }
        EhsHSysSetStaticIpAddr(EHS_DEVMAN_COREURLS,element_cropped);
    }
#endif
    if ((element_start = Ehs_ReadXMLTag(returndata, "UpdateDeviceDate")))
    {
        Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE);
        EhsHSysUpdateDate(element_cropped);	//@todo - not implemented
    }
    if ((element_start = Ehs_ReadXMLTag(returndata, "SetDeviceResetTime")))
    {
        Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE);
        //EhsHSysDeviceResetTime(element_cropped);	//@todo - not implemented
    }
    /* Clear Storage removes data older than X days */
    if ((element_start = Ehs_ReadXMLTag(returndata, "ClearStorage")))
    {
        Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE);
        EhsHFileClearUserData(element_cropped);	//@todo - not implemented
    }
    if ((element_start = Ehs_ReadXMLTag(returndata, "Restart")))
    {
        EhsHSysRestart();
    }
    if ((element_start = Ehs_ReadXMLTag(returndata, "Reboot")))
    {
        EhsHSysReboot();	//@todo - not implemented
    }
    /* Check for new applications to be pushed into device */
    if ((element_start = Ehs_ReadXMLTagAttrib(&attrib, returndata, "DevmanMiscAppDLData")))
    {

        ehs_char * value = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX);
        ehs_char * tempBuff = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX);
        value[0]='\0';
        if (attrib)
        {
            if (Ehs_ReadXMLAttribValue(value, attrib, "type"))
                EhsStrcpy(EhsHMetaGetPtrToDevmanMiscDLDataType(),value);
        }
        if (EhsStrncmp(element_start,"<!CDATA[",EhsStrlen("<!CDATA[")) == 0)
        {
            EHSH_LOG_ERROR("Not Implemented CDATA parsing in devman mon");
        }
        else
        {
            ehs_uint16 count = EHS_STRING_LENGTH_MAX-2; // todo - make this more robust using the full named closing tag.
            //ehs_char * copyto = EhsHMetaGetPtrToDevmanMiscDLData();
            ehs_char * copyto = tempBuff;
            while (*element_start != '\0' && *element_start != '<' && count > 0)
            {
                *copyto = *element_start;
                copyto++;
                element_start++;
                count--;
            }
            *copyto = '\0';
            EhsHMetaSetNewDevmanMiscDLDataNew(EHS_TRUE);
            EhsHMetaSetDevmanMiscDLData(tempBuff);
        }
        EhsHMem_tempFree(value);
        EhsHMem_tempFree(tempBuff);
    }
    else
        /* Check for new applications to be pushed into device */
        if ((element_start = Ehs_ReadXMLTagAttrib(&attrib, returndata, "LoadAppPermDefault")))
        {
            ehs_bool runFlag = EHS_FALSE;
            ehs_bool defaultAppFlag = EHS_FALSE;

            ehs_char szUrl[EHS_MAXDEVMANNAMELEN];// App URL/
            GetDevmanBASEURL(szUrl);
            EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY);

            if (attrib)
            {
                ehs_char * value = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX);
                if (Ehs_ReadXMLAttribValue(value, attrib, "run"))
                {
                    temp = EhsStrstr(value, "yes");
                    if (temp)
                        runFlag = EHS_TRUE;
                }
                if (Ehs_ReadXMLAttribValue(value, attrib, "defaultAppFlag"))
                {
                    temp = EhsStrstr(value, "yes");
                    if (temp)
                        defaultAppFlag = EHS_TRUE;
                }
                EhsHMem_tempFree(value);
            }
            EHSH_LOG_INFO(" Getting from %s runFlag[%d],defaultAppFlag[%d]",szUrl,runFlag,defaultAppFlag);

            /* todo this is a bit repetitive - should use another plugin perhaps?? */
            Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE);
            EHSH_LOG_INFO("Downloading application %s from %s\n",element_cropped,szUrl);
            if (start_appget_getapp(element_cropped, EHS_SYS_APP_DEFAULT_NAME,szUrl, EHS_TRUE, runFlag, defaultAppFlag))
            {
                EHSH_LOG_INFO("Downloaded application %s from %s",element_cropped,szUrl);
            }
            else
            {
                EHSH_LOG_ERROR("Couldn't downloaded application %s from %s",element_cropped,szUrl);
            }
        }
        else if ((element_start = Ehs_ReadXMLTagAttrib(&attrib, returndata, "LoadAppPerm")))
        {
            ehs_bool runFlag = EHS_FALSE;
            ehs_bool defaultAppFlag = EHS_FALSE;
            ehs_char szUrl[EHS_MAXDEVMANNAMELEN];// App URL/
            GetDevmanBASEURL(szUrl);
            EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY);
            if (attrib)
            {
                ehs_char * value = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX);
                if (Ehs_ReadXMLAttribValue(value, attrib, "run"))
                {
                    temp = EhsStrstr(value, "yes");
                    if (temp)
                        runFlag = EHS_TRUE;
                }
                if (Ehs_ReadXMLAttribValue(value, attrib, "defaultAppFlag"))
                {
                    temp = EhsStrstr(value, "yes");
                    if (temp)
                        defaultAppFlag = EHS_TRUE;
                }
                EhsHMem_tempFree(value);
            }
            Ehs_CopyXMLTagElement(element_cropped, element_start,EHS_STRING_LENGTH_MAX, EHS_TRUE);
            //EHSH_LOG_INFO
            EHSH_LOG_INFO("Downloading application %s from %s",element_cropped,szUrl);
            if (start_appget_getapp(element_cropped, element_cropped, szUrl,
                                    EHS_TRUE, runFlag, defaultAppFlag))
            {
                //EHSH_LOG_INFO
                EHSH_LOG_INFO("Downloaded application %s from %s",element_cropped,szUrl);
            }
            else
            {
                //EHSH_LOG_WARNING
                EHSH_LOG_ERROR("Couldn't downloaded application %s from %s",element_cropped,szUrl);
            }
        }
        else if ((element_start = Ehs_ReadXMLTagAttrib(&attrib, returndata, "LoadAppTemp")))
        {
            ehs_bool runFlag = EHS_FALSE;
            ehs_bool defaultAppFlag = EHS_FALSE;
            ehs_char szUrl[EHS_MAXDEVMANNAMELEN];// App URL/
            GetDevmanBASEURL(szUrl);
            EhsStrcat(szUrl, EHS_DEVMAN_APPGETDEFAULTURLPATHONLY);

            if (attrib)
            {
                ehs_char * value = EhsHMem_tempAlloc(EHS_STRING_LENGTH_MAX);
                if (Ehs_ReadXMLAttribValue(value, attrib, "run"))
                {
                    temp = EhsStrstr(value, "yes");
                    if (temp)
                        runFlag = EHS_TRUE;
                }
                if (Ehs_ReadXMLAttribValue(value, attrib, "defaultAppFlag"))
                {
                    temp = EhsStrstr(value, "yes");
                    if (temp)
                        defaultAppFlag = EHS_TRUE;
                }
                EhsHMem_tempFree(value);
            }

            Ehs_CopyXMLTagElement(element_cropped, element_start,	EHS_STRING_LENGTH_MAX, EHS_TRUE);
            EHSH_LOG_INFO("Downloading application %s from %s\n",element_cropped,szUrl);
            if (start_appget_getapp(element_cropped, EHS_SYS_APP_TEMP_NAME, szUrl,
                                    EHS_TRUE, runFlag, defaultAppFlag))
            {
                EHSH_LOG_INFO("Downloaded application %s from %s",element_cropped,szUrl);
            }
            else
            {
                EHSH_LOG_WARNING("Couldn't downloaded application %s from %s",element_cropped,szUrl);
            }
        }
    return EHS_TRUE;
}

#ifdef EHS_ENABLE_CURL_VERBOSE
int ehs_curl_devman_mon_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; // unused
    (void)userptr; // unused

    switch (type) {
        case CURLINFO_TEXT:
            EHSH_LOG_ERROR("DEVMON CURL [INFO] %.*s", (int)size, data);
            break;
        case CURLINFO_HEADER_IN:
            EHSH_LOG_ERROR("DEVMON CURL [HEADER_IN] %.*s", (int)size, data);
            break;
        case CURLINFO_HEADER_OUT:
            EHSH_LOG_ERROR("DEVMON CURL [HEADER_OUT] %.*s", (int)size, data);
            break;
        case CURLINFO_DATA_IN:
            // EHSH_LOG_ERROR("DEVMON CURL [DATA_IN] %.*s", (int)size, data);
            break;
        case CURLINFO_DATA_OUT:
          //  EHSH_LOG_ERROR("DEVMON CURL [DATA_OUT] %.*s", (int)size, data);
            break;
        default: /* Do nothing */ ;
    }

    return 0;
}
#endif

void *DevmanMonThread(void *arg)
{
    ehs_char * PostString;
    ehs_char sZtemp[EHS_STRING_LENGTH_MAX*2];// Max size of a date string //TODO:STRINGLENGTH!!!
    ehs_char szUrl[EHS_MAXDEVMANNAMELEN*2];// URL
    ehs_char tempBUff[EHS_STRING_LENGTH_MAX*2];// Mutex data buffer //TODO:STRINGLENGTH!!!
    ehs_uint32 tempint = 0;
    ehs_sint32 retry=EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL; /* this counts how many times we're not using the top of list URL */
    ehs_sint32 retrytimes=EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL;  /* Retries before moving on to next URL */
    ehs_bool trynext=EHS_TRUE; /*flag to identify if the next in the list should be tried */
    //ehs_uint32 ret32=0;
    long http_no = 0L; 
    EhsH_URLwrite_data_bufferType * buffer_struct=NULL;
    static CURL *curl=NULL; //todo this should be made dynamic butt persistent.
    ehs_sint16 CurrentURLindex = 0;
    ehs_bool status = EHS_FALSE;
    ehs_bool sslClientCertificates = EHS_FALSE;
    ehs_bool sslCaCertificates = EHS_FALSE;
    ehs_char *returndata = NULL; // this points to the data generated in EhsH_URLwrite_data_bufferType. It is cleared by the free function for EhsHwrite_data_bufferTypes
    EhsNetworkServerInfo_t server_info = { .http_username = "",.http_password = "", .authentication = 0 };

    // we are not an app instance so don't do this:	Ehs_FB_ThreadStarted();
    /* Start the Lib URL configuration - Use a 64K buffer for the data, and a 50ms chunk delay*/

    // populate szUrl
    //goto curl_init_error;

    szUrl[0]='\0';

    GetDevmanBASEURL(szUrl); /* Get our default URL from file or hard-code fallback */
    EhsStrcat(szUrl, EHS_DEVMAN_MONITORURLPATHONLY); /* This is the ping address */

    if (!curl){
        curl = curl_easy_init(); /* this is thread safe if global init is always done before any other url threads start */
#ifdef EHS_ENABLE_CURL_VERBOSE
        if(curl){
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, ehs_curl_devman_mon_debug_callback);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        }
#endif
    }

    if (!curl)
    {
        EHSH_LOG_ERROR("Could not initialise URL access");
        goto curl_init_error;
    }
    buffer_struct = EhsHDoAllGenericConfig(curl, &server_info, 64 * 1024,50000,120);
    if (buffer_struct)   // create a buffer "large enough for all" - 30s timeouts
    {
        // Create the post
        EhsHSetUpLocalProxy(curl);

        /* Try to load ssl certificates and keys if we are an https site - or even if the URL doesn't indicate https*/

        EHSH_LOG_INFO("Starting Devmon");
#define EHS_USE_DYNAMIC_DOMAIN_CERTIFICATES
#ifdef EHS_USE_DYNAMIC_DOMAIN_CERTIFICATES
        // Look for any SSL vertificates and load them for the request. Failure presumes none are available.
        EhsHGetDevmanCombinedClientCertificateKeyPath(sZtemp,szUrl);
        sslClientCertificates =  EhsHSetUpClientTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR,sZtemp , NULL /* combined in PEM */, NULL);
        // separate cert and key: sslClientCertificates =  EhsHSetUpClientTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY, EHS_DEVMAN_CLIENT_PRIVATEKEY, NULL);
        EhsHGetDevmanCaCertificatePath(sZtemp,szUrl);
        sslCaCertificates =  EhsHSetUpCaTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, sZtemp);
#else
        sslClientCertificates =  EhsHSetUpClientTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CLIENT_CERTIFICATE_KEY, EHS_DEVMAN_CLIENT_PRIVATEKEY, NULL); 
        sslCaCertificates =      EhsHSetUpCaTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, EHS_DEVMAN_CA_CERTIFICATE);
#endif
        /* set up buffers for received data */
        PostString = EhsHMem_permAlloc(EHS_POST_STRING_LENGTH_MAX); //more than enough?
        //
        while (1)
        {
            //update the meta data block
            EhsHMetaUpdateDynamic(); //@todo make this conditional it hasn't been done recently?
            PostString[0] = '\0';

            EhsHMetaGetCpyDevmanMiscULData(tempBUff);

            /* will mutex thiswith other devman calls to minimise server load and libcurl shittyness */
            EhsTPMutex_lock(EhsTPMutex_devman_request);
            EhsHCreateQueryString(curl, PostString, "Group", "DEFAULT",
                                  EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl, PostString, "DeviceType", "DEFAULT",
                                  EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl, PostString, "DeviceID",
                                  EhsHMetaGetHWID(), EHS_POST_STRING_LENGTH_MAX);
            EhsHGetdateTime(sZtemp,EHS_STRING_LENGTH_MAX, EHS_TRUE, 0); //get adate string
            EhsHCreateQueryString(curl, PostString, "device_date", sZtemp,EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "device_system_space_total_KB", EhsHMetaGetStorTotal(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "device_system_space_used_KB", EhsHMetaGetSysUsed(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "device_user_space_total_KB", EhsHMetaGetStorTotal(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "device_user_space_used_KB", EhsHMetaGetStorUsed(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "total_physmem", EhsHMetaGetRAMTotal(), EHS_POST_STRING_LENGTH_MAX);
            //format the version to include platform variant.
            EhsStrcpy(sZtemp,EhsHMetaGetVersion());
            EhsStrcat(sZtemp,":");
            EhsStrcat(sZtemp,EhsHMetaGetTargetVariant());
            EhsHCreateQueryString(curl, PostString, "ehs_version",sZtemp, EHS_POST_STRING_LENGTH_MAX);

            EhsHCreateQueryString(curl, PostString, "ehs_date", EhsHMetaGetBuildDate(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl, PostString, "app_name", EhsHAppMetaGetAppName(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl, PostString, "sdl_date", EhsHAppMetaGetAppDate(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "percproc_ehs", EhsHMetaGetCPUUsage(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "percproc_miscapp", EhsHMetaGetMiscAppCPUUsage(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "ramuse_miscapp", EhsHMetaGetMiscAppRAMUsed_kB(), EHS_POST_STRING_LENGTH_MAX);
            ehs_uint32 ram_total = EhsHMetaGetRAMTotal();
            ehs_uint32 perc_mem = (ram_total) ? (EhsHMetaGetMiscAppRAMUsed_kB()*100)/ram_total : 0;
            EhsHCreateQueryStringNum(PostString, "percmem_miscapp", perc_mem, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryString(curl, PostString, "misc_sys_info", EhsHMetaGetSysInfo(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "ramuse_ehs", EhsHMetaGetRAMUsedEHS_kB(), EHS_POST_STRING_LENGTH_MAX);
            /* misc app info is the json structure collected in the app */
            EhsHCreateQueryString(curl, PostString, "misc_app_info",tempBUff, EHS_POST_STRING_LENGTH_MAX);
            if (EhsHMetaGetRAMTotal())
            {
                tempint = (EhsHMetaGetRAMUsedEHS_kB() * 100) / EhsHMetaGetRAMTotal();
            }
            else
            {
                tempint = 0;
            }
            EhsHCreateQueryStringNum(PostString, "percmem_ehs", tempint, EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "virtmem_ehs", 0, EHS_POST_STRING_LENGTH_MAX);//@todo this isn't implemented yet
            EhsHCreateQueryString(curl, PostString, "ehs_startdate", EhsHMetaGetEHSStartDate(), EHS_POST_STRING_LENGTH_MAX);

            EhsHCreateQueryString(curl, PostString, "ipaddr", EhsHMetaGetIPAddr(), EHS_POST_STRING_LENGTH_MAX);
            EhsHCreateQueryStringNum(PostString, "sdl_version", EhsHAppMetaGetBuildNumber(), EHS_POST_STRING_LENGTH_MAX);
            if (EhsHMetaGetMissedPing() == EHS_TRUE)
            {
                EhsHCreateQueryStringNum(PostString, "devman_mon_missed_pings_duration", EhsHMetaGetCPUMissedPingTime(), EHS_POST_STRING_LENGTH_MAX);
            }
            /* Conditional data/requests to send */
            if (EhsHSysGetPairedOrganisationStatus() == 2  )   // 2 is data pending signal
            {
                EHSH_LOG_INFO("GETTING current pairing info");
                EhsHCreateQueryString(curl, PostString, "get_pairing_info","yes", EHS_POST_STRING_LENGTH_MAX);
            }
            /* check if the URL has changed and find any relevant certs */
#define EHS_USE_DYNAMIC_DOMAIN_CERTIFICATES
#ifdef EHS_USE_DYNAMIC_DOMAIN_CERTIFICATES
            EhsHGetDevmanCombinedClientCertificateKeyPath(sZtemp,szUrl);
            sslClientCertificates =  EhsHSetUpClientTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR,sZtemp , NULL /* combined in PEM */, NULL);
            EhsHGetDevmanCaCertificatePath(sZtemp,szUrl);
            sslCaCertificates =  EhsHSetUpCaTlsCertificate(curl, EHS_RUNTIME_DEVMAN_DIR, sZtemp);
            //printf("SSL CETTIFICATE PATH=%s\n",sZtemp);
            /* Finally do the call*/
#endif
            if (EhsHURLConfigPostGet(curl, buffer_struct, szUrl, PostString,EHS_TRUE) )
            {
                http_no = EhsHURLdoRequest(curl);
                returndata=EhsHURLget_write_data_buffer(buffer_struct);
                //EHSH_LOG_INFO("DONE DEMVMAN_MON_REQUEST GOT [%ld]:[%s]\n with POST=%s",http_no,returndata,PostString);
                //LOGE("DONE DEMVMAN_MON_REQUEST GOT [%s][%ld]:[%s]\n with POST=%s",szUrl,http_no,returndata,PostString);
                //LOGE("DONE DEMVMAN_MON_REQUEST GOT [%ld]:[%s]",http_no,returndata);
            }
            else
            {
                EHSH_LOG_ERROR("DDIDNTLIKE THE POST SETUP");
                http_no=0;
                returndata=NULL;
            }
            EhsTPMutex_unlock(EhsTPMutex_devman_request);
            /* May have populated the return data buffer - assume it is complete*/
            if (http_no == 200)
            {
                if (CurrentURLindex == 0) EhsHMetaResetMissedPingTime(); // reset this if we get a response from the primary server
                status = EHS_TRUE;
                if (CurrentURLindex != 0)   /* if not choice number 1 then try the number one very so often */
                {
                    retry--;
                    if (retry <= 0)   /* We will retry the list every so often if we aren't first choice*/
                    {
                        CurrentURLindex = 0;
                        retry = EHS_DEVMAN_CORE_RETRY_PERIOD_FIRSTURL;
                        if (!EhsHDevmanGetURL(szUrl, EHS_DEVMAN_COREURLS,EHS_MAXDEVMANNAMELEN, CurrentURLindex))
                        {
                            /* Leave URL as it was - no better one exists */
                        }
                        else
                        {
                            EhsStrcat(szUrl, EHS_DEVMAN_MONITORURLPATHONLY); //OK - so add the extra bit
                        }
                    }
                }
            }
            else /* http request failed...*/
            {
                EHSH_LOG_ERROR("FAILED TO DO DEMVNANMON POST [%s] errocode=%d:",szUrl,(ehs_sint32) http_no);
                EhsHMetaSetMissedPing();
                status = EHS_FALSE;
                trynext=EHS_TRUE;
                EHSH_LOG_WARNING("No response from [%s]\n",szUrl);
                
                if (CurrentURLindex == 0 )   /* try a bit harder with the first on the list - only skip to next after a few tries */
                {
                    retrytimes--;
                    if (retrytimes<=0)
                    {
                        trynext=EHS_TRUE;
                        retrytimes=EHS_DEVMAN_CORE_RETRY_TIMES_FIRSTURL;
                        if (retrytimes < 3 ) retrytimes =3; /* Don't allow no retries (or too few) as this can mean no tries on our number one done atall */
                    }
                    else trynext=EHS_FALSE; /* Don't retry yet */
                }
                if ( EHS_TRUE == trynext)   /* if we're on the list or the first on the list has been tried too many times */
                {
                    CurrentURLindex++; /* Read the next URL info file - if we were on hard-wired this will switch to 0 - top of the list*/
                    if (!EhsHDevmanGetURL(szUrl, EHS_DEVMAN_COREURLS, EHS_MAXDEVMANNAMELEN, CurrentURLindex))
                    {
                        CurrentURLindex=-1; /* No NEXT URL found - reset counter and go to hardwired*/
                        EHSH_LOG_ERROR("No response from any config URLs  [%s], trying default URL,  [%s]",szUrl,EHS_DEVMAN_DEFAULTBASEURL);
                        EhsStrcpy(szUrl, EHS_DEVMAN_DEFAULTBASEURL EHS_DEVMAN_MONITORURLPATHONLY); /* fall back to hardwired - not to DEVMANURL.000 */
                    }
                    else
                    {
                        EhsStrcat(szUrl, EHS_DEVMAN_MONITORURLPATHONLY); /* OK so add the path bit */
                    }
                }
            }
            if (status && returndata && EhsStrlen(returndata) > 3)
            {
                ParseDevmanMonitorXML(returndata); // do any actions
            }
            else
            {
                //Log an Error
            }
            EhsSleep(EHS_TIME_s(EHS_DEVMANMON_POLL_RATE_SECONDS));//EhsMetaData.nDevmanPeriod)); // sleep for 10s @need a parameter here
            EhsHURLreset_write_data_buffer(buffer_struct);
        } //end while
curl_init_error:
        EhsHURLfree_write_data_buffer(buffer_struct);
        buffer_struct = NULL;

        EhsHMem_permFree(PostString);
        PostString = NULL;
    } // end made the buffer OK.
    return NULL;
}
#endif // EHS_DEVMAN_MON_SUPPORT

//#undef EHS_NODEVMAN
void DevmanMon_init(void)
{
//#define EHS_DEBUG_DISABLE_DEVMON
#ifndef EHS_DEBUG_DISABLE_DEVMON

#if EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_MQTT
#ifndef EHS_MQTT_SUPPORT
    #error "Cannot use Devman MQTT mon without 'EHS_MQTT_SUPPORT' in the target config"
#endif
    // init devman mon OTA handler
    EhsOtaDevmanMonSupportInit(EhsOtaDevmanMonNext);
    // init devman mon mqtt handler
    EhsMqttDevmanMon_t* pEhsMqttDevmanMon = EhsMqttDevmanMonSupportInit();
    // set mqtt devman mon loop callback which gets called form matt clinet loop
    pEhsMqttDevmanMon->pMqttDevmanMonLoop = DevmanMonThread;

    #if EHS_MQTT_CLIENT_FB_THREAD
    // for targets which use function block thread for running a blocking mqtt clinet loop e.g. linux
    // we need to create a thread that runs mqtt client loop for devman mon
    pthread_t t1;
    if (pthread_create(&t1, NULL, EhsMqttClientLoop, (void*)EhsMqttDevmanMonSupport()))
    {
        EHSH_LOG_ERROR("Error creating Devman monitor thread");
    }
    #endif

    gMqttClientInstanceCount++;
    EHSH_LOG_INFO("Increase MQTT clinet instace count (%d) \n", gMqttClientInstanceCount);
    
#else // EHS_DEVMAN_MON_SUPPORT == EHS_DEVMAN_MON_CURL
    pthread_t t1;
    if (pthread_create(&t1, NULL, DevmanMonThread, NULL))
    {
        EHSH_LOG_ERROR("Error creating Devman monitor thread");
    }
#endif // EHS_DEVMAN_MON_SUPPORT

#endif // EHS_DEBUG_DISABLE_DEVMON
}
