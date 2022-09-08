/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


/** @file hal_xml.c
 * Definitions for Common utilities for supporting xml access
 *
 * @author: inx limited
 *
 */

/*****************************************************************************/
/* Included files */
#include "hal_xml_minimal.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/*
 * This is designed for small XML chunks. the whole string is parsed searching for the first tag match.
 *  This version doesn't return an incremented source string pointer, like Ehs_ReadXMLTagAttrib does.
 */

ehs_char * Ehs_ReadXMLTag(ehs_char* XML, ehs_char * tag)
{
    char * attrib;
    return Ehs_ReadXMLTagAttrib(&attrib, XML, tag);
}


/*
 * This is designed for small XML chunks. the whole string is parsed searching for the first tag match
 * NOTE - checks chrs before and after match with tag, are angled brackets, to ensure we are matching against whole tag name, not just part of it
 *
 * Returns pointer after end of tag that matches the tag parameter or null if no matching tag in the xml passed in
 *
 * The pAttrib parameter is a return parameter, pointing to the first attribute for the tag that matches the tag parameter
 * pAttrib is null if there are no attributes for the tag
 *
 */
ehs_char * Ehs_ReadXMLTagAttrib(ehs_char** pAttrib, ehs_char* XML, ehs_char * tag)
{
    *pAttrib = NULL;
    if (XML == NULL || tag == NULL || EhsStrlen(tag) < 1 ) return NULL;
    ehs_char * temp = EhsStrstr(XML, tag);
    if (temp == NULL ) return NULL;
    ehs_char * endOfTag;

    // find first chr after tag but before angle bracket <tag attrib_1="x" attrib_2="y" attrib_n="z">
    // this is the pointer to the first attrib, returns null if no attributes
    ehs_bool bIsWhiteSpace = EHS_FALSE;

    //eat white space
    if (temp && temp > XML )   // read to the end of the <tag> - skipping white space too. temp must point > 1 char into XML.
    {
        endOfTag = temp + strlen(tag);
        temp--;


        // check chr before and after match to ensure we are matching against whole tag name, not just part of it
        if (((temp[0] == '<') || (temp[0] == ' ')) && (endOfTag[0] == '>' || endOfTag[0] == ' '))
        {
            while ((temp[0] != '>') && (temp[0] != '\0'))
            {
                temp++;

                if (temp[0] == ' ')
                {
                    bIsWhiteSpace = EHS_TRUE;
                }
                // if found some white space followed by non white space chr, this must be the first attrib
                if (*pAttrib == NULL && bIsWhiteSpace && temp[0] != ' ')
                {
                    *pAttrib = temp;
                }

                /*
                 while ((temp[0] != ' ') && (temp[0] != '\t') && (temp[0] != '\r') && (temp[0] != '\n')&& (temp[0] != '\0') ) {
                 temp++;
                 */
            }
            if (temp[0] == '>')
                temp++; // skip the angle bracket

            return temp;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

/**
 * Retrieves attribute value for the attribute name passed in
 * @param result - a return param with the value of the attribute
 * @param XML - the bit of xml that starts with the attribute list for tag being parsed
 * @param name - the name of the attribute whose value is to be retrieved
 *
 * Note that XML is a string that starts at the attributes for the current tag, but may also contain XML for other tags
 * so first thing to do is isolate the attributes for the current tag and discard the rest of the XML
 *
 * @returns bool - true if retrieved a value for the attribute name passed in
 */
ehs_bool Ehs_ReadXMLAttribValue(ehs_char* result, ehs_char* XML, ehs_char * name)
{
    ehs_bool bFound = EHS_FALSE;
    int len = 0;

    //@todo - is there a better way - we want to only search for attribute name in xml, up to the close of angle bracket, which is the end of tag
    ehs_char * cEnd = XML;
    while ((cEnd[0] != '>') && (cEnd[0] != '\0'))
    {
        cEnd++;
        len++;
    }
    if (len > 0)
    {
        ehs_char * tempXML = EhsHMem_tempAlloc(len + 1);
        EhsStrncpy(tempXML, XML, len);
        tempXML[len] = '\0';


        ehs_char * temp = EhsStrstr(tempXML, name);
        // if attrib is found in the xml, return its value where attrib is expected to be of the form 'name=value'
        if (temp)
        {
            while ((temp[0] != '=') && (temp[0] != '\0'))
            {
                temp++;
            }
            if (temp[0] == '=')
            {
                temp++; // want str after '='
                EhsSscanf(temp,"%s",result);
                bFound = EHS_TRUE;
            }
        }
        EhsHMem_tempFree(tempXML);
    }
    return bFound;
}

/* CHeap XML parser for devman */
ehs_char * Ehs_CopyXMLTagElement(ehs_char * element, ehs_char* XML, ehs_uint32 max,
                                 ehs_bool trim_end_too)
{
    ehs_uint32 countdown = max;
    if (XML == NULL || element == NULL ) return NULL;
    //read off white space

    /*
     * we should be after the tag angle bracker here, so need to find it here
     *
     while (XML && (*XML != '\0') && (*XML != '>')) { // read to the end of the <tag>
     XML++;
     }
     */
    while (XML && (*XML != '\0') && ((*XML == ' ') || (*XML == '\t') || (*XML
                                     == '\n') || (*XML == '\r')))   // read oof any white space
    {
        XML++;
    }
    while (countdown && XML && (*XML != '<') && (*XML != '\0'))   // copy till we get to the end tag
    {
        if (trim_end_too)
        {
            if ((*XML == '\r') || (*XML == '\n') || (*XML == '\t'))
                break;
        }
        *element = *XML;
        element++;
        XML++;
        max--;
    }
    //element++; // get to next
    *element = 0;//terminate
    return XML; //return the end pointer in XML in case it's useful'
}
