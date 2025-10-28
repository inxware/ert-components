/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-xml_stream.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include <expat.h>

#define printf(...)
//#define printf LOGI

typedef struct inx_xml_stream_parser_chunk_struct
{
    ehs_char chunk[EHS_STRING_LENGTH_MAX];
    struct inx_xml_stream_parser_chunk_struct* pNext;
} inx_xml_stream_parser_chunk_type;

typedef struct inx_xml_stream_parser_script_eater_struct
{
    ehs_uint32 state; //parsing state
    ehs_char *position; //position in the character data
    ehs_char *data; //character data to parse
    ehs_uint32 scriptpos; //tells us where in </script> we are
    ehs_char *startpos; //tells us where the start of the </script> is when we find one
    inx_xml_stream_parser_chunk_type *headchunk;
    ehs_char *firstchunkpos; //tells us where in state->input the xml parser finished
    inx_xml_stream_parser_chunk_type *startchunk; //the chunk containing the start of our </script>
    inx_xml_stream_parser_chunk_type *currentchunk; //the chunk containing the start of our </script>
} inx_xml_stream_parser_script_eater_type;

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    const char **attrs;
    ehs_bool cdata;
    const XML_Char *cdataString;
    int cdataLength;
    ehs_bool eatScriptContent;
    XML_Parser hand;
    ehs_bool ignoreScript;
    EhsFunctionInstanceDataType* pReadFid;
    EhsFunctionInstanceDataType* pParseFid;
    ehs_bool pausedForEvent;
    inx_xml_stream_parser_chunk_type* input;
    ehs_bool ignore_script;
    inx_xml_stream_parser_script_eater_type scriptEater;
    ehs_char ampersandReplace;
    ehs_char* ignore_entities;
    ehs_bool waitingForInput;
    ehs_bool startTag;
    const char* endTagName;
    ehs_bool streamFinished;
    //FILE* received;
    //FILE* parsed;
} inx_xml_stream_parser_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(xml_stream_parser)

EHS_FB_FUNCTION_ENTRY("eos", 0x01, xml_stream_parser_eos)

EHS_FB_FUNCTION_ENTRY("read", 0x02, xml_stream_parser_read)

EHS_FB_FUNCTION_ENTRY("parse", 0x03, xml_stream_parser_parse)

EHS_FB_FUNCTION_ENTRY("streamFinished", 0x04, xml_stream_parser_streamFinished)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_xml_stream_parser_ARG_eos_reset_finish 1
#define INX_xml_stream_parser_ARG_read_data 1
#define INX_xml_stream_parser_ARG_read_ignore_entities 2
#define INX_xml_stream_parser_ARG_read_data_out 1
#define INX_xml_stream_parser_ARG_read_read_finish 1
#define INX_xml_stream_parser_ARG_parse_ignore_script 1
#define INX_xml_stream_parser_ARG_parse_name 1
#define INX_xml_stream_parser_ARG_parse_cdata 2
#define INX_xml_stream_parser_ARG_parse_attr_name 3
#define INX_xml_stream_parser_ARG_parse_attr_value 4
#define INX_xml_stream_parser_ARG_parse_processing_target 5
#define INX_xml_stream_parser_ARG_parse_processing_instruction 6
#define INX_xml_stream_parser_ARG_parse_comment 7
#define INX_xml_stream_parser_ARG_parse_start_element 1
#define INX_xml_stream_parser_ARG_parse_finish_element 2
#define INX_xml_stream_parser_ARG_parse_next_finish 3
#define INX_xml_stream_parser_ARG_parse_character_data 4
#define INX_xml_stream_parser_ARG_parse_attribute 5
#define INX_xml_stream_parser_ARG_parse_error 6
#define INX_xml_stream_parser_ARG_parse_processing_finish 7
#define INX_xml_stream_parser_ARG_parse_comment_finish 8
#define INX_xml_stream_parser_ARG_parse_cdata_start_finish 9
#define INX_xml_stream_parser_ARG_parse_cdata_end_finish 10
#define INX_xml_stream_parser_ARG_parse_finished 11
#define INX_xml_stream_parser_ARG_streamFinished_streamFinishedFinish 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_xml_stream_parser_ignore_script 0
#define INX_FB_xml_stream_parser_ignore_entities &
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(xml_stream_parser)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    	ehs_bool ignore_script;
    	ehs_char* ignore_entities;
    	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %s",&ignore_script,&ignore_entities); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_xml_stream_parser_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

//int badstate=0;


EHS_FB_INIT_FUNCTION(xml_stream_parser)
{
    ehs_bool ignore_script;
    ehs_char ignore_entities[EHS_STRING_LENGTH_MAX];
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_xml_stream_parser_state_type* inx_xml_stream_parser_state = (inx_xml_stream_parser_state_type*)EHS_FB_INIT_CONTEXT;
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%hhd %s",&ignore_script,ignore_entities);// TODO:This probably isn't needed - shouldu  use teh * in the fomrat to skip?
    //now convert that to a bool
    inx_xml_stream_parser_state->attrs=NULL;
    inx_xml_stream_parser_state->cdata=EHS_FALSE;
    inx_xml_stream_parser_state->cdataLength=0;
    inx_xml_stream_parser_state->cdataString=NULL;
    inx_xml_stream_parser_state->eatScriptContent=EHS_FALSE;
    inx_xml_stream_parser_state->hand=NULL;
    inx_xml_stream_parser_state->ignoreScript=ignore_script;
    inx_xml_stream_parser_state->input=NULL;
    inx_xml_stream_parser_state->pReadFid=NULL;
    inx_xml_stream_parser_state->pausedForEvent=EHS_TRUE;
    inx_xml_stream_parser_state->pParseFid=NULL;
    inx_xml_stream_parser_state->scriptEater.state=0;
    //inx_xml_stream_parser_state->ampersandReplace=ignore_entities[0];
    inx_xml_stream_parser_state->ampersandReplace='$'; //TODO read parameters correctly
    inx_xml_stream_parser_state->waitingForInput=EHS_FALSE;
    inx_xml_stream_parser_state->startTag=EHS_FALSE;
    inx_xml_stream_parser_state->endTagName=NULL;
    inx_xml_stream_parser_state->endTagName=EHS_FALSE;
    /*inx_xml_stream_parser_state->received=fopen("/home/pdrezet/read.xml","w");
    	inx_xml_stream_parser_state->parsed=fopen("/home/pdrezet/parsedxml","w");*/
    /* read the initialisation parameters */
    /*
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %s",&ignore_script,&ignore_entities);
    */
    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
static void inx_xml_stream_parser_replace_ampersands(ehs_char replaceChar,ehs_char* string,ehs_sint32 length)
{
    if(replaceChar=='\0'||replaceChar=='&')
    {
        //no valid replace char so do nothing
        return;
    }
    ehs_sint32 i=0;
    for(i=0; i<length; i++)
    {
        if(string[i]==replaceChar)
        {
            string[i]='&';
        }
    }
}

static void inx_xml_stream_parser_cleanup(inx_xml_stream_parser_state_type* state)
{
    // Your code here
    XML_Parser hand=state->hand;
    inx_xml_stream_parser_chunk_type* current;
    inx_xml_stream_parser_chunk_type* parent;
    if(hand!=NULL)
    {
        XML_ParserFree(hand);
        state->hand=NULL;
    }
    //free up the linked list
    current=state->input;
    if(current!=NULL)
    {
        while(current->pNext!=NULL)
        {
            parent=current;
            current=current->pNext;
            EhsHMem_tempFree(parent);
            parent=NULL;
        }
    }
    state->input=NULL;
    state->pausedForEvent=EHS_FALSE;
    state->waitingForInput=EHS_FALSE;
    state->streamFinished=EHS_FALSE;
}

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(xml_stream_parser)
{
    inx_xml_stream_parser_state_type *inx_xml_stream_parser_state = (inx_xml_stream_parser_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    inx_xml_stream_parser_cleanup(inx_xml_stream_parser_state);
    /*fclose(inx_xml_stream_parser_state->received);
    fclose(inx_xml_stream_parser_state->parsed);*/
    return EHS_FALSE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

/* normal data handler */

static void XMLCALL
inx_xml_stream_parser_character_data(void *data,const XML_Char *s,int len)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    XML_StopParser(state->hand,EHS_TRUE);
    //have we seen a cdata start?
    if(state->cdata==EHS_TRUE)
    {
        //don't fire our own port because we have already fired the cdata start
        //just keep a pointer to the data and return
        state->cdata=EHS_FALSE;
        state->cdataString=s;
        state->cdataLength=len;
        //cdata fired so we shouldn't fire our character data port
        return;
    }
    //do we already have some data to from the last call?
    if(state->cdataString!=NULL)
    {
        //copy out what we already have
        if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_cdata))
        {
            //clean the string before copying
            //todo 2023 - check the length is not larger than the ERT max string legnth
            strncpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_cdata),state->cdataString,state->cdataLength);
            inx_xml_stream_parser_replace_ampersands(state->ampersandReplace,EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_cdata),state->cdataLength);
        }
        EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_character_data);
        //hang on to the new character data
        state->cdataString=s;
        state->cdataLength=len;
        //wait till we are called again
        return;
    }
    //if we got here then we have no existing data and we are not in a cdata section so just output the string
    if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_cdata))
    {
        strncpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_cdata),s,len);
        inx_xml_stream_parser_replace_ampersands(state->ampersandReplace,EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_cdata),len);
    }
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_character_data);
}


/*
 * @brief call back function for start of tags
 *
 *
 */
static void XMLCALL
inx_xml_stream_parser_start(void *data, const char *el, const char **attr)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    XML_StopParser(state->hand,EHS_TRUE);
    //keep a handle to the attributes so we can dump them as events
    state->attrs=attr;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    //copy the name to the output

    //badstate=1;
    if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_name))
    {
        strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_name),el);
    }
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_start_element);
    state->startTag=EHS_TRUE; //set this because if it is a self closing tag there is no guarantee it will stop
}

/*
 * @brief call back function for start of tags
 *
 *
 */
static void XMLCALL
inx_xml_stream_parser_end(void *data, const char *el)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    XML_StopParser(state->hand,EHS_TRUE);
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    //try to detect if we are the end of a self closing tag
    if(state->startTag==EHS_TRUE)
    {
        //we are the end of a self closing tag, how scary
        //prevent our own event from firing but remember our name so we can be fired next time round
        state->endTagName=el;
    }
    else
    {
        //copy the name to the output
        if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_name))
        {
            strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_name),el);
        }
        EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_finish_element);
    }
}

/*
 * @brief callback function for end of tags
 */
static void XMLCALL
inx_xml_stream_parser_processing(void *data,const XML_Char *target,const XML_Char *instruction)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    XML_StopParser(state->hand,EHS_TRUE);
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    //copy the target to the output
    if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_processing_target))
    {
        strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_processing_target),target);
    }
    //copy the data
    if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_processing_instruction))
    {
        strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_processing_instruction),instruction);
    }
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_processing_finish);
}

/*
 * @brief callback function for start of comment
 */
static void XMLCALL
inx_xml_stream_parser_comment(void *data, const XML_Char *comment)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    XML_StopParser(state->hand,EHS_TRUE);
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    //copy the comment to the output
    if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_comment))
    {
        strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_comment),comment);
        inx_xml_stream_parser_replace_ampersands(state->ampersandReplace,EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_comment),EhsStrlen(comment));
    }
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_comment_finish);
}

/*
 * @brief callback function for start of cdata
 */

static void XMLCALL
inx_xml_stream_parser_cdata_start(void *data)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    XML_StopParser(state->hand,EHS_TRUE); //this doesn't work here so we set various flags to stop any further events happening
    state->cdata=EHS_TRUE; //tell the character data handling function that we shouldn't fire immediately
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_cdata_start_finish);
}

/*
 * @brief callback function for end of cdata
 */
static void XMLCALL
inx_xml_stream_parser_cdata_end(void *data)
{
    inx_xml_stream_parser_state_type *state=(inx_xml_stream_parser_state_type*)data;
    XML_StopParser(state->hand,EHS_TRUE);
    //clear out any recorded character data stuff
    state->cdata=EHS_FALSE;
    state->cdataString=NULL;
    state->cdataLength=0;
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid;
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_cdata_end_finish);
}

/*static void inx_xml_stream_parser_script_eater_clean_chunk(inx_xml_stream_parser_script_eater_type *eater){
	EhsStrcpy(eater->startchunk->chunk,eater->startpos);
}*/

//script eater functions
static void inx_xml_stream_parser_script_eater_parse(inx_xml_stream_parser_script_eater_type *eater)
{
    ehs_char c;
    ehs_char *script="</script>";
    while(1)
    {
        //make sure current position is inside the current data
        //eater->position=eater->data;
        //if state==0 then look for starting <
        if(eater->state==0)
        {
            //use strstr to find <
            eater->position=strstr(eater->position,"<");
            if(eater->position==NULL)
            {
                //can't find it
                return;
            }
            else
            {
                //change state to next state
                eater->state=1;
                eater->scriptpos=0;
                eater->startpos=eater->position; //keep a reference to the start so that our parser can tell expat to start from there
                eater->startchunk=eater->currentchunk;
            }
        }
        //if state==1 then compare next characters till we get a full match or any mis match
        if(eater->state==1)
        {
            //loop till end of string or match
            c=*eater->position;
            while(1)
            {
                if(c=='\0')
                {
                    //end of data so return and wait for more data
                    return;
                }
                //not the end of the data so does it match our current position in </script>
                if(script[eater->scriptpos]==c)
                {
                    //they match so move on to the next character
                    eater->scriptpos++;
                    eater->position++;
                    c=*eater->position;
                    //have we reached the end of </script>?
                    if(eater->scriptpos>=9)
                    {
                        //yep found a match so go to matched state
                        eater->state=2;
                        return;
                    }
                }
                else
                {
                    //they don't match so reset state
                    eater->state=0;
                    break;
                }
            }
        }
    }
}

static void inx_xml_stream_parser_script_eater_start_parse(inx_xml_stream_parser_state_type *state)
{
    inx_xml_stream_parser_script_eater_type *eater=&(state->scriptEater);
    ehs_char c;
    ehs_char *script="<script";
    while(1)
    {
        //if state==0 then look for starting <
        if(eater->state==0)
        {
            //use strstr to find <
            eater->position=strstr(eater->position,"<");
            if(eater->position==NULL)
            {
                //can't find it
                return;
            }
            else
            {
                //change state to next state
                eater->state=1;
                eater->scriptpos=0;
                eater->startpos=eater->position; //keep a reference to the start so that our parser can tell expat to start from there
                eater->startchunk=eater->currentchunk;
                eater->headchunk=eater->currentchunk; //this tells us where to go back to
                eater->firstchunkpos=eater->position;
            }
        }
        //if state==1 then compare next characters till we get a full match or any mis match
        if(eater->state==1)
        {
            //loop till end of string or match
            c=*eater->position;
            while(1)
            {
                if(c=='\0')
                {
                    //end of data so return and wait for more data
                    return;
                }
                //not the end of the data so does it match our current position in <script>
                if(script[eater->scriptpos]==c)
                {
                    //they match so move on to the next character
                    eater->scriptpos++;
                    eater->position++;
                    c=*eater->position;
                    //have we reached the end of <script>?
                    if(eater->scriptpos>=7)
                    {
                        //yep found a match so go to matched state
                        eater->state=2;
                        return;
                    }
                }
                else
                {
                    //they don't match so reset state
                    eater->state=0;
                    break;
                }
            }
        }
    }
}

static void inx_xml_stream_parser_eat_script(inx_xml_stream_parser_state_type* state)
{
    inx_xml_stream_parser_chunk_type* input=state->input;
    inx_xml_stream_parser_script_eater_type *eater=&(state->scriptEater);
    int i,status=0;
    inx_xml_stream_parser_chunk_type *pOld;
    //we are eating input data till we find </script>
    //printfc is thus:
    //look for </script> in each input
    //discard inputs if they don't contain it
    //when you find the </script> then join them together
    while ( input != NULL )
    {
        eater->data=input->chunk;
        eater->position=eater->data;
        eater->currentchunk=input;
        inx_xml_stream_parser_script_eater_parse(eater);
        status=eater->state;
        if(status==0)
        {
            //ran out of input so get more
            if(input->pNext!=NULL)
            {
                //there is a next chunk so use that
                pOld=input;
                input=pOld->pNext;
                //only free up the chunk if it isn't our starting chunk
                if(pOld!=state->input)
                {
                    //we should only be freeing stuff that is after our starting chunk
                    EhsHMem_tempFree(pOld);
                    pOld=NULL;
                    state->input->pNext=input;
                }
            }
            else
            {
                if(input!=state->input)
                {
                    //free current input as it is redundant
                    EhsHMem_tempFree(input);
                    state->input->pNext=NULL;
                }
                //no next chunk so just wait for more
                input=NULL;
                state->waitingForInput=EHS_TRUE;
            }
        }
        else if(status==1)
        {
            //get the next chunk but don't discard our previous chunks
            input=input->pNext;
        }
        else if(status==2)
        {
            //found a valid </script> tag
            //free up any inputs from the start chunk to the end chunk
            if(eater->startchunk!=eater->headchunk)
            {
                //we have to start from the beginning of the list so that we don't break the list
                input=state->input->pNext;
                while(input!=NULL&&input!=eater->currentchunk)
                {
                    state->input->pNext=input->pNext;
                    EhsHMem_tempFree(input);
                    input=state->input->pNext;
                }
            }
            //in the same chunk or different chunks?
            if(eater->currentchunk==eater->headchunk)
            {
                //same chunk then stitch current position back
                //EhsStrcpy(eater->firstchunkpos,eater->position);
                for(i=0; i<=EhsStrlen(eater->position); i++)
                {
                    eater->firstchunkpos[i]=eater->position[i];
                }
            }
            else
            {
                //copy trailing first chunk in to place
                eater->firstchunkpos[0]='\0';
                //parser should resume from just after the </script>
                //move the trailing content of </script> to the front of the end chunk
                //EhsStrcpy(eater->currentchunk->chunk,eater->position);
                for(i=0; i<=EhsStrlen(eater->position); i++)
                {
                    eater->currentchunk->chunk[i]=eater->position[i];
                }
                if(eater->headchunk->pNext!=eater->currentchunk)
                {
                }
                //now free from the after the start to our current chunk
                input=state->input->pNext;
                while(input!=NULL&&input!=eater->currentchunk)
                {
                    state->input->pNext=input->pNext;
                    EhsHMem_tempFree(input);
                    input=state->input->pNext;
                }
                //double check we did this right
            }
            state->eatScriptContent=EHS_FALSE;
            state->input=eater->headchunk;
            eater->state=0;
            input=state->input;
            break;
        }
        //if we ran out of input and were in status==1 then reset to status==0
        if(input==NULL)
        {
            state->waitingForInput=EHS_TRUE;
            if(status==1)
            {
                eater->state=0;
            }
        }
    }
}

static const char** inx_xml_stream_parser_handle_attributes(const char **attrs,EhsFunctionInstanceDataType* pFIdata,inx_xml_stream_parser_state_type* state)
{
    //start from where we left off with the attributes, pump one out then break
    const char *attr=NULL;
    attr=attrs[0];
    if(attr==NULL||*attr=='\0')
    {
        //no more attributes so move on
        attrs=NULL;
    }
    else
    {
        //output attribute then break
        //this string should be the attribute name, the next string should be the value
        //copy name to the output
        if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_attr_name))
        {
            strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_attr_name),attr);
        }
        //copy value to the output
        attrs++; //move to the next string in the list for the value
        attr=attrs[0];
        if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_attr_value))
        {
            strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_attr_value),attr);
            inx_xml_stream_parser_replace_ampersands(
                state->ampersandReplace,
                EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_attr_value),
                EhsStrlen(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_attr_value)));
        }
        attrs++;
        EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_attribute);
    }
    return attrs;
}

/* @brief Parses the current XML and produces events and presents data in single chunks
 * as the XML is parsed.
 */


