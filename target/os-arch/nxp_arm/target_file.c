#include "heatrod_config.h"
#include "target_types.h"
#include "targetos_init.h"

#define INX_EHS_NXP_TSDL_POINT 1
#define INX_EHS_NXP_TSDL_NULL 2
ehs_bool EhsTF_exists(const ehs_char* fname) {
	if(strcmp(fname,"/appdata")==0){
		return EHS_TRUE;
	}else{
		
	}
    return EHS_FALSE;
}

ehs_bool EhsTF_mkdir(const char * szParameterFilePath) {
	return EHS_FALSE;
}

ehs_bool EhsTF_rmdir(ehs_char* szPath) {
	return EHS_FALSE;
}

ehs_bool GetDevmanBASEURL(ehs_char * szUrl) {
    return EHS_FALSE;
}

EHS_GLOBAL ehs_bool EhsTOsSys_UpdateEnvironment(EhsMetaDataType * pEhsMetaData, ehs_uint8 what){
    return EHS_FALSE;
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTOsApp_term(void)
{
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTOsApp_reset(void)
{
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTOsSys_term(void)
{
	// Leave the mutexes to the OS EhsTPMutex_term();
}

/**
 * Perform necessary Operating system setup upon system initialisation
 */
void EhsTOsSys_init(void)
{
	EhsTPMutex_init();
}

void EhsBinSearchPath(void )
{
}

/**
 * Perform setup before loading in a new application
 */
void EhsTOsApp_init(void)
{
//	#ifdef EHS_GUI_SUPPORT //@todo and this (as above)
//            EhsGtkKeypressCallback = NULL;
//        #endif
}

/**
 * Cleanup - close the current directory
 * @param[in] pFiles Context for the file operation
 */
void EhsTDFiles_cleanup(EhsTDFilesClass* pFiles)
{
	
}

/**
 * Change the directory we will use for file listing operations
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir absolute Directory path
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass* pFiles, const ehs_char* szDir) {
	ehs_bool bRet = EHS_FALSE; /* assume this fails */
	return bRet;
}

/**
 * Start retrieving files from the current directory
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller must pass in an array
 * of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass* pFiles, ehs_char* szName)
{
	return EHS_FALSE;
}

ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass* pFiles, ehs_char* szName)
{
    return EHS_FALSE;
}

void EhsTDFiles_init(struct EhsTDFilesStruct** pFiles) {}

/*
we're going to use a linked list for storing files, similar to the app variables
in fact it may even be the same linked list as the app variables

the linked list starts at our earlist sector for storing data, this will be the sector
after our program has ended (or maybe some abritrary number like 100)

on start of new file clear the linked list
add each file as a new entry to the list, entry is
char* filename, use strlen and null terminator to figure out the length
uint32_t sizeBytes, the size of the file
uint8_t* data, the actual file data

we know the list has ended when we hit a zero length filename and file size.

when adding a file we need to make sure we aren't going to run off the end of our available space


when writing a file we will have to fill our firmware buffer, probably reuse all the code we did for writing t.sdl



so question 1, when do we clear the linked list? 
it's crude but the first open with 'w' or 'wb' after an open with a 'r'.
so open with an 'r' sets "clearNextWrite"
open with a 'w' clears the flag and does the emptying of the linked list

how does adding a new file work? let's look at our variables system
given we are only ever reading or writing one file at a time we only need one variable to store that
we know the head of the files list as it is the address of the first flash block we use for file storing


*/
static char inxEHSFilesCurrentFilename[256]={0};
static inxEHSFile_t inxEHSFilesCurrentFile={0};
static ehs_uint16 inxEHSFilesBytesInBuffer=0;
static ehs_uint8 inxClearFilesFlag=0;
static ehs_uint8 inxEHSFilesWriteOnCloseFlag=0;
static ehs_uint32 inxEHSFilesCurrentFlashAddress=0;
static inxEHSFile_t inxEHSFilesFirstFile(){
	inxEHSFile_t firstFile={0};
	firstFile.filename=(char*)inxEHSPermanentStorageAddress();
	firstFile.sizeBytes=*(uint16_t*)(firstFile.filename+strlen(firstFile.filename)+1);
	return firstFile;
}

static uint32_t inxEHSFilesLastByteOf(inxEHSFile_t* pFile){
	uint32_t address=(uint32_t)pFile->filename;
	address=address+strlen(pFile->filename)+sizeof(pFile->sizeBytes)+pFile->sizeBytes+1; //we add an extra 1 for the null terminator
	return address;
}

static uint32_t inxEHSFilesAddressToSectorStart(const uint32_t address){
	uint32_t distanceInToFlash=address-inxEHSPermanentStorageAddress();
	uint8_t numSectors=distanceInToFlash/CONFIG_FLASH_SECTOR_SIZE_BYTES;
	return (numSectors*CONFIG_FLASH_SECTOR_SIZE_BYTES)+inxEHSPermanentStorageAddress();
}

static uint32_t inxEHSFilesWriteBufferToFlash(const uint32_t address){
	uint8_t* pBuffer=(uint8_t*)inxSODLGetWritePointer();
	//need to convert address to the nearest sector start
	//on the assumption pBuffer already contains the data of that sector
	const uint32_t sectorStartAddress=inxEHSFilesAddressToSectorStart(address);
	writeToFlash(pBuffer,CONFIG_FLASH_SECTOR_SIZE_BYTES,0,true,sectorStartAddress);
	return sectorStartAddress+CONFIG_FLASH_SECTOR_SIZE_BYTES; //return address of the next sector
}

static void inxEHSFilesWriteEmptyToFirstSector(){
	uint32_t firstAddress=inxEHSPermanentStorageAddress();
	uint8_t* pBuffer=(uint8_t*)inxSODLGetWritePointer();
	memset(pBuffer,0,CONFIG_FLASH_SECTOR_SIZE_BYTES);
	inxEHSFilesWriteBufferToFlash(firstAddress);
}

void inxClearFiles(){
	inxClearFilesFlag=0;
	//to clear the list we have to write that first flash sector to null
	inxEHSFilesWriteEmptyToFirstSector();
	inxEHSFilesCurrentFile=inxEHSFilesFirstFile();
}

static inxEHSFile_t inxEHSFilesNext(const inxEHSFile_t* pCurrentFile){
	inxEHSFile_t pNext={0};
	uint32_t address=(uint32_t)pCurrentFile->filename;
	address=address+strlen(pCurrentFile->filename)+1+sizeof(pCurrentFile->sizeBytes)+pCurrentFile->sizeBytes+1; //we add two 1s, first 1 for the null terminator on the file name, second 1 for the null terminator on the data
	pNext.filename=(char*)address;
	//now we have the filename look for the size in bytes
	address=(uint32_t)pNext.filename;
	address=address+strlen(pNext.filename)+1;
	ehs_uint16* pNextSizeBytes=(ehs_uint16*)address;
	pNext.sizeBytes=*pNextSizeBytes;
	return pNext;
}

static inxEHSFile_t inxEHSFilesLast(){
	inxEHSFile_t currentFile=inxEHSFilesFirstFile();
	inxEHSFile_t nextFile={0};
	while(true){
		nextFile=inxEHSFilesNext(&currentFile);
		if(strlen(nextFile.filename)>0){
			currentFile=nextFile;
		}else{
			break;
		}
	}
	return currentFile;
}

static uint32_t inxEHSFilesAddressOfNextFile(){
	inxEHSFile_t lastFile=inxEHSFilesLast();
	uint32_t addressOfNextFile=(uint32_t)lastFile.filename;
	if(lastFile.sizeBytes>0){
		addressOfNextFile=inxEHSFilesLastByteOf(&lastFile)+1;
	}
	return addressOfNextFile;
}

static ehs_uint8 inxEHSFileWillFit(inxEHSFile_t file,ehs_uint16 sizeBytes){
	ehs_uint8 willFit=1;
	uint32_t lastAddress=inxEHSFilesAddressOfNextFile()+strlen(file.filename)+1+sizeof(file.sizeBytes)+sizeBytes;
	uint32_t sectorsUsed=(lastAddress-inxEHSPermanentStorageAddress())/CONFIG_FLASH_SECTOR_SIZE_BYTES;
	if(file.sizeBytes>0){
		sectorsUsed=((inxEHSFilesCurrentFlashAddress+sizeBytes)-inxEHSPermanentStorageAddress())/CONFIG_FLASH_SECTOR_SIZE_BYTES;
	}
	if(sectorsUsed>CONFIG_EHS_MAX_SODL_SECTORS){
		willFit=0;
	}
	return willFit;
}

inxEHSFile_t inxEHSFilesFind(const char* fname){
	inxEHSFile_t file={0};
	//start at the head
	inxEHSFile_t currentFile=inxEHSFilesFirstFile();
	while(strlen(currentFile.filename)!=0){
		//check filename
		if(strcmp(currentFile.filename,fname)==0){
			//if match return that
			file=currentFile;
			break;
		}else{
			//if not try next file
			currentFile=inxEHSFilesNext(&currentFile);
		}
	}
	return file;
}


static ehs_uint32 inxEHSFilesFP=0;
//buffer that we store tsdl in, also gets written to flash and loaded from flash
ehs_FILE* EhsFopen(const char* path,const char* mode){
	char* fname=strrchr(path,'/');
	if(fname==NULL){
		fname=(char*)path;
	}else{
		fname=fname+1; //skip the leading /
	}
	ehs_FILE* pFile=NULL;
	if(*mode=='r'){
		inxEHSFilesFP=0;
		inxClearFilesFlag=1;
		//we are reading a file so iterate our list to get an address for it
		inxEHSFilesCurrentFile=inxEHSFilesFind(fname);
		if(strlen(inxEHSFilesCurrentFile.filename)>0){
			pFile=(ehs_FILE*)&inxEHSFilesCurrentFile;
		}
	}else{
		if(inxClearFilesFlag){
			inxClearFiles();
		}
		strncpy(inxEHSFilesCurrentFilename,fname,sizeof(inxEHSFilesCurrentFilename));
		inxEHSFilesCurrentFile.filename=inxEHSFilesCurrentFilename;
		inxEHSFilesCurrentFile.sizeBytes=0;
		pFile=(ehs_FILE*)&inxEHSFilesCurrentFile;
		inxEHSFilesWriteOnCloseFlag=1;
	}
	return pFile;
	/*
	printf("PBB 110 %s %s\n",fname,mode);
	if(strcmp(fname,"/appdata/default/t.sdl")==0 || strcmp(fname,"/appdata/temp/t.sdl")==0){
		tsdlfp=0;
		if(*mode=='r'){
			const char* data=(const char*)inxSODLGetSectorAddress(0);
			sdlLength=strlen(data);
			printf("PBB sdlLength=%d\n",sdlLength);
		}
		return (ehs_FILE*)INX_EHS_NXP_TSDL_POINT;
	}else if(strcmp(fname,"/appdata/temp/g0000000.gui")==0 || strcmp(fname,"/appdata/temp/nls.csv")==0){
		return (ehs_FILE*)INX_EHS_NXP_TSDL_NULL;
	}else{
		return NULL;
	}*/
}

uint8_t* inxEHSFilesGetData(inxEHSFile_t file){
	uint32_t address=(uint32_t)file.filename;
	address=address+strlen(file.filename)+1+sizeof(file.sizeBytes);
	return (uint8_t*)address;
}

static uint8_t* inxEHSFilesCurrentFileData(){
	return inxEHSFilesGetData(inxEHSFilesCurrentFile);
}

ehs_sint16 EhsFgetc(ehs_FILE* stream){
	ehs_sint16 c=EHS_EOF;
	if(inxEHSFilesFP<inxEHSFilesCurrentFile.sizeBytes){
		const uint8_t* data=inxEHSFilesCurrentFileData();	
		c=data[inxEHSFilesFP];
		inxEHSFilesFP=inxEHSFilesFP+1;
	}
	/*if(stream==(ehs_FILE*)INX_EHS_NXP_TSDL_POINT){
		const char* data=(const char*)inxSODLGetSectorAddress(0);
		if(tsdlfp<sdlLength){
			c=data[tsdlfp];
			tsdlfp=tsdlfp+1;
		}
	}*/
	//printf("PBB 337 %d %c\n",inxEHSFilesFP,c);
	return c;
}

EHS_GLOBAL ehs_char* EhsFgets(ehs_char* dst,ehs_uint16 max,ehs_FILE* fp){
	ehs_sint16 c;
	ehs_char* p;

	/* get max bytes or upto a newline */

	for (p = dst, max--; max > 0; max--) {
		if ((c = EhsFgetc (fp)) == EHS_EOF)
			break;
		*p++ = c;
		if (c == '\n')
			break;
	}
	*p = 0;
	if (p == dst)
		return (0);
	return (p);
}

static uint8_t* inxEHSFilesPrepareBuffer(const uint32_t address,uint16_t* bytesInToBuffer){
	uint8_t* pBuffer=(uint8_t*)inxSODLGetWritePointer();
	//copy the contents of that sector in to buffer
	const uint32_t sectorAddress=inxEHSFilesAddressToSectorStart(address);
	memcpy(pBuffer,(uint8_t*)sectorAddress,CONFIG_FLASH_SECTOR_SIZE_BYTES);
	*bytesInToBuffer=address-sectorAddress;
	return pBuffer;
}

static void inxEHSFilesWriteData(ehs_uint8* pBufferToFill,ehs_uint16* bytesInToBuffer,ehs_uint8* ptr8,ehs_uint16 totalBytes);
static void inxEHSFilesWriteHeaderForLastFile(){
	//add final null terminator
	uint8_t terminator=0;
	printf("PBB 372 %s %d\n",inxEHSFilesCurrentFilename,inxEHSFilesBytesInBuffer);
	inxEHSFilesWriteData((uint8_t*)inxSODLGetWritePointer(),&inxEHSFilesBytesInBuffer,&terminator,1);
	printf("PBB 374 %d\n",inxEHSFilesBytesInBuffer);
	//that is the current flash address - size bytes - length of name - null terminator
	uint32_t fileSizeAddressInFlash=inxEHSFilesCurrentFlashAddress-inxEHSFilesCurrentFile.sizeBytes-sizeof(inxEHSFilesCurrentFile.sizeBytes)-1; //we add 1 for the null terminator
	uint32_t sectorAddress=inxEHSFilesAddressToSectorStart(fileSizeAddressInFlash);
	if(sectorAddress==inxEHSFilesAddressToSectorStart(inxEHSFilesCurrentFlashAddress)){
		if(inxEHSFilesBytesInBuffer>0 && inxEHSFilesCurrentFile.sizeBytes>0){
			//we can just modify current buffer
			uint8_t* pBuffer=(uint8_t*)inxSODLGetWritePointer();
			uint16_t bytesInToBuffer=inxEHSFilesBytesInBuffer-inxEHSFilesCurrentFile.sizeBytes-sizeof(inxEHSFilesCurrentFile.sizeBytes)-1; //we add one for the null terminator
			printf("PBB 383 %d %d %d %d %d\n",inxEHSFilesBytesInBuffer,inxEHSFilesCurrentFile.sizeBytes,strlen((char*)pBuffer)+1,bytesInToBuffer,inxEHSFilesCurrentFile.sizeBytes);
			memcpy(&pBuffer[bytesInToBuffer],&inxEHSFilesCurrentFile.sizeBytes,sizeof(inxEHSFilesCurrentFile.sizeBytes));
			inxEHSFilesWriteBufferToFlash(inxEHSFilesCurrentFlashAddress);
			inxEHSFilesBytesInBuffer=0;
			printf("PBB 388 %x %x\n",fileSizeAddressInFlash,inxEHSFilesCurrentFlashAddress);
		}
	}else{
		if(inxEHSFilesBytesInBuffer>0){
			inxEHSFilesWriteBufferToFlash(inxEHSFilesCurrentFlashAddress);
			inxEHSFilesBytesInBuffer=0;
		}
		//we need to write current buffer then write another buffer
		//update the sizeBytes with our current sizebytes
		uint16_t bytesInToBuffer=0;
		uint8_t* pBuffer=inxEHSFilesPrepareBuffer(fileSizeAddressInFlash,&bytesInToBuffer);
		memcpy(&pBuffer[bytesInToBuffer],&inxEHSFilesCurrentFile.sizeBytes,sizeof(inxEHSFilesCurrentFile.sizeBytes));
		printf("PBB 401 %d %d\n",bytesInToBuffer,inxEHSFilesCurrentFile.sizeBytes);
		inxEHSFilesWriteBufferToFlash(sectorAddress);
	}
}

ehs_sint16 EhsFclose(ehs_FILE* stream){
	if(inxEHSFilesWriteOnCloseFlag){
		inxEHSFilesWriteOnCloseFlag=0;
		inxEHSFilesWriteHeaderForLastFile();
	}
	return 0;
}

//how do we support more than 4kb of SODL?
//we get rid of our heap buffer for SODL writing and use the firmware buffer
//on an Fwrite detect if we are going to fill our buffer
//if we are then flag the network thread to write our buffer to flash and wait for it
//once that has finished clear our buffer and go back to the start again


//this method attempts to get the next sector of flash to be written to
//it then sets a bunch of pointers for where to copy data to so that the next write goes correctly
/*static void inxEHSFilesGetWriteBuffer(ehs_uint32* pFlashAddress,ehs_uint8** ppBufferStart,ehs_uint8** ppBufferCurrent,ehs_uint16* pBufferSize){
	// *pBufferSize is the size of the firmware buffer
	*pBufferSize=CONFIG_FLASH_SECTOR_SIZE_BYTES;
	//how do we find the write address?
	//if *pFlashAddress==0 then we need to search for the end of the last file
	if(*pFlashAddress==0){
		*pFlashAddress=inxEHSFilesAddressOfNextFile();
	}
	//otherwise we just use the gap between *pFlashAddress and the end of that sector
	// *ppBufferStart is always the firmware buffer
	*ppBufferStart=(uint8_t*)inxSODLGetWritePointer();
	uint32_t firstAddress=inxEHSPermanentStorageAddress();
	uint32_t distanceInToFlash=*pFlashAddress-firstAddress;
	uint8_t* pSectorStart=(uint8_t*)inxEHSFilesAddressToSectorStart(*pFlashAddress);
	memcpy(*ppBufferStart,pSectorStart,*pBufferSize);
	inxEHSFilesBytesInBuffer=distanceInToFlash%*pBufferSize;
	*ppBufferCurrent=*ppBufferStart+inxEHSFilesBytesInBuffer;
	//our buffer now has the contents of our flash sector, memset everything after *ppBufferCurrent to 0
	size_t remainingBuffer=*pBufferSize-(*ppBufferCurrent-*ppBufferStart);
	memset(*ppBufferCurrent,0,remainingBuffer);
}
*/
/*
what is the exact process for writing a file

find sector for the last file
check the file will fit
copy sector in to firmware buffer
set buffer size to remaining space in the sector
while dataLeftToWrite
	fill buffer from starting point (up to sector size)
	when buffer full write the current sector
*/

static void inxEHSFilesWriteData(ehs_uint8* pBufferToFill,ehs_uint16* bytesInToBuffer,ehs_uint8* ptr8,ehs_uint16 totalBytes){
	//we're going to add some data to our buffer
	//if the buffer is full or the force flag is true then we write to flash and empty the buffer
	//otherwise just append to the buffer

	//can we change the GetWriteBuffer to return the current in memory buffer if needed?
	ehs_uint16 spaceInBufferBytes=CONFIG_FLASH_SECTOR_SIZE_BYTES-*bytesInToBuffer;
	ehs_uint16 bytesWritten=0;
	ehs_uint16 numberOfBytesToWrite=totalBytes-bytesWritten;
	if(numberOfBytesToWrite>spaceInBufferBytes){
		numberOfBytesToWrite=spaceInBufferBytes;
	}
	while(totalBytes>bytesWritten){
		memcpy(&pBufferToFill[*bytesInToBuffer],&ptr8[bytesWritten],numberOfBytesToWrite);
		*bytesInToBuffer=*bytesInToBuffer+numberOfBytesToWrite;
		if(*bytesInToBuffer==CONFIG_FLASH_SECTOR_SIZE_BYTES){
			inxEHSFilesWriteBufferToFlash(inxEHSFilesCurrentFlashAddress);
			//clear buffer
			*bytesInToBuffer=0;
			memset(pBufferToFill,0,CONFIG_FLASH_SECTOR_SIZE_BYTES);
		}
		inxEHSFilesCurrentFlashAddress=inxEHSFilesCurrentFlashAddress+numberOfBytesToWrite;
		bytesWritten=bytesWritten+numberOfBytesToWrite;
		spaceInBufferBytes=CONFIG_FLASH_SECTOR_SIZE_BYTES-*bytesInToBuffer;
		numberOfBytesToWrite=totalBytes-bytesWritten;
		if(numberOfBytesToWrite>spaceInBufferBytes){
			numberOfBytesToWrite=spaceInBufferBytes;
		}
	}
}

ehs_sint16 EhsFwrite(void *ptr,ehs_uint16 size,ehs_uint16 count,ehs_FILE* fp){
	ehs_uint16 returnCount = 0;
	ehs_uint16 totalBytes=size*count;
	//we will get multiple writes for a file, so if the file being written is our current file
	//then we should be appending to it rather than overwriting it
	if(inxEHSFileWillFit(inxEHSFilesCurrentFile,totalBytes)){
		if(inxEHSFilesCurrentFile.sizeBytes==0){
			//new file
			//get a buffer to the start of the next file prefilled with data
			inxEHSFilesCurrentFlashAddress=inxEHSFilesAddressOfNextFile();
			uint8_t* pBuffer=inxEHSFilesPrepareBuffer(inxEHSFilesCurrentFlashAddress,&inxEHSFilesBytesInBuffer);
			printf("PBB 499 address to start file %x bytesInToBuffer=%d\n",inxEHSFilesCurrentFlashAddress,inxEHSFilesBytesInBuffer);
			//clear the rest of it
			memset(&pBuffer[inxEHSFilesBytesInBuffer],0,CONFIG_FLASH_SECTOR_SIZE_BYTES-inxEHSFilesBytesInBuffer);
			inxEHSFilesWriteData(pBuffer,&inxEHSFilesBytesInBuffer,(uint8_t*)inxEHSFilesCurrentFilename,strlen(inxEHSFilesCurrentFilename)+1);
			inxEHSFilesWriteData(pBuffer,&inxEHSFilesBytesInBuffer,(uint8_t*)&inxEHSFilesCurrentFile.sizeBytes,sizeof(inxEHSFilesCurrentFile.sizeBytes));
		}else{
			//append
			//continue filling current buffer
		}
		//we only actually want to write data if our buffer is full or fclose has been called
		//flashAddress starts off as 0, the API will then set it to the first available address
		//if it gets a non zero address then it will just start from there
		inxEHSFilesWriteData((uint8_t*)inxSODLGetWritePointer(),&inxEHSFilesBytesInBuffer,ptr,totalBytes);
		returnCount=count;
		inxEHSFilesCurrentFile.sizeBytes+=totalBytes;
	}
	/*
	if(fp==(ehs_FILE*)INX_EHS_NXP_TSDL_POINT){
		inxTSDL* pSDL=inxSODLGetWritePointer();
		ehs_uint8* ptr8=(ehs_uint8*)ptr;
		ehs_uint16 spaceInBufferBytes=sizeof(pSDL->fileData)-tsdlfp;
		if(tsdlfp==0){
			memset(pSDL->fileData,0,sizeof(pSDL->fileData));
		}
		if(totalBytes<=spaceInBufferBytes){
			memcpy(&pSDL->fileData[tsdlfp],ptr8,totalBytes);
			tsdlfp=tsdlfp+totalBytes;
			returnCount=totalBytes;
		}else{
			//copy what we can in to this buffer
			memcpy(&pSDL->fileData[tsdlfp],ptr8,spaceInBufferBytes);
			totalBytes=totalBytes-spaceInBufferBytes;
			ptr8=ptr8+spaceInBufferBytes;
			tsdlfp=0; //so we are ready to write to next buffer
			returnCount=returnCount+spaceInBufferBytes;
			//write the SODL since we assume this being called from the network thread
			inxWriteSODLBuffer();
			memset(pSDL->fileData,0,sizeof(pSDL->fileData));
			//then write the rest in to the next buffer
			//make sure what is left fits in our buffer
			if(totalBytes>sizeof(pSDL->fileData)){
				totalBytes=sizeof(pSDL->fileData);
			}
			memcpy(&pSDL->fileData[tsdlfp],ptr8,totalBytes);
			tsdlfp=tsdlfp+totalBytes;
			returnCount=returnCount+totalBytes;
		}
	}else{
		returnCount=count;
	}*/
	return returnCount;
}

ehs_bool EhsTF_cd(const ehs_char * dir){
	return EHS_FALSE;
}