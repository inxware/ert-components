/*
 * hal_appstorage.h
 *
 *  Created on: 4 Apr 2019
 *      Author: pdrezet
 */

#ifndef COMMON_HAL_INCLUDE_HAL_APPSTORAGE_H_
#define COMMON_HAL_INCLUDE_HAL_APPSTORAGE_H_


/* Public filesystem specifications for application management */
/* @info the 3 directories *_dl, *, *_prev for each app allow for safe downloading and fall-back */
#define EHS_SYS_APP_BASE_DIR "appdata"
#define EHS_SYS_APP_DEFAULT_NAME "default"
#define EHS_SYS_APP_TEMP_NAME "temp"
#define EHS_SYS_APP_DOWNLOAD_OK_TOKEN "dl_ok_flag.ehs"
#define EHS_SYS_APP2RUN_PREVIOUS_FILENAME_POSTFIX "_prev"
#define EHS_SYS_APP2RUN_DOWNLOAD_FILENAME_POSTFIX "_dl"
#define EHS_SYS_APP2RUN_PERSISTENT_FILENAME_PREFIX "persist_"
#define EHS_SYS_APP_INFO_FILENAME "info.xml"
#define EHS_SYS_APP_INFO_XMLTAG_NAME "Name"
#define EHS_SYS_APP_INFO_XMLTAG_COMM_NAME "CommercialName"
#define EHS_SYS_APP_INFO_XMLTAG_VERSION "Version"
#define EHS_SYS_APP_INFO_XMLTAG_DESCRIPTION "Description"



#endif /* COMMON_HAL_INCLUDE_HAL_APPSTORAGE_H_ */
