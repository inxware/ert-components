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
#include "inx-JSONObjectFunctionBlock.h"
#include "yajl_parse.h"
#include "yajl_gen.h"

/* TODO - wouldn't this block be better implemented with YAJL tree functions? We iterate the whole tree for every query at mo.?*/

//ICB HEADER MACRO END -- DO NOT ALTER
typedef struct inx_JSONObject_chunk_struct
{
    ehs_char chunk[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH! leave this max and truncate & warn incoming?
    struct inx_JSONObject_chunk_struct* pNext;
} inx_JSONObject_chunk_type;

typedef struct inx_JSONObject_queryPart_struct
{
    ehs_char *map;
    struct inx_JSONObject_queryPart_struct* pNext;
} inx_JSONObject_queryPart_type;

typedef struct inx_JSONObject_array_struct
{
    ehs_sint32 index;
    ehs_uint32 depth;
    struct inx_JSONObject_array_struct *pNext;
    struct inx_JSONObject_array_struct *pPrev;
} inx_JSONObject_array_type;

typedef enum
{
    parse_error=0,
    parse_end,
    parse_map,
    parse_array
} inx_JSONObject_parse_state;

typedef enum
{
    json_string=0,
    json_number=1,
    json_boolean=2,
    json_null=3,
    json_array=4,
    json_object=5,
    json_object_truncated=6
} inx_JSONObject_dataType;
//ICB STATE VAR MACRO START -- DO NOT ALTER

//#define EHS_MAX_JSON_DEPTH 100
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    yajl_handle hand;
    inx_JSONObject_chunk_type* input;
    inx_JSONObject_queryPart_type *query;
    inx_JSONObject_queryPart_type *queryPos;
    ehs_char result[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!
    ehs_uint32 parseLevel;
    ehs_uint32 queryLevel;
    ehs_sint32 returnLevel;
    inx_JSONObject_dataType dataType;
    ehs_uint32 resultLength;
    ehs_char needcomma[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!??
    inx_JSONObject_array_type *arrayStart;
    inx_JSONObject_array_type *arrayPos;
    ehs_bool fieldNameMode;
    //ehs_sint32 array_index_at_level[EHS_MAX_JSON_DEPTH];
} inx_JSONObject_state_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
static void cleanup(inx_JSONObject_state_type *state);
static ehs_uint32 append(inx_JSONObject_state_type *state,const ehs_char *sou);
static void needcomma(inx_JSONObject_state_type *state);
static ehs_bool testArray(inx_JSONObject_state_type *state);
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(JSONObject)

EHS_FB_FUNCTION_ENTRY("reset", 0x01, JSONObject_reset)

EHS_FB_FUNCTION_ENTRY("read", 0x02, JSONObject_read)

EHS_FB_FUNCTION_ENTRY("query", 0x03, JSONObject_query)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
//#define INX_JSONObject_ARG_read_readFinish 1
//#define INX_JSONObject_ARG_read_readData 1
//#define INX_JSONObject_ARG_read_readFinish 1
//#define INX_JSONObject_ARG_query_queryData 1
//#define INX_JSONObject_ARG_query_queryFinish 1
//#define INX_JSONObject_ARG_query_queryError 2
//#define INX_JSONObject_ARG_read_data 1
//#define INX_JSONObject_ARG_query_error 2
#define INX_JSONObject_ARG_reset_rest_finished 1

#define INX_JSONObject_ARG_read_data 1
#define INX_JSONObject_ARG_read_read_finished 1

#define INX_JSONObject_ARG_query_path1 1
#define INX_JSONObject_ARG_query_path2 2
#define INX_JSONObject_ARG_query_path3 3
#define INX_JSONObject_ARG_query_path4 4
#define INX_JSONObject_ARG_query_path5 5
#define INX_JSONObject_ARG_query_path6 6
#define INX_JSONObject_ARG_query_path7 7
#define INX_JSONObject_ARG_query_path8 8

#define INX_JSONObject_ARG_query_result2 1
#define INX_JSONObject_ARG_query_result3 2
#define INX_JSONObject_ARG_query_result1 3
#define INX_JSONObject_ARG_query_result5 4
#define INX_JSONObject_ARG_query_result6 5
#define INX_JSONObject_ARG_query_result7 6
#define INX_JSONObject_ARG_query_result8 7
#define INX_JSONObject_ARG_query_result4 8

#define INX_JSONObject_ARG_query_error 1
#define INX_JSONObject_ARG_query_queryOK 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(JSONObject)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,""); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_JSONObject_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(JSONObject)
{
    ehs_uint32 i;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_JSONObject_state_type* inx_JSONObject_state = (inx_JSONObject_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
// no parameters set	EhsSscanf(EHS_FB_INIT_PARAMETERS,"");

    /* Add any further intialisation code here */
    inx_JSONObject_state->input=NULL;
    inx_JSONObject_state->hand=NULL;
    inx_JSONObject_state->query=NULL;
    inx_JSONObject_state->arrayStart=NULL;
    //for (i=0;i<EHS_MAX_JSON_DEPTH; i++)
    //	inx_JSONObject_state->array_index_at_level[i] = 0;
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(JSONObject)
{
    inx_JSONObject_state_type *inx_JSONObject_state = (inx_JSONObject_state_type*)EHS_FB_DESTROY_CONTEXT;
    //Your code below here
    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)EHS_FB_DESTROY_CONTEXT;
    cleanup(state);
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION reset MACRO START -- DO NOT ALTER
/**
 * Definition of JSONObject_reset.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(JSONObject_reset)
{
    inx_JSONObject_state_type* inx_JSONObject_state = (inx_JSONObject_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here
    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)EHS_FB_RUN_CONTEXT;
    cleanup(state);
    EHS_FB_FINISH(INX_JSONObject_ARG_reset_rest_finished);
}//ICB FUNCTION reset MACRO END -- DO NOT ALTER THIS LINE

#if 0 // this was a mind fart  - we have array indexing it was just broken apparently
/* Call this from all call backs that have found something to see if the level is an array level
 * Assumes only one array acive at each level if JSON is parsed incrementally and start/stop
 * callbacks keep the indexes properly reset */
static void update_array_index_tracking(inx_JSONObject_state_type* state)
{

    if (state->parseLevel>0 && state->parseLevel < EHS_MAX_JSON_DEPTH)
    {
        if (state->array_index_at_level[state->parseLevel])
        {
            state->array_index_at_level[state->parseLevel]++;
        }
    }
}
#endif

static int callback_null(void * ctx)
{

    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    if (!state->fieldNameMode)   // we ignoe values in this mode and don't add anything to the result
    {
        if(state->returnLevel==state->parseLevel)
        {
            EhsStrcpy(state->result,"null");
            state->result[4]='\0';
            state->dataType=json_null;
            return 0;
        }
        else if(state->returnLevel!=-1&&state->parseLevel>state->returnLevel)
        {
            //append a null
            if(state->needcomma[state->parseLevel]>1)
            {
                if (!state->fieldNameMode)  needcomma(state);
            }
            if (!state->fieldNameMode) append(state,"null");
        }
        return testArray(state);
    }
    return 1;
}

static int callback_boolean(void * ctx, int boolean)
{

    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    if (!state->fieldNameMode)   // we ignoe values in this mode and don't add anything to the result
    {

        if(state->returnLevel==state->parseLevel)
        {
            state->dataType=json_boolean;
            if(boolean==0)
            {
                EhsStrcpy(state->result,"false");
                state->result[5]='\0';
            }
            else
            {
                EhsStrcpy(state->result,"true");
                state->result[4]='\0';
            }
            return 0;
        }
        else if(state->returnLevel!=-1&&state->parseLevel>state->returnLevel)
        {
            //append a boolean on to the result
            if(state->needcomma[state->parseLevel]>1)
            {
                if (!state->fieldNameMode) needcomma(state);
            }
            if(boolean==0)
            {
                if (!state->fieldNameMode) append(state,"false");
            }
            else
            {
                if (!state->fieldNameMode) append(state,"true");
            }
        }
        return testArray(state);
    }
    return 1;
}

static int callback_number(void * ctx, const char * s, size_t l)
{
    inx_JSONObject_state_type *state = (inx_JSONObject_state_type*)ctx;
    if (!state->fieldNameMode)   // we ignoe values in this mode and don't add anything to the result
    {
        ehs_char number[l+1];
        if (!state->fieldNameMode)   // we ignoe values in this mode and don't add anything to the result
        {
            number[l]='\0';
            if(state->returnLevel==state->parseLevel)
            {
                EhsStrncpy(state->result,s,l);
                state->result[l]='\0';
                state->dataType=json_number;
                return 0;
            }
            else if(state->returnLevel!=-1&&state->parseLevel>state->returnLevel)
            {
                //copy the number into a null terminated string
                EhsStrncpy(number,s,l);
                if(state->needcomma[state->parseLevel]>1)
                {
                    if (!state->fieldNameMode) needcomma(state);
                }
                append(state,number);
            }
            return testArray(state);
        }
    }
    return 1;
}

static int callback_string(void * ctx, const unsigned char * stringVal,size_t stringLen)
{

    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    if (!state->fieldNameMode)   // we ignoe values in this mode and don't add anything to the result
    {

        ehs_char string[stringLen+1];

        string[stringLen]='\0';
        //do we need to return this value
        if(state->returnLevel==state->parseLevel)
        {
            EhsStrncpy(state->result,stringVal,stringLen);
            state->result[stringLen]='\0';
            state->dataType=json_string;
            return 0;
        }
        else if(state->returnLevel!=-1&&state->parseLevel>state->returnLevel)
        {
            //copy the string into a null terminated string
            EhsStrncpy(string,stringVal,stringLen);
            if(state->needcomma[state->parseLevel]>1)
            {
                if (!state->fieldNameMode) needcomma(state);
            }
            if (!state->fieldNameMode)
            {
                append(state,"\"");
                append(state,string);
                append(state,"\"");
            }
        }
        return testArray(state);
    }
    return 1;
}
/**
 * @brief Finds a key at specific key level in the JSON
 */
static int callback_map_key(void * ctx,const unsigned char * stringVal,size_t stringLen)
{
    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    ehs_char mapName[stringLen+1];
    mapName[stringLen]='\0';
    //we found a map so compare it to our current position in the query
    if(state->returnLevel > -1 && state->parseLevel > state->returnLevel)
    {
        //append a map on to our result
        EhsStrncpy(mapName,stringVal,stringLen);
        if (!state->fieldNameMode)
        {
            needcomma(state);
            append(state,"\"");
            append(state,mapName);
            append(state,"\":");
        }
    }
    else if(state->parseLevel != state->queryLevel)
    {

        //we are at the wrong level so don't compare
    }
    else
    {
        //we are at the right level so see if they match
        EhsStrncpy(mapName,stringVal,stringLen);
        /* first check if we are in wldcard mode, where we only want to return fields at one level */
        if(EhsStrcmp(state->queryPos->map,"?") == 0)
        {
            append(state,mapName);
            append(state,",");
            if(state->queryPos->pNext==NULL)
            {
                state->returnLevel=state->parseLevel;
            }
            else
            {
                state->queryPos=state->queryPos->pNext;
                state->queryLevel++;
            }
            //state->returnLevel=state->parseLevel; //This should trigger a return?
        }
        else
        {
            if(EhsStrcmp(state->queryPos->map,mapName)==0)
            {
                //they do match so go down a level in the query if possible, if not indicate we want to return the next value or JSON object
                if(state->queryPos->pNext==NULL)
                {
                    state->returnLevel=state->parseLevel;
                }
                else
                {
                    state->queryPos=state->queryPos->pNext;
                    state->queryLevel++;
                }
            }
            else
            {
            }
        }
    }
    return 1;
}


/** @brief start mapping the path in the query to the database
 *
 */
static int callback_start_map(void * ctx)
{
    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    //we are going down a level
    //do we need to record this position
    if(state->returnLevel==state->parseLevel)
    {
        state->dataType=json_object;
        if (!state->fieldNameMode)
        {
            state->resultLength=0;
            append(state,"{");
        }
    }
    else if(state->returnLevel>-1&&state->parseLevel>state->returnLevel)
    {
        if(state->needcomma[state->parseLevel]>1)
        {
            if (!state->fieldNameMode) needcomma(state);
        }
        if (!state->fieldNameMode) append(state,"{");
    }
    state->parseLevel++;
    state->needcomma[state->parseLevel]=0;
    return 1;
}

static int callback_end_map(void * ctx)
{
    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    //we are going up a level
    state->parseLevel--;
    //was this a chunk of json we were looking for
    if(state->parseLevel==state->returnLevel)
    {
        if (!state->fieldNameMode) append(state,"}");
        return 0;
    }
    else if(state->returnLevel>-1&&state->parseLevel>state->returnLevel)
    {
        if (!state->fieldNameMode) append(state,"}");
    }
    testArray(state);
    return 1;
}

/**
 * @brief YAJL callback when we find an array in the database
 */
static int callback_start_array(void * ctx)
{
    inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
    if (!state->fieldNameMode)   // we ignore arrays (no names just values) in this mode and don't add anything to the result
    {
        inx_JSONObject_array_type *array,*acurrent;
        if(state->returnLevel==state->parseLevel)
        {
            state->dataType=json_array;
            state->resultLength=0;
            if (!state->fieldNameMode)  append(state,"[");
        }
        else if(state->returnLevel>-1&&state->parseLevel>state->returnLevel)
        {
            if (!state->fieldNameMode) append(state,"[");
        }
        state->parseLevel++;
        state->needcomma[state->parseLevel]=2;
        //create an a new array structure
        array=(inx_JSONObject_array_type*)EhsHMem_tempAlloc(sizeof(inx_JSONObject_array_type));
        array->index=0;
        array->depth=state->parseLevel;
        array->pNext=NULL;
        //add to the end of the current arrays
        acurrent=state->arrayStart;
        if(acurrent==NULL)
        {
            array->pPrev=NULL;
            state->arrayStart=array;
        }
        else
        {
            //find end of linked list
            while(acurrent->pNext!=NULL)
            {
                acurrent=acurrent->pNext;
            }
            array->pPrev=acurrent;
            acurrent->pNext=array;
        }
        state->arrayPos=array;
        return testArray(state);
    }
    return 1;
}

#if 0
else if(EhsStrcmp(state->queryPos->map,mapName)==0)
{
    //EHSH_LOG_INFO("match");
    //they do match so go down a level in the query if possible, if not indicate we want to return the next value or JSON object
    if(state->queryPos->pNext==NULL)
    {
        state->returnLevel=state->parseLevel;
    }
    else
    {
        state->queryPos=state->queryPos->pNext;
        state->queryLevel++;
    }
    if(EhsSscanf(mapName),&index)
    {
        //EHSH_LOG_INFO("match");
        //they do match so go down a level in the query if possible, if not indicate we want to return the next value or JSON object
        if(state->queryPos->pNext==NULL)
        {
            state->returnLevel=state->parseLevel;
        }
        else
        {
            state->queryPos=state->queryPos->pNext;
            state->queryLevel++;
        }


#endif

        /**
         * @brief YAJL callback for the end of an array
         */
        static int callback_end_array(void * ctx)
        {

            inx_JSONObject_state_type* state = (inx_JSONObject_state_type*)ctx;
            if (!state->fieldNameMode)   // we ignore arrays (no names just values) in this mode and don't add anything to the result
            {
                inx_JSONObject_array_type *array;
                //we are going up a level
                state->parseLevel--;
                //was this a chunk of json we were looking for
                if(state->parseLevel==state->returnLevel)
                {
                    if (!state->fieldNameMode) append(state,"]");
                    return 0;
                }
                else if(state->returnLevel>-1&&state->parseLevel>state->returnLevel)
                {
                    if (!state->fieldNameMode) append(state,"]");
                }
                //free up current arrayPos and go back one
                array=state->arrayPos->pPrev;
                EhsHMem_tempFree(state->arrayPos);
                state->arrayPos=array;
                if(state->arrayPos!=NULL)
                {
                    state->arrayPos->pNext=NULL;
                }
                else
                {
                    //at start of list again so make sure that is null
                    state->arrayStart=NULL;
                }
            }
            return 1;
        }

        static yajl_callbacks callbacks =
        {
            callback_null,
            callback_boolean,
            NULL,
            NULL,
            callback_number,
            callback_string,
            callback_start_map,
            callback_map_key,
            callback_end_map,
            callback_start_array,
            callback_end_array
        };

        /** @brief appends matched content to the output result buffer in state
         *
         */
        static ehs_uint32 append(inx_JSONObject_state_type *state,const ehs_char *sou)
        {
            ehs_uint32 slength=EhsStrlen(sou);

            if((state->resultLength+slength)>=EHS_STRING_LENGTH_MAX)
            {
                //too big, set type to truncated
                state->dataType=json_object_truncated;
                return 0;
            }
            else
            {
                EhsStrcat(state->result,sou);
                state->resultLength=state->resultLength+slength;
                return 1;
            }
        }
        static void cleanup(inx_JSONObject_state_type *state)
        {
            ehs_uint32 i ;
            yajl_handle hand=state->hand;
            inx_JSONObject_chunk_type* current;
            inx_JSONObject_chunk_type* parent;
            inx_JSONObject_queryPart_type *qcurrent;
            inx_JSONObject_queryPart_type *qparent;
            inx_JSONObject_array_type *acurrent;
            inx_JSONObject_array_type *aparent;
            if(hand!=NULL)
            {
                yajl_free(hand);
                state->hand=NULL;
            }
            //free up the linked list
            current=state->input;
            if(current!=NULL)
            {
                while(current!=NULL)
                {
                    parent=current;
                    current=current->pNext;
                    EhsHMem_tempFree(parent);
                    parent=NULL;
                }
            }
            state->input=NULL;
            //do the same for the query
            qcurrent=state->query;
            if(qcurrent!=NULL)
            {
                while(qcurrent!=NULL)
                {
                    qparent=qcurrent;
                    qcurrent=qcurrent->pNext;
                    EhsHMem_tempFree(qparent);
                    qparent=NULL;
                }
            }
            state->query=NULL;
            //same for arrays
            acurrent=state->arrayStart;
            while(acurrent!=NULL)
            {
                aparent=acurrent;
                acurrent=acurrent->pNext;
                EhsHMem_tempFree(aparent);
                aparent=NULL;
            }
            state->arrayStart=NULL;
            state->fieldNameMode=EHS_FALSE;
            //for (i=0;i<EHS_MAX_JSON_DEPTH; i++) - remote this
            //	state->array_index_at_level[i]=0;
        }


        /** @brief adds a comma to the output result if needed
         * */

        static void needcomma(inx_JSONObject_state_type *state)
        {
            if(state->needcomma[state->parseLevel]==1)
            {
                append(state,",");
            }
            else if(state->needcomma[state->parseLevel]==0)
            {
                state->needcomma[state->parseLevel]=1;
            }
            else if(state->needcomma[state->parseLevel]==2)
            {
                state->needcomma[state->parseLevel]=3;
            }
            else if(state->needcomma[state->parseLevel]==3)
            {
                append(state,",");
            }
        }


        /**
         * @brief Checks if there is a match at the right level in the query array.
         *
         * Updates the state info
         */
        static ehs_bool testArray(inx_JSONObject_state_type *state)
        {
            ehs_char buf[EHS_STRING_LENGTH_MAX];
            ehs_uint32 notfound = 1; // this is the number format the YAJL parser likes
            //check if we are in an array

            if(state->arrayPos!=NULL && state->arrayPos->depth == state->parseLevel)
            {
                //test to see if we need to return the next item
                if(state->parseLevel == state->queryLevel)
                {
                    //we are at the right level so see if they match
                    //convert integer index to string
                    EhsSprintf(buf,"%d",state->arrayPos->index);

                    if(EhsStrcmp(state->queryPos->map,buf)==0)
                    {
                        //they do match so go down a level in the query if possible, if not indicate we want to return the next value or JSON object
                        if(state->queryPos->pNext==NULL)
                        {
                            state->returnLevel=state->parseLevel;
                            notfound = 1;
                            //EhsStrncpy(state->result,stringVal,stringLen);
                            //state->result[stringLen]='\0';
                            //state->dataType=json_string;
                        }
                        else
                        {
                            state->queryPos=state->queryPos->pNext;
                            state->queryLevel++;
                        }
                    }
                }
                //increment the index
                state->arrayPos->index++;
            }
            return notfound;
        }

        /** @brief parses a JSON array that is read in to be queried.
         *
         * This parses the JSON PATH stuff
         *
         *
         */
        static inx_JSONObject_parse_state lexer_lex(ehs_char *string,ehs_uint32 length,ehs_uint32 *offset,ehs_char **outbuf)
        {
            ehs_uint32 start; //holds the start point of a token
            ehs_uint32 tokLength=0; //holds the length of the current token
            inx_JSONObject_parse_state type = parse_end;
            ehs_char* outbuffer;
            //test for end of string
            ehs_uint32 position=*offset;
            if(position>=length)
            {
                return parse_end;
            }
            char c,lastc='\0';
            c=string[position];
            if ( ( position == 0 && (c == '[' || c == '.'|| c == '$')) ||
                    ( position > 0) ) position++;
            if( c=='$')   //skip the initial $ as we only support root path anyway
            {
                c=string[position];
                position++;
            }
            if (    ( c == '[' || c == '.') ||
                    ( position == 0 /* inferred from above && (c != '[' && c != '.')*/) )   // will accept either JSON path or a single entry with no delimeters
            {
                type=parse_array; //assume we are going to be parsing an array, this will be changed if we hit any non numeric characters
                //mark the start position
                start=position;
                //try to read till ]
                while(1)
                {
                    //if we hit the end of the string throw an error
                    if(position>length)
                    {
                        return parse_error;
                    }
                    //read the next char
                    lastc = c;
                    c=string[position];
                    if( ( (c==']' || c=='.') && lastc != '\\') || c == '\0'  || position == (length) )  // use end of line or delimeter to finish
                    {
                        //end of token so allocate space and copy to the outbuf, figure out if map key or an array index
                        //is the token long enough
                        if(position<(start+1))
                        {
                            EHSH_LOG_WARNING("Token not long enough");
                            return parse_error;
                        }
                        tokLength=position-start;
                        outbuffer=(ehs_char*)EhsHMem_tempAlloc(sizeof(ehs_char)*(tokLength+1)); //+1 for the null terminator
                        outbuffer[tokLength]='\0';
                        EhsStrncpy(outbuffer,&string[start],tokLength);
                        *outbuf=outbuffer;
                        //update the offset
                        if (c != '.')  // if this is dot delineated then we'll leave the dot there so we parse it again next loop
                            position++;
                        *offset=position;
                        return type;
                    }
                    else
                    {

                        //we've hit a character so change type and make like a turkey 'gobble gobble'
                        type=parse_map;
                        position++;
                    }
                }
            }
            else
            {
                EHSH_LOG_WARNING("Query JSON path string error");
                return parse_error;
            }
            return type;
        }

        /** @brief parses a JSON query string, which is in JSONPAth format (e.g. [a][b][c]... or a.b.c
         *
         */
        static inx_JSONObject_queryPart_type* parse_query(ehs_char *queryString)
        {
            inx_JSONObject_parse_state found;
            inx_JSONObject_queryPart_type *queryPart;
            inx_JSONObject_queryPart_type *queryStart=NULL;
            inx_JSONObject_queryPart_type *current=NULL;
            ehs_uint32 size=EhsStrlen(queryString);
            ehs_uint32 offset=0;
            ehs_char *token=NULL;
            while(1)
            {
                found=lexer_lex(queryString,size,&offset,&token);
                if(found==parse_end)
                {
                    //EHSH_LOG_INFO("Query parse_end");
                    return queryStart;
                }
                else if(found==parse_error)
                {
                    EHSH_LOG_WARNING("Query parse error in %s",queryString);
                    //free up any allocated query parts
                    current=queryStart;
                    while(current!=NULL)
                    {
                        queryPart=current;
                        current=current->pNext;
                        EhsHMem_tempFree(queryPart);
                        queryPart=NULL;
                    }
                    return NULL;
                }
                else if(found==parse_map)
                {
                    //EHSH_LOG_WARNING("Query found @%s@",token);
                    //we have found a map, token is the name of that map. Add it to the end of our linked list of maps we are looking for
                    queryPart=(inx_JSONObject_queryPart_type*)EhsHMem_tempAlloc(sizeof(inx_JSONObject_queryPart_type));
                    queryPart->map=token;
                    queryPart->pNext=NULL;
                    //add to end of query list
                    if(queryStart==NULL)
                    {
                        queryStart=queryPart;
                    }
                    else
                    {
                        current=queryStart;
                        while(current->pNext!=NULL)
                        {
                            current=current->pNext;
                        }
                        current->pNext=queryPart;
                    }
                }
            }
        }

//ICB FUNCTION read MACRO START -- DO NOT ALTER
        /**
         * Definition of JSONObject_read.
         * @brief adds JSON to the internally stored JSON buffer.
         *
         * The JSON format must be labelled JSON values (or sub JSON structures). It should not be an array
         * Returns only the first entry if there are multiple entries with the same name (for arrays too)
         *
         * @todo - If this contains an array then the query should return elements that match a query in JSONPath format [jim][bob[thisone]] or jim.bob[thisone] using dot notation.
         * @todo support multiple returns from arrays  e.g. [jim][bob[*]] - will return all in the array labelled bob.
         *
         *
         * [User's info entered in ICB added here]
         * This function can access the object data shared using the following macros:
         *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
         *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
         */
        EHS_FB_RUN_FUNCTION(JSONObject_read)
        {
            inx_JSONObject_state_type* inx_JSONObject_state = (inx_JSONObject_state_type*)EHS_FB_RUN_CONTEXT;

            inx_JSONObject_state_type *state = inx_JSONObject_state;
            inx_JSONObject_chunk_type *input=state->input;
            inx_JSONObject_chunk_type *current=NULL;
            ehs_char *chunk=NULL;
            if (EHS_FB_IN_CONNECTED_API2(INX_JSONObject_ARG_read_data))
            {
                if(input==NULL)
                {
                    //our first string so make start of list
                    input=(inx_JSONObject_chunk_type*)EhsHMem_tempAlloc(sizeof(inx_JSONObject_chunk_type));
                    input->pNext=NULL;
                    chunk=input->chunk;
                    state->input=input;
                }
                else
                {
                    //add to end of list
                    current=input;
                    while(current->pNext!=NULL)
                    {
                        current=current->pNext;
                    }
                    current->pNext=(inx_JSONObject_chunk_type*)EhsHMem_tempAlloc(sizeof(inx_JSONObject_chunk_type));
                    current=current->pNext;
                    current->pNext=NULL;
                    chunk=current->chunk;
                }
                EhsStrcpy(chunk,EHS_FB_IN_S_API2(INX_JSONObject_ARG_read_data));
            }
            EHS_FB_FINISH(INX_JSONObject_ARG_read_read_finished);


        }//ICB FUNCTION read MACRO END -- DO NOT ALTER THIS LINE


        static ehs_bool clearQuery(inx_JSONObject_state_type *state)
        {

            inx_JSONObject_queryPart_type *qcurrent;
            inx_JSONObject_queryPart_type *qparent;
            inx_JSONObject_array_type *acurrent;
            inx_JSONObject_array_type *aparent;

            //initialise handle
            if(state->hand!=NULL)
            {
                yajl_free(state->hand);
            }
            state->hand=yajl_alloc(&callbacks,NULL,(void*)state);
            //clear any stored query
            qcurrent=state->query;
            while(qcurrent!=NULL)
            {
                qparent=qcurrent;
                qcurrent=qcurrent->pNext;
                EhsHMem_tempFree(qparent);
                qparent=NULL;
            }
            state->query=NULL;
            //same for arrays
            acurrent=state->arrayStart;
            while(acurrent!=NULL)
            {
                aparent=acurrent;
                acurrent=acurrent->pNext;
                EhsHMem_tempFree(aparent);
                aparent=NULL;
            }
            state->arrayStart=NULL;
            state->fieldNameMode=EHS_FALSE;

            return EHS_TRUE;
        }

        static ehs_bool doQuery(inx_JSONObject_state_type * state, ehs_char * queryString, ehs_char * out)
        {
            inx_JSONObject_chunk_type *input;
            yajl_status status;
            ehs_char *chunk=NULL;
            ehs_uint32 size=0;
            ehs_bool ret = EHS_TRUE;
            /*
            if (EHS_FB_OUT_CONNECTED_API2(INX_JSONObject_ARG_query_queryResult))
            		EHS_FB_OUT_S_API2(INX_JSONObject_ARG_query_queryResult) ;
             */
            //do we have any input?
            input=state->input;
            if(input==NULL||state->hand==NULL)
            {
                EHSH_LOG_WARNING("No data to query");
                return EHS_FALSE;
            }
            if (queryString)
            {
                ehs_char tempBuff[EHS_STRING_LENGTH_MAX];
                ehs_char * input = queryString;
                int i,j=0;
                tempBuff[0]='\0';
                // remove any quotes in the string format
                for (i=0; i < EHS_STRING_LENGTH_MAX; i++)
                {
                    if (i>0 && input[i] == '?')
                    {
                        if (input[i-1] == '[' && input[i+1] == ']')
                            state->fieldNameMode = EHS_TRUE;
                    }
                    if ( (input[i] != '"' && input[i] != '\'' && input[i] != '\\') || (i > 0 && input[i-1] == '\\' ) )   // quotes and slash need to be escaped if in the tag
                    {
                        tempBuff[j] = input[i];
                        j++;
                    }
                    else if (input[i] == '\0') break;
                }
                state->query=parse_query(tempBuff);
            }
            //did we get a valid query
            if(state->query==NULL)
            {
                //query error so throw an error
                EHSH_LOG_WARNING("Query parse error @%s@",queryString);
                if (out) out[0]='\0';
                return EHS_FALSE;
            }
            //reset the level before we start parsing
            state->returnLevel=-1;
            state->parseLevel=0;
            state->queryLevel=1;
            state->queryPos=state->query;
            state->arrayPos=state->arrayStart;
            state->result[0]='\0';
            state->resultLength=0;
            if (state->fieldNameMode == EHS_TRUE)
                append(state,"["); // start an array for field names if we are in this mode

            while(input!=NULL)
            {
                chunk=input->chunk;
                size=EhsStrlen((ehs_char*)chunk);
                status=yajl_parse(state->hand,(unsigned char*)chunk,size);
                if(status != yajl_status_ok && status != yajl_status_client_canceled)
                {
                    ehs_char * str = (ehs_char*)yajl_get_error(state->hand,1,(unsigned char*)chunk,size);
                    ret = EHS_FALSE;
                    EHSH_LOG_WARNING("%s",(const char *)str);
                    yajl_free_error(state->hand,(unsigned char*)str);
                    // EHS_FB_FINISH(INX_JSONObject_ARG_query_queryError);

                    break;
                }
                else if(status == yajl_status_client_canceled)
                {
                    //ended because we have our result
                    ret = EHS_TRUE;
                    break;
                }
                else
                {
                    //status equalled 0 so end of file hit?
                    //is there a next chunk?
                    if(input->pNext!=NULL)
                    {
                        //there is a next chunk so use that
                        input=input->pNext;
                    }
                    else
                    {
                        //no more so break
                        if (!state->fieldNameMode) ret = EHS_FALSE;
                        else ret = EHS_TRUE; //In this mode we expect to git the end of the file - if we have found something or not.
                        break;
                    }
                }
            }

            return ret;
        }


//ICB FUNCTION query MACRO START -- DO NOT ALTER
        /**
         * @brief return a JSON element that matches the array of Tag strings provided:
         *
         * query strings are of the form [firstLevel][secondLevel][..], where each string represents the nesting of node tags that are being retrieved.
         * query strings may also be of the form
         * 			["firstLevel"][ "secondLevel"][...]  or
         * 		  	['firstLevel']['secondLevel'][...] or
         * 		   	firstLevel.secondLevel or
         * 		   	firstLevel
         * 		   	$.firstLevel
         *
         * 		   	as in JSON path syntax
         * 		   	 square brackets or dots may by included in strings using \ escape e.g. \" \[ \' \.
         *
         * 		   	 only asserts an error output if all queries fail.
         *
         * Definition of JSONObject_query.
         * [User's info entered in ICB added here]
         * This function can access the object data shared using the following macros:
         *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
         *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
         */
        EHS_FB_RUN_FUNCTION(JSONObject_query)
        {
            inx_JSONObject_state_type* inx_JSONObject_state = (inx_JSONObject_state_type*)EHS_FB_RUN_CONTEXT;

            inx_JSONObject_state_type *state=inx_JSONObject_state;
            ehs_char * out = NULL;
            ehs_char * query = NULL;
            ehs_bool aFail = EHS_FALSE;
            ehs_bool someOk = EHS_FALSE;


            int inPortArray[]=
            {
                INX_JSONObject_ARG_query_path1,
                INX_JSONObject_ARG_query_path2,
                INX_JSONObject_ARG_query_path3,
                INX_JSONObject_ARG_query_path4,
                INX_JSONObject_ARG_query_path5,
                INX_JSONObject_ARG_query_path6,
                INX_JSONObject_ARG_query_path7,
                INX_JSONObject_ARG_query_path8
            };

            int outPortArray[]=
            {
                INX_JSONObject_ARG_query_result1,
                INX_JSONObject_ARG_query_result2,
                INX_JSONObject_ARG_query_result3,
                INX_JSONObject_ARG_query_result4,
                INX_JSONObject_ARG_query_result5,
                INX_JSONObject_ARG_query_result6,
                INX_JSONObject_ARG_query_result7,
                INX_JSONObject_ARG_query_result8
            };

            int i;
            /* do all 8 potential query inputs */
            for (i=0; i<8; i++)
            {
                clearQuery(state);

                if (EHS_FB_IN_CONNECTED_API2(inPortArray[i]))
                {
                    if(EHS_FB_OUT_CONNECTED_API2(outPortArray[i]))
                    {
                        out = EHS_FB_OUT_S_API2(outPortArray[i]);
                        if (EHS_TRUE == doQuery( state, EHS_FB_IN_S_API2(inPortArray[i]), out))
                        {
                            someOk = EHS_TRUE;
                            if (state->fieldNameMode == EHS_TRUE)
                            {
                                int len = state->resultLength;
                                if ( len > 0 && state->result[len-1] == ',')
                                {
                                    state->result[len-1] = '\0'; // remove the trailing comma
                                    state->resultLength--;
                                }
                                append(state,"]"); // start an array for field names if we are in this mode
                            }
                            if(out)
                            {
                                EhsDataString_set(out,state->result);
                            }
                        }
                        else
                        {
                            aFail = EHS_TRUE;
                            if (out) out[0]='\0';
                        }
                    }
                    else out = NULL;


                }
            }

            if (EHS_TRUE == someOk)
                EHS_FB_FINISH(INX_JSONObject_ARG_query_queryOK);
            else
                EHS_FB_FINISH(INX_JSONObject_ARG_query_error);


        }//ICB FUNCTION query MACRO END -- DO NOT ALTER THIS LINE
