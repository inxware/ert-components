/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#define __DEBUG__
// #define __USE_FATFS__
#define __USE_LITTLEFS__

#include "target_types.h"
#include "target_file.h"
#include "targetos_init.h"

#ifndef EHS_USE_SIMPLE_FILESYSTEM
#include <sys/stat.h>
#include <errno.h>
#endif

#ifndef __USE_FATFS__
#define __USE_FATFS__
#endif
#ifdef __USE_LITTLEFS__
#undef __USE_FATFS__
#endif
#ifdef __USE_FATFS__
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#endif

#ifdef __USE_LITTLEFS__
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "esp_idf_version.h"
#include "esp_flash.h"
#include "esp_littlefs.h"
#endif

#ifdef __USE_FATFS__
// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
#endif
// Mount path for the partition
const char *base_path = "/ehs";
#ifdef __DEBUG__
#include "esp_log.h"
#else
#define ESP_LOGI(...)
#define ESP_LOGW(...)
#define ESP_LOGE(...)
#define ESP_LOGD(...)
#endif
#define TAG "target_file"

#define INX_EHS_NXP_TSDL_POINT 1
#define INX_EHS_NXP_TSDL_NULL 2
/*
##################################################################
THis file has a conditional switch to toggle between a full esp32
file system and the inxware simplified one (as used on more bare
targets).
##################################################################
*/

#ifdef EHS_USE_SIMPLE_FILESYSTEM
#error sfsdfds
ehs_bool EhsTF_exists(const ehs_char *fname)
{
    if (strcmp(fname, "/appdata") == 0)
    {
        return EHS_TRUE;
    }
    else
    {
    }
    return EHS_FALSE;
}

ehs_bool EhsTF_mkdir(const char *szParameterFilePath)
{
    return EHS_FALSE;
}

ehs_bool EhsTF_rmdir(ehs_char *szPath)
{
    return EHS_FALSE;
}

/*
we're going to use a linked list for storing files, similar to the app variables
in fact it may even be the same linked list as the app variables

the linked list starts at our earlist sector for storing data, this will be the
sector after our program has ended (or maybe some abritrary number like 100)

on start of new file clear the linked list
add each file as a new entry to the list, entry is
char* filename, use strlen and null terminator to figure out the length
uint32_t sizeBytes, the size of the file
uint8_t* data, the actual file data

we know the list has ended when we hit a zero length filename and file size.

when adding a file we need to make sure we aren't going to run off the end of
our available space

when writing a file we will have to fill our firmware buffer, probably reuse all
the code we did for writing t.sdl

so question 1, when do we clear the linked list?
it's crude but the first open with 'w' or 'wb' after an open with a 'r'.
so open with an 'r' sets "clearNextWrite"
open with a 'w' clears the flag and does the emptying of the linked list

how does adding a new file work? let's look at our variables system
given we are only ever reading or writing one file at a time we only need one
variable to store that we know the head of the files list as it is the address
of the first flash block we use for file storing


*/
static char inxEHSFilesCurrentFilename[256] = {0};
static inxEHSFile_t inxEHSFilesCurrentFile = {0};
static ehs_uint16 inxEHSFilesBytesInBuffer = 0;
static ehs_uint8 inxClearFilesFlag = 0;
static ehs_uint8 inxEHSFilesWriteOnCloseFlag = 0;
static ehs_uint32 inxEHSFilesCurrentFlashAddress = 0;
static inxEHSFile_t inxEHSFilesFirstFile()
{
    inxEHSFile_t firstFile = {0};
    firstFile.filename = (char *)inxEHSPermanentStorageAddress();
    firstFile.sizeBytes =
        *(uint16_t *)(firstFile.filename + strlen(firstFile.filename) + 1);
    return firstFile;
}

static uint32_t inxEHSFilesLastByteOf(inxEHSFile_t *pFile)
{
    uint32_t address = (uint32_t)pFile->filename;
    address = address + strlen(pFile->filename) + sizeof(pFile->sizeBytes) +
              pFile->sizeBytes + 1; // we add an extra 1 for the null terminator
    return address;
}

static uint32_t inxEHSFilesAddressToSectorStart(const uint32_t address)
{
    uint32_t distanceInToFlash = address - inxEHSPermanentStorageAddress();
    uint8_t numSectors = distanceInToFlash / CONFIG_FLASH_SECTOR_SIZE_BYTES;
    return (numSectors * CONFIG_FLASH_SECTOR_SIZE_BYTES) +
           inxEHSPermanentStorageAddress();
}

static uint32_t inxEHSFilesWriteBufferToFlash(const uint32_t address)
{
    uint8_t *pBuffer = (uint8_t *)inxSODLGetWritePointer();
    // need to convert address to the nearest sector start
    // on the assumption pBuffer already contains the data of that sector
    const uint32_t sectorStartAddress = inxEHSFilesAddressToSectorStart(address);
    writeToFlash(pBuffer, CONFIG_FLASH_SECTOR_SIZE_BYTES, 0, EHS_TRUE,
                 sectorStartAddress);
    return sectorStartAddress +
           CONFIG_FLASH_SECTOR_SIZE_BYTES; // return address of the next sector
}

static void inxEHSFilesWriteEmptyToFirstSector()
{
    uint32_t firstAddress = inxEHSPermanentStorageAddress();
    uint8_t *pBuffer = (uint8_t *)inxSODLGetWritePointer();
    memset(pBuffer, 0, CONFIG_FLASH_SECTOR_SIZE_BYTES);
    inxEHSFilesWriteBufferToFlash(firstAddress);
}

void inxClearFiles()
{
    inxClearFilesFlag = 0;
    // to clear the list we have to write that first flash sector to null
    inxEHSFilesWriteEmptyToFirstSector();
    inxEHSFilesCurrentFile = inxEHSFilesFirstFile();
}

static inxEHSFile_t inxEHSFilesNext(const inxEHSFile_t *pCurrentFile)
{
    inxEHSFile_t pNext = {0};
    uint32_t address = (uint32_t)pCurrentFile->filename;
    address = address + strlen(pCurrentFile->filename) + 1 +
              sizeof(pCurrentFile->sizeBytes) + pCurrentFile->sizeBytes +
              1; // we add two 1s, first 1 for the null terminator on the file
    // name, second 1 for the null terminator on the data
    pNext.filename = (char *)address;
    // now we have the filename look for the size in bytes
    address = (uint32_t)pNext.filename;
    address = address + strlen(pNext.filename) + 1;
    ehs_uint16 *pNextSizeBytes = (ehs_uint16 *)address;
    pNext.sizeBytes = *pNextSizeBytes;
    return pNext;
}

static inxEHSFile_t inxEHSFilesLast()
{
    inxEHSFile_t currentFile = inxEHSFilesFirstFile();
    inxEHSFile_t nextFile = {0};
    while (EHS_TRUE)
    {
        nextFile = inxEHSFilesNext(&currentFile);
        if (strlen(nextFile.filename) > 0)
        {
            currentFile = nextFile;
        }
        else
        {
            break;
        }
    }
    return currentFile;
}

static uint32_t inxEHSFilesAddressOfNextFile()
{
    inxEHSFile_t lastFile = inxEHSFilesLast();
    uint32_t addressOfNextFile = (uint32_t)lastFile.filename;
    if (lastFile.sizeBytes > 0)
    {
        addressOfNextFile = inxEHSFilesLastByteOf(&lastFile) + 1;
    }
    return addressOfNextFile;
}

static ehs_uint8 inxEHSFileWillFit(inxEHSFile_t file, ehs_uint16 sizeBytes)
{
    ehs_uint8 willFit = 1;
    uint32_t lastAddress = inxEHSFilesAddressOfNextFile() +
                           strlen(file.filename) + 1 + sizeof(file.sizeBytes) +
                           sizeBytes;
    uint32_t sectorsUsed = (lastAddress - inxEHSPermanentStorageAddress()) /
                           CONFIG_FLASH_SECTOR_SIZE_BYTES;
    if (file.sizeBytes > 0)
    {
        sectorsUsed = ((inxEHSFilesCurrentFlashAddress + sizeBytes) -
                       inxEHSPermanentStorageAddress()) /
                      CONFIG_FLASH_SECTOR_SIZE_BYTES;
    }
    if (sectorsUsed > CONFIG_EHS_MAX_SODL_SECTORS)
    {
        willFit = 0;
    }
    return willFit;
}

inxEHSFile_t inxEHSFilesFind(const char *fname)
{
    inxEHSFile_t file = {0};
    // start at the head
    inxEHSFile_t currentFile = inxEHSFilesFirstFile();
    while (strlen(currentFile.filename) != 0)
    {
        // check filename
        if (strcmp(currentFile.filename, fname) == 0)
        {
            // if match return that
            file = currentFile;
            break;
        }
        else
        {
            // if not try next file
            currentFile = inxEHSFilesNext(&currentFile);
        }
    }
    return file;
}

static ehs_uint32 inxEHSFilesFP = 0;
// buffer that we store tsdl in, also gets written to flash and loaded from
// flash
ehs_FILE *EhsFopen(const char *path, const char *mode)
{
    char *fname = strrchr(path, '/');
    if (fname == NULL)
    {
        fname = (char *)path;
    }
    else
    {
        fname = fname + 1; // skip the leading /
    }
    ehs_FILE *pFile = NULL;
    if (*mode == 'r')
    {
        inxEHSFilesFP = 0;
        inxClearFilesFlag = 1;
        // we are reading a file so iterate our list to get an address for it
        inxEHSFilesCurrentFile = inxEHSFilesFind(fname);
        if (strlen(inxEHSFilesCurrentFile.filename) > 0)
        {
            pFile = (ehs_FILE *)&inxEHSFilesCurrentFile;
        }
    }
    else
    {
        if (inxClearFilesFlag)
        {
            inxClearFiles();
        }
        strncpy(inxEHSFilesCurrentFilename, fname,
                sizeof(inxEHSFilesCurrentFilename));
        inxEHSFilesCurrentFile.filename = inxEHSFilesCurrentFilename;
        inxEHSFilesCurrentFile.sizeBytes = 0;
        pFile = (ehs_FILE *)&inxEHSFilesCurrentFile;
        inxEHSFilesWriteOnCloseFlag = 1;
    }
    return pFile;
    /*

    if(strcmp(fname,"/appdata/default/t.sdl")==0 ||
    strcmp(fname,"/appdata/temp/t.sdl")==0){ tsdlfp=0; if(*mode=='r'){ const char*
    data=(const char*)inxSODLGetSectorAddress(0); sdlLength=strlen(data);
            }
            return (ehs_FILE*)INX_EHS_NXP_TSDL_POINT;
    }else if(strcmp(fname,"/appdata/temp/g0000000.gui")==0 ||
    strcmp(fname,"/appdata/temp/nls.csv")==0){ return
    (ehs_FILE*)INX_EHS_NXP_TSDL_NULL; }else{ return NULL;
    }*/
}

uint8_t *inxEHSFilesGetData(inxEHSFile_t file)
{
    uint32_t address = (uint32_t)file.filename;
    address = address + strlen(file.filename) + 1 + sizeof(file.sizeBytes);
    return (uint8_t *)address;
}

static uint8_t *inxEHSFilesCurrentFileData()
{
    return inxEHSFilesGetData(inxEHSFilesCurrentFile);
}

ehs_sint16 EhsFgetc(ehs_FILE *stream)
{
    ehs_sint16 c = EHS_EOF;
    if (inxEHSFilesFP < inxEHSFilesCurrentFile.sizeBytes)
    {
        const uint8_t *data = inxEHSFilesCurrentFileData();
        c = data[inxEHSFilesFP];
        inxEHSFilesFP = inxEHSFilesFP + 1;
    }
    /*if(stream==(ehs_FILE*)INX_EHS_NXP_TSDL_POINT){
            const char* data=(const char*)inxSODLGetSectorAddress(0);
            if(tsdlfp<sdlLength){
                    c=data[tsdlfp];
                    tsdlfp=tsdlfp+1;
            }
    }*/
    return c;
}

EHS_GLOBAL ehs_char *EhsFgets(ehs_char *dst, ehs_uint16 max, ehs_FILE *fp)
{
    ehs_sint16 c;
    ehs_char *p;

    /* get max bytes or upto a newline */

    for (p = dst, max--; max > 0; max--)
    {
        if ((c = EhsFgetc(fp)) == EHS_EOF)
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

static uint8_t *inxEHSFilesPrepareBuffer(const uint32_t address,
        uint16_t *bytesInToBuffer)
{
    uint8_t *pBuffer = (uint8_t *)inxSODLGetWritePointer();
    // copy the contents of that sector in to buffer
    const uint32_t sectorAddress = inxEHSFilesAddressToSectorStart(address);
    memcpy(pBuffer, (uint8_t *)sectorAddress, CONFIG_FLASH_SECTOR_SIZE_BYTES);
    *bytesInToBuffer = address - sectorAddress;
    return pBuffer;
}

static void inxEHSFilesWriteData(ehs_uint8 *pBufferToFill,
                                 ehs_uint16 *bytesInToBuffer, ehs_uint8 *ptr8,
                                 ehs_uint16 totalBytes);
static void inxEHSFilesWriteHeaderForLastFile()
{
    // add final null terminator
    uint8_t terminator = 0;

    inxEHSFilesWriteData((uint8_t *)inxSODLGetWritePointer(),
                         &inxEHSFilesBytesInBuffer, &terminator, 1);

    // that is the current flash address - size bytes - length of name - null
    // terminator
    uint32_t fileSizeAddressInFlash = inxEHSFilesCurrentFlashAddress -
                                      inxEHSFilesCurrentFile.sizeBytes -
                                      sizeof(inxEHSFilesCurrentFile.sizeBytes) -
                                      1; // we add 1 for the null terminator
    uint32_t sectorAddress =
        inxEHSFilesAddressToSectorStart(fileSizeAddressInFlash);
    if (sectorAddress ==
            inxEHSFilesAddressToSectorStart(inxEHSFilesCurrentFlashAddress))
    {
        if (inxEHSFilesBytesInBuffer > 0 && inxEHSFilesCurrentFile.sizeBytes > 0)
        {
            // we can just modify current buffer
            uint8_t *pBuffer = (uint8_t *)inxSODLGetWritePointer();
            uint16_t bytesInToBuffer = inxEHSFilesBytesInBuffer -
                                       inxEHSFilesCurrentFile.sizeBytes -
                                       sizeof(inxEHSFilesCurrentFile.sizeBytes) -
                                       1; // we add one for the null terminator

            memcpy(&pBuffer[bytesInToBuffer], &inxEHSFilesCurrentFile.sizeBytes,
                   sizeof(inxEHSFilesCurrentFile.sizeBytes));
            inxEHSFilesWriteBufferToFlash(inxEHSFilesCurrentFlashAddress);
            inxEHSFilesBytesInBuffer = 0;
        }
    }
    else
    {
        if (inxEHSFilesBytesInBuffer > 0)
        {
            inxEHSFilesWriteBufferToFlash(inxEHSFilesCurrentFlashAddress);
            inxEHSFilesBytesInBuffer = 0;
        }
        // we need to write current buffer then write another buffer
        // update the sizeBytes with our current sizebytes
        uint16_t bytesInToBuffer = 0;
        uint8_t *pBuffer =
            inxEHSFilesPrepareBuffer(fileSizeAddressInFlash, &bytesInToBuffer);
        memcpy(&pBuffer[bytesInToBuffer], &inxEHSFilesCurrentFile.sizeBytes,
               sizeof(inxEHSFilesCurrentFile.sizeBytes));
        inxEHSFilesWriteBufferToFlash(sectorAddress);
    }
}

ehs_sint16 EhsFclose(ehs_FILE *stream)
{
    if (inxEHSFilesWriteOnCloseFlag)
    {
        inxEHSFilesWriteOnCloseFlag = 0;
        inxEHSFilesWriteHeaderForLastFile();
    }
    return 0;
}

// how do we support more than 4kb of SODL?
// we get rid of our heap buffer for SODL writing and use the firmware buffer
// on an Fwrite detect if we are going to fill our buffer
// if we are then flag the network thread to write our buffer to flash and wait
// for it once that has finished clear our buffer and go back to the start again

// this method attempts to get the next sector of flash to be written to
// it then sets a bunch of pointers for where to copy data to so that the next
// write goes correctly
/*static void inxEHSFilesGetWriteBuffer(ehs_uint32* pFlashAddress,ehs_uint8**
ppBufferStart,ehs_uint8** ppBufferCurrent,ehs_uint16* pBufferSize){
        // *pBufferSize is the size of the firmware buffer
        *pBufferSize=CONFIG_FLASH_SECTOR_SIZE_BYTES;
        //how do we find the write address?
        //if *pFlashAddress==0 then we need to search for the end of the last
file if(*pFlashAddress==0){ *pFlashAddress=inxEHSFilesAddressOfNextFile();
        }
        //otherwise we just use the gap between *pFlashAddress and the end of
that sector
        // *ppBufferStart is always the firmware buffer
        *ppBufferStart=(uint8_t*)inxSODLGetWritePointer();
        uint32_t firstAddress=inxEHSPermanentStorageAddress();
        uint32_t distanceInToFlash=*pFlashAddress-firstAddress;
        uint8_t*
pSectorStart=(uint8_t*)inxEHSFilesAddressToSectorStart(*pFlashAddress);
        memcpy(*ppBufferStart,pSectorStart,*pBufferSize);
        inxEHSFilesBytesInBuffer=distanceInToFlash%*pBufferSize;
        *ppBufferCurrent=*ppBufferStart+inxEHSFilesBytesInBuffer;
        //our buffer now has the contents of our flash sector, memset everything
after *ppBufferCurrent to 0 size_t
remainingBuffer=*pBufferSize-(*ppBufferCurrent-*ppBufferStart);
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

static void inxEHSFilesWriteData(ehs_uint8 *pBufferToFill,
                                 ehs_uint16 *bytesInToBuffer, ehs_uint8 *ptr8,
                                 ehs_uint16 totalBytes)
{
    // we're going to add some data to our buffer
    // if the buffer is full or the force flag is true then we write to flash and
    // empty the buffer otherwise just append to the buffer

    // can we change the GetWriteBuffer to return the current in memory buffer if
    // needed?
    ehs_uint16 spaceInBufferBytes =
        CONFIG_FLASH_SECTOR_SIZE_BYTES - *bytesInToBuffer;
    ehs_uint16 bytesWritten = 0;
    ehs_uint16 numberOfBytesToWrite = totalBytes - bytesWritten;
    if (numberOfBytesToWrite > spaceInBufferBytes)
    {
        numberOfBytesToWrite = spaceInBufferBytes;
    }
    while (totalBytes > bytesWritten)
    {
        memcpy(&pBufferToFill[*bytesInToBuffer], &ptr8[bytesWritten],
               numberOfBytesToWrite);
        *bytesInToBuffer = *bytesInToBuffer + numberOfBytesToWrite;
        if (*bytesInToBuffer == CONFIG_FLASH_SECTOR_SIZE_BYTES)
        {
            inxEHSFilesWriteBufferToFlash(inxEHSFilesCurrentFlashAddress);
            // clear buffer
            *bytesInToBuffer = 0;
            memset(pBufferToFill, 0, CONFIG_FLASH_SECTOR_SIZE_BYTES);
        }
        inxEHSFilesCurrentFlashAddress =
            inxEHSFilesCurrentFlashAddress + numberOfBytesToWrite;
        bytesWritten = bytesWritten + numberOfBytesToWrite;
        spaceInBufferBytes = CONFIG_FLASH_SECTOR_SIZE_BYTES - *bytesInToBuffer;
        numberOfBytesToWrite = totalBytes - bytesWritten;
        if (numberOfBytesToWrite > spaceInBufferBytes)
        {
            numberOfBytesToWrite = spaceInBufferBytes;
        }
    }
}

EHS_GLOBAL ehs_sint16 EhsFwrite(void *ptr, ehs_uint16 size, ehs_uint16 count,
                                ehs_FILE *fp)
{
    ehs_uint16 returnCount = 0;
    ehs_uint16 totalBytes = size * count;
    // we will get multiple writes for a file, so if the file being written is our
    // current file then we should be appending to it rather than overwriting it
    if (inxEHSFileWillFit(inxEHSFilesCurrentFile, totalBytes))
    {
        if (inxEHSFilesCurrentFile.sizeBytes == 0)
        {
            // new file
            // get a buffer to the start of the next file prefilled with data
            inxEHSFilesCurrentFlashAddress = inxEHSFilesAddressOfNextFile();
            uint8_t *pBuffer = inxEHSFilesPrepareBuffer(
                                   inxEHSFilesCurrentFlashAddress, &inxEHSFilesBytesInBuffer);

            // clear the rest of it
            memset(&pBuffer[inxEHSFilesBytesInBuffer], 0,
                   CONFIG_FLASH_SECTOR_SIZE_BYTES - inxEHSFilesBytesInBuffer);
            inxEHSFilesWriteData(pBuffer, &inxEHSFilesBytesInBuffer,
                                 (uint8_t *)inxEHSFilesCurrentFilename,
                                 strlen(inxEHSFilesCurrentFilename) + 1);
            inxEHSFilesWriteData(pBuffer, &inxEHSFilesBytesInBuffer,
                                 (uint8_t *)&inxEHSFilesCurrentFile.sizeBytes,
                                 sizeof(inxEHSFilesCurrentFile.sizeBytes));
        }
        else
        {
            // append
            // continue filling current buffer
        }
        // we only actually want to write data if our buffer is full or fclose has
        // been called flashAddress starts off as 0, the API will then set it to the
        // first available address if it gets a non zero address then it will just
        // start from there
        inxEHSFilesWriteData((uint8_t *)inxSODLGetWritePointer(),
                             &inxEHSFilesBytesInBuffer, ptr, totalBytes);
        returnCount = count;
        inxEHSFilesCurrentFile.sizeBytes += totalBytes;
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
                    //write the SODL since we assume this being called from the
    network thread inxWriteSODLBuffer();
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

ehs_bool EhsTF_cd(const ehs_char *dir)
{
    return EHS_FALSE;
}
/* ####################################################################### */
#else  // if not simple file system

ehs_bool EhsTF_cd(const ehs_char * dir)
{
    return (chdir(dir)==0);
}


ehs_bool EhsTF_mkdir(const char * szParameterFilePath)
{
    return (mkdir(szParameterFilePath,S_IRWXU) == 0);
}

#ifdef INX_GET_THIS_WORING
static int remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d)
    {
        struct dirent *p;

        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_directory(buf);
                    }
                    else
                    {remove_directory
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r)
    {
        r = rmdir(path);
    }

    return r;
}
#else
static int remove_directory(const char *path) {
    return  0;
}
#endif


ehs_bool EhsTF_rmdir(ehs_char* szPath)
{
    ehs_sint16 ret= 0;
    if (EhsTF_exists(szPath) >= 1)   //@todo this will remove a file also - is this what we want?
    {
        ret=remove_directory(szPath);
        if(ret < 0)
        {
            ret=remove(szPath);
        }
        EHSH_LOG_INFO("EhsTF_rmdir ret=%d",ret);
    }
    else
    {
        ret=-1;
    }
    EHSH_LOG_INFO("EhsTF_rmdir ret=%d szPath=%s",ret,szPath);
    return (ret == 0);
}

ehs_uint8 EhsTF_exists(const ehs_char* fname)
{
    struct stat xFileInfo;
    ehs_uint8 nRet = 0;

    const ehs_char *fname_temp = fname;
    int stat_ret=stat(fname_temp,&xFileInfo);
    if (( stat_ret == -1) && (errno == ENOENT))
    {
        nRet = 0;
    }
    else
    {
        if ( stat_ret != -1)   // only do the following if stat() as successful (because ENOENT is not reliable on all platforms
        {
           // if (S_ISREG(xFileInfo.st_mode))
           // {
                nRet=1;
           // }
           // else if (S_ISDIR(xFileInfo.st_mode))
           // {
                nRet=2;
           // }
        }
        else
        {
            nRet = 0;
        }
    }
    return nRet;
}




