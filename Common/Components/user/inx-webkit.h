/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef INXWARE_html_java
#define INXWARE_html_java
#include "inx-component.h"

/*****************************************************
 * load plain JavaScript or HTML with Javascript and run Java functions
 *****************************************************/
#define INXWARE_FB_NAME_html_java "html_java"
#define INXWARE_FB_ID_html_java 0x3C43

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(html_java); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(html_java); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(html_java); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(html_java_LoadScript); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(html_java_RunFunction); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(html_java_UnLoadAll); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(html_java_setInvisible); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(html_java_setVisibile); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(html_java)

#endif /* INXWARE_html_java */