static void inx_xml_stream_parser_parse(inx_xml_stream_parser_state_type* state)
{
    EhsFunctionInstanceDataType* pFIdata=state->pParseFid; //so we can fire parse ports
    XML_Parser hand=state->hand;
    inx_xml_stream_parser_chunk_type* input=state->input;
    inx_xml_stream_parser_chunk_type *pOld,*pInput=NULL;
    ehs_char* chunk=NULL;
    int ii=0; //@todo remove
    int size=0;
    int status=0;
    long parserPosition;
    int i;



    ehs_bool parsed=EHS_FALSE;
    ehs_bool shouldParse=EHS_TRUE;
    //test for input and handle
    if(input==NULL||hand==NULL||pFIdata==NULL)
    {
        return;
    }
    input=state->input;
    //check the ignore script port and parameter
    if (EHS_FB_IN_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_ignore_script))
    {
        state->ignoreScript=EHS_FB_IN_B_API2(INX_xml_stream_parser_ARG_parse_ignore_script);
    }
    //handle case where we have stored characters for cdata that are waiting to be output
    if(state->cdataString!=NULL)
    {
        //we have a cdataString waiting to be fired
        inx_xml_stream_parser_character_data(state,NULL,0);
        shouldParse=EHS_FALSE;
    }
    while( shouldParse == EHS_TRUE )
    {
        ii++;
        if(state->eatScriptContent==EHS_TRUE)
        {
            inx_xml_stream_parser_eat_script(state);
            if(state->eatScriptContent==EHS_TRUE)
            {
                shouldParse=EHS_FALSE;
            }
            continue;
        }
        parsed=EHS_FALSE;
        if(state->pausedForEvent == EHS_TRUE)
        {
            //did we stop at a start tag with attributes?
            if(state->attrs!=NULL)
            {
                state->attrs=inx_xml_stream_parser_handle_attributes(state->attrs,state->pParseFid,state);
                if(state->attrs!=NULL)
                {
                    //there are still attributes to be processed so wait till we are next called
                    break;
                }
            }
            else //attributes finished or did not exist so look whether there is an end tag to be output
                if( state->endTagName != NULL )
                {
                    if(EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_name))
                    {
                        strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_name),state->endTagName);
                    }
                    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_finish_element);
                    state->endTagName=NULL;
                    state->startTag=EHS_FALSE;
                    //break so that no more events are fired
                    break;
                }
            //no attributes so start from where we left off
            state->pausedForEvent=EHS_TRUE;
            status=XML_ResumeParser(hand);
            state->startTag=EHS_FALSE;
            parsed=EHS_TRUE;
        }
        else   /* not parsed for event */
        {
            chunk=input->chunk;
            //do we need to check the chunk for starting a script?
            if(state->ignoreScript==EHS_TRUE)
            {
                //look in the chunk for the start of a script
                while(input!=NULL)
                {
                    state->scriptEater.data=input->chunk;
                    state->scriptEater.position=state->scriptEater.data;
                    state->scriptEater.currentchunk=input;
                    inx_xml_stream_parser_script_eater_start_parse(state);
                    status=state->scriptEater.state;
                    if(status==0)
                    {
                        //no start found so safe to parse
                        size=EhsStrlen(chunk);
                        /* fprintf(state->parsed,"%s\n",chunk); fflush(state->parsed); */
                        status=XML_Parse(hand, chunk, size,0);
                        parsed=EHS_TRUE;
                        //clear off the startTag
                        state->startTag=EHS_FALSE;
                        break;
                    }
                    else if(status==1)
                    {
                        //get the next chunk but don't discard our previous chunks
                        input=input->pNext;
                    }
                    else if(status==2)
                    {
                        //found a valid <script> tag
                        //shorten the first chunk to just before the script tag
                        //state->scriptEater.startpos[0]='\0';
                        //turn on the script eater so that it chomps until </script> found
                        //first of all get the current position of the parser
                        //state->scriptEater.firstchunkpos=state->scriptEater.startpos;
                        state->eatScriptContent=EHS_TRUE;
                        state->scriptEater.state=0;
                        parsed=EHS_FALSE;
                        //input=NULL;
                        break;
                    }
                }
                if(input==NULL&&status==1)
                {
                    state->scriptEater.state=0;
                    state->waitingForInput=EHS_TRUE;
                    shouldParse=EHS_FALSE;
                }
            }
            else
            {
                size=EhsStrlen(chunk);
                /*fprintf(state->parsed,"%s\n",chunk);
                fflush(state->parsed);*/
                status=XML_Parse(hand, chunk, size,0);
                state->startTag=EHS_FALSE;
                parsed=EHS_TRUE;
            }
        }//if(state->read==1)
        if(parsed==EHS_TRUE)
        {
            //state->read=state->read+XML_GetCurrentByteCount(hand);
            if(status == XML_STATUS_ERROR)
            {
                EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_error);
                /*fclose(state->received);
                fclose(state->parsed);*/
                break;
            }
            else if(status==2)
            {
                //parsing paused due to event
                state->pausedForEvent=EHS_TRUE;
                break;
            }
            else
            {

                //status equaled 0 so end of file hit?
                //is there a next chunk?
                if(input->pNext!=NULL)
                {
                    //there is a next chunk so use that
                    pOld=input;
                    input=pOld->pNext;
                    if (pOld) EhsHMem_tempFree(pOld); /*todo crashes here */
                    pOld=NULL;
                    state->pausedForEvent=EHS_FALSE;
                    state->input=input;
                }
                else
                {
                    //no next chunk so just free the input
                    EhsHMem_tempFree(input);
                    input=NULL;
                    state->input=NULL;
                    state->pausedForEvent=EHS_FALSE;
                    //we are out data, either we need more or the stream is finished
                    //do we need to fire the parse finished port?
                    if(state->streamFinished==EHS_TRUE)
                    {
                        EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_finished);
                    }
                    else
                    {
                        state->waitingForInput=EHS_TRUE; //set this flag so that we know to kick off parsing again when we next read something
                    }
                    shouldParse=EHS_FALSE;
                }
            }//if(status == XML_STATUS_ERROR)
        }//if (parsed==EHS_TRUE)
    }//while(shouldParse==EHS_TRUE)
    /*if (EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_name))
    	EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_name) ;
    if (EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_cdata))
    	EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_cdata) ;
    if (EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_attr_name))
    	EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_attr_name) ;
    if (EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_parse_attr_value))
    	EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_parse_attr_value) ;*/
    //EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_next_finish);
}