#endif // endif not simple file system

void EhsTDFiles_cleanup(EhsTDFilesClass *pFiles) {}

/**
 * Change the directory we will use for file listing operations
 * @param[in] pFiles Context for the file operation
 * @param[in] szDir absolute Directory path
 * @return true if successful.
 */
ehs_bool EhsTDFiles_setDir(EhsTDFilesClass *pFiles, const ehs_char *szDir)
{
    ehs_bool bRet = EHS_FALSE; /* assume this fails */
    return bRet;
}

/**
 * Start retrieving files from the current directory
 * @param[in] pFiles Context for the file operation
 * @param[out] szName Text containing the name of the retrieved object. Caller
 * must pass in an array of length EHS_TD_FILES_MAX_FILENAME
 * @return true if a file name has been obtained
 */
ehs_uint8 EhsTDFiles_listFirst(EhsTDFilesClass *pFiles, ehs_char *szName)
{
    return EHS_FALSE;
}

ehs_uint8 EhsTDFiles_listNext(EhsTDFilesClass *pFiles, ehs_char *szName)
{
    return EHS_FALSE;
}

void EhsTDFiles_init(struct EhsTDFilesStruct **pFiles) 
{
    
}

#ifdef __USE_FATFS__
static void esp_get_fatfs_usage(size_t* out_total_bytes, size_t* out_free_bytes);
ehs_bool EhsTgtFilesystem_Init(void)
{
    EHSH_LOG_INFO("Mounting FAT filesystem");
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    // To mount device we need name of device partition, define base_path
    // and allow format partition in case if it is new one and was not formated before
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 4,
            .format_if_mount_failed = true,
            .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        EHSH_LOG_ERROR("Failed to mount FATFS (%s)", esp_err_to_name(err));
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return EHS_FALSE;
    }

    EhsHMetaSetInstPath(base_path);
    ehs_char szCanonicalFilePath[EHS_MAXPATHLENGTH];
    EhsTF_tryCanonicPath(szCanonicalFilePath, EHS_RUNTIME_SYSDATA_DIR,"fcheck.txt", EHS_TRUE);
    Ehs_MakePath(szCanonicalFilePath, EHS_TRUE);
    FILE *fp = fopen(szCanonicalFilePath, "wb");
    if (fp == NULL)
    {
        EHSH_LOG_ERROR("Failed to open file for writing, file is %s", szCanonicalFilePath);
        ESP_LOGE(TAG, "Failed to open file for writing, file is %s", szCanonicalFilePath);
        return EHS_FALSE;
    }
    fprintf(fp, "Written using ESP-IDF %s\n", esp_get_idf_version());
    fclose(fp);
    if (EhsTF_exists(szCanonicalFilePath) == 0)
    {
        err = esp_vfs_fat_spiflash_unmount(base_path, s_wl_handle);
        if (err != ESP_OK) {
            EHSH_LOG_ERROR("Failed to unmount FATFS (%s)", esp_err_to_name(err));
            ESP_LOGE(TAG, "Failed to unmount FATFS (%s)", esp_err_to_name(err));
        }
        const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "storage");
        ESP_ERROR_CHECK(esp_partition_erase_range(partition, 0, partition->size));
        s_wl_handle = WL_INVALID_HANDLE;
        err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);
        if (err != ESP_OK) {
            EHSH_LOG_ERROR("Failed to mount FATFS (%s)", esp_err_to_name(err));
            ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
            return EHS_FALSE;
        }
    }
    else
    {
        if (remove(szCanonicalFilePath) != 0)
        {
            EHSH_LOG_ERROR("Failed to delete the file");
            ESP_LOGE(TAG, "Failed to delete the file");
            return EHS_FALSE;
        }
    }
    size_t bytes_total, bytes_free;
    esp_get_fatfs_usage(&bytes_total, &bytes_free);
    printf("\nFAT FS: %d kB total, %d kB free\n", bytes_total / 1024, bytes_free / 1024);
    ESP_LOGI(TAG, "FAT FS: %d kB total, %d kB free", bytes_total / 1024, bytes_free / 1024);
    return EHS_TRUE;
}


