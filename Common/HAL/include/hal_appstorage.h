
/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/*
 * @file hal_appstorage.h
 *
 * @author: inx limited
 *
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