//ICB FUNCTION eos MACRO START -- DO NOT ALTER
/**
 * Definition of xml_stream_parser_eos.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(xml_stream_parser_eos)
{
    inx_xml_stream_parser_state_type* inx_xml_stream_parser_state = (inx_xml_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_xml_stream_parser_cleanup(inx_xml_stream_parser_state);
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_eos_reset_finish);
}//ICB FUNCTION eos MACRO END -- DO NOT ALTER THIS LINE


//ICB FUNCTION read MACRO START -- DO NOT ALTER
/**
 * Definition of xml_stream_parser_read.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(xml_stream_parser_read)
{
    inx_xml_stream_parser_state_type* inx_xml_stream_parser_state = (inx_xml_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;
    inx_xml_stream_parser_state_type *state=inx_xml_stream_parser_state;
    inx_xml_stream_parser_chunk_type* input=inx_xml_stream_parser_state->input;
    inx_xml_stream_parser_chunk_type* pNew=NULL;
    inx_xml_stream_parser_chunk_type* pCurrent=NULL;
    ehs_char* chunk=NULL;
    XML_Parser hand=state->hand;
    state->pReadFid=pFIdata;
    // Your code here
    //now write it out to file
    /*fprintf(state->received,"%s\n",EHS_FB_IN_S_API2(INX_xml_stream_parser_ARG_read_data));
    fflush(state->received);*/
    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_xml_stream_parser_ARG_read_data))
    {
        //initialise handle
        if(hand==NULL)
        {
            state->hand = XML_ParserCreate(NULL);
            if (! state->hand)
            {
                fprintf(stderr, "Couldn't allocate memory for parser\n");
                exit(-1); /*todo someting else here! */
            }
            XML_SetUserData(state->hand,(void*)state);
            XML_SetElementHandler(state->hand,inx_xml_stream_parser_start,inx_xml_stream_parser_end);
            XML_SetCharacterDataHandler(state->hand,inx_xml_stream_parser_character_data);
            XML_SetProcessingInstructionHandler(state->hand,inx_xml_stream_parser_processing);
            XML_SetCommentHandler(state->hand,inx_xml_stream_parser_comment);
            XML_SetCdataSectionHandler(state->hand,inx_xml_stream_parser_cdata_start,inx_xml_stream_parser_cdata_end);
            inx_xml_stream_parser_state->input=NULL;
            input=NULL;
            hand=inx_xml_stream_parser_state->hand;
            inx_xml_stream_parser_state->pausedForEvent=EHS_FALSE;
        }
        //deal with ampersand replacement
        //read in the replacement character
        if(EHS_FB_IN_CONNECTED_API2(INX_xml_stream_parser_ARG_read_ignore_entities))
        {
            state->ampersandReplace=((ehs_char*)EHS_FB_IN_S_API2(INX_xml_stream_parser_ARG_read_ignore_entities))[0];
        }
        //what we are being presented with should be held in a linked list
        if(input==NULL)
        {
            //no existing input so this is the first
            input=(inx_xml_stream_parser_chunk_type*)EhsHMem_tempAlloc(sizeof(inx_xml_stream_parser_chunk_type));
            input->pNext=NULL;
            chunk=input->chunk;
            inx_xml_stream_parser_state->input=input;
        }
        else
        {
            //input is already there so use the pNext
            //find the next free slot
            pCurrent=input;
            while(pCurrent->pNext!=NULL)
            {
                pCurrent=pCurrent->pNext;
            }
            pCurrent->pNext=(inx_xml_stream_parser_chunk_type*)EhsHMem_tempAlloc(sizeof(inx_xml_stream_parser_chunk_type));
            pCurrent->pNext->pNext=NULL;
            chunk=pCurrent->pNext->chunk;
        }
        //copy our new chunk of data in
        strcpy(chunk,EHS_FB_IN_S_API2(INX_xml_stream_parser_ARG_read_data));
        //TODO sort out reading in the ampersand replace parameter correctly
        //if(state->ampersandReplace!='\0'&&state->ampersandReplace!='&'){
        //clean our chunk of ampersands
        //chunk=EHS_FB_IN_S_API2(INX_xml_stream_parser_ARG_read_data);
        char c=chunk[0];
        while(c!='\0')
        {
            if(c=='&')
            {
                //chunk[0]=state->ampersandReplace;
                chunk[0]=state->ampersandReplace;
            }
            chunk++;
            c=chunk[0];
        }
        //}
        if (EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_read_data_out))
        {
            strcpy(EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_read_data_out),chunk);
        }
        //do we need to fire the parser off again?
        if(state->waitingForInput==EHS_TRUE)
        {
            //yep the parser is waiting so kick it off again
            state->waitingForInput=EHS_FALSE;
            inx_xml_stream_parser_parse(state);
        }
    }
    /*if (EHS_FB_OUT_CONNECTED_API2(INX_xml_stream_parser_ARG_read_data_out))
    	EHS_FB_OUT_S_API2(INX_xml_stream_parser_ARG_read_data_out) ;*/

    /*if (EHS_FB_IN_CONNECTED_API2(INX_xml_stream_parser_ARG_read_ignore_entities))
    	EHS_FB_IN_S_API2(INX_xml_stream_parser_ARG_read_ignore_entities) ;
    */
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_read_read_finish);
    return;
}//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION parse MACRO START -- DO NOT ALTER
/**
 * Definition of xml_stream_parser_parse.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(xml_stream_parser_parse)
{
    inx_xml_stream_parser_state_type* inx_xml_stream_parser_state = (inx_xml_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;

    inx_xml_stream_parser_state_type *state=inx_xml_stream_parser_state;
    state->pParseFid=pFIdata; //hold a handle to this function instance data so we can fire ports from the xml callbacks
    inx_xml_stream_parser_parse(state);


    EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_next_finish);
    //EHS_FB_FINISH(INX_xml_stream_parser_ARG_parse_finished);
}//ICB FUNCTION parse MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION streamFinished MACRO START -- DO NOT ALTER
/**
 * Definition of xml_stream_parser_streamFinished.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(xml_stream_parser_streamFinished)
{
    inx_xml_stream_parser_state_type* inx_xml_stream_parser_state = (inx_xml_stream_parser_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_xml_stream_parser_state->streamFinished=EHS_TRUE;
    EHS_FB_FINISH(INX_xml_stream_parser_ARG_streamFinished_streamFinishedFinish);
}//ICB FUNCTION streamFinished MACRO END -- DO NOT ALTER THIS LINE