static void esp_get_fatfs_usage(size_t* out_total_bytes, size_t* out_free_bytes)
{
    FATFS *fs;
    size_t free_clusters;
    int res = f_getfree("0:", &free_clusters, &fs);
    assert(res == FR_OK);
    size_t total_sectors = (fs->n_fatent - 2) * fs->csize;
    size_t free_sectors = free_clusters * fs->csize;

    // assuming the total size is < 4GiB, should be true for SPI Flash
    if (out_total_bytes != NULL) {
        *out_total_bytes = total_sectors * fs->ssize;
    }
    if (out_free_bytes != NULL) {
        *out_free_bytes = free_sectors * fs->ssize;
    }
}
#endif

#ifdef __USE_LITTLEFS__
ehs_bool EhsTgtFilesystem_Init(void)
{
    EHSH_LOG_INFO("Initializing LittleFS");
    ESP_LOGI(TAG, "Initializing LittleFS");

    esp_vfs_littlefs_conf_t conf = {
        .base_path = base_path,
        .partition_label = "storage",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    // Use settings defined above to initialize and mount LittleFS filesystem.
    // Note: esp_vfs_littlefs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret != ESP_OK)
    {
            if (ret == ESP_FAIL)
            {
                    EHSH_LOG_ERROR("Failed to mount or format filesystem");
                    ESP_LOGE(TAG, "Failed to mount or format filesystem");
            }
            else if (ret == ESP_ERR_NOT_FOUND)
            {
                    EHSH_LOG_ERROR("Failed to find LittleFS partition");
                    ESP_LOGE(TAG, "Failed to find LittleFS partition");
            }
            else
            {
                    EHSH_LOG_ERROR("Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
                    ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
            }
            return EHS_FALSE;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
            EHSH_LOG_ERROR("Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
            ESP_LOGE(TAG, "Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
            EHSH_LOG_INFO(TAG, "Partition size: total: %d, used: %d", total, used);
            ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    EhsHMetaSetInstPath(base_path);

    return EHS_TRUE;
}
#endif
