/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#define __DEBUG__
// #define __USE_FATFS__
#define __USE_LITTLEFS__

//#include "target_types.h"
#include "globals.h"
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
#ifdef EHS_RUNTIME_LOGGER_ENABLED
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
    if (fname == NULL) return 0; // Check NULL pointer
    if (fname[0] == '\0') return 0; // Check the string is empty
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
