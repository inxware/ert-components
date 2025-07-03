//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-scheduler.h"
#include <errno.h>
#include <unistd.h>
//#include "hal-api.h"
#include "target_file.h"
#include "target_string.h"
#include <stdio.h>

#define CONFIG_MAX_SCHEDULE_SIZE_BYTES 1024
#define CONFIG_HOURS_IN_WEEK 168
#define CONFIG_SCHEDULE_MAX 9

#define CONFIG_SCHEDULE_VERSION 3 
#define CONFIG_SCHEDULE_HEADER_SIZE (1 + 2 + 1 + 1 + 1) // ver,crc,id,chunk,end (6 bytes)

typedef struct {
    ehs_uint16 minutesFromSunday;
    ehs_uint8 value;
} inxScheduleV1_t;

//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_DevmanScheduler_state
{
    ehs_uint8 version;
    ehs_uint32 crc32;
    ehs_uint8 pendingChunks;
    ehs_sint32 dataSize;
    ehs_uint8 data[CONFIG_MAX_SCHEDULE_SIZE_BYTES];
    ehs_sint32 channelID;
    ehs_bool autoMode; // Todo: this doesn't do anything currrently?
    ehs_bool written;
    ehs_bool isOn;
    ehs_sint32 temperature; // Stored temperature
} inx_DevmanScheduler_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER

/** TODO: implement EhsFseek from fseek
 * Read the scheduler from file with given schedule ID in the schedule array
 * Return code:
 *  0 Success
 *  1 Schedule buffer is NULL
 *  2 Schedule not found
 *  3 Schedule size overflow
 *  4 Schedule read size does not match the file size
 */
ehs_uint8 target_readScheduleFromFile(inx_DevmanScheduler_state_type* state, ehs_uint8* schedule, ehs_uint32* pfile_size, ehs_sint32 id)
{
    if (state == NULL || schedule == NULL || pfile_size == NULL) return 1;
    if (id > CONFIG_SCHEDULE_MAX) return 2;
    ehs_char filename[20] = {0};
    EhsSprintf(filename, "_sched%d.scd", id);
    ehs_FILE *fp = Ehs_UserFopen(filename, "rb");
    if (fp == NULL) return 2;

    // Get the size of the scheduler file
    fseek(fp, 0L, SEEK_END);
    ehs_uint32 file_size = EhsFtell(fp);
    if (file_size > CONFIG_MAX_SCHEDULE_SIZE_BYTES+CONFIG_SCHEDULE_HEADER_SIZE)
    {
        EhsFclose(fp);
        return 3;
    }else if(file_size < CONFIG_SCHEDULE_HEADER_SIZE){
        EhsFclose(fp);
        return 4;
    }
    fseek(fp, 0, SEEK_SET);

    // read header from the file
    ehs_uint8 header[CONFIG_SCHEDULE_HEADER_SIZE] = {0};
    ehs_uint32 header_size = EhsFread(header, sizeof(ehs_uint8), CONFIG_SCHEDULE_HEADER_SIZE, fp);
    if(header_size != CONFIG_SCHEDULE_HEADER_SIZE){
        EhsFclose(fp);
        return 4;
    }
    state->version = header[0];
    state->crc32 = ((ehs_uint16)(ehs_uint8)header[2] << 8) | (ehs_uint16)(ehs_uint8)header[1];
    ehs_uint8 header_id = header[3];
    if(id != header_id){
        printf("File contains incorrect scheduler id (%d) in the header (%d)\n",id,header_id);
    }

    file_size -= header_size;
    // Read all from file into the schedule buffer
    ehs_uint32 read_size = EhsFread(schedule, sizeof(ehs_uint8), file_size, fp);
    EhsFclose(fp);
    if (read_size != file_size) {
        return 4;
    }
    *pfile_size = file_size;

    return 0;
}

 /**
  * Write the scheduler into file with given schedule ID
  * Return code:
  *  0 Success
  *  1 Schedule buffer is NULL
  *  2 Schedule ID out of range
  *  3 Schedule file open error
  *  4 Schedule size is insufficient to be written
  *  5 Schedule write size does not match
  */
ehs_uint8 target_writeScheduleIntoFile(ehs_uint8* schedule, ehs_uint32 size, ehs_sint32 id, const ehs_char* mode)
{
    if (schedule == NULL) return 1;
    if (id > CONFIG_SCHEDULE_MAX) return 2;
    ehs_char filename[20] = {0};
    EhsSprintf(filename, "_sched%d.scd", id);
    ehs_FILE *fp = Ehs_UserFopen(filename, mode);
    if (fp == NULL) return 3;
    if (CONFIG_MAX_SCHEDULE_SIZE_BYTES+CONFIG_SCHEDULE_HEADER_SIZE < size)
    {
        EhsFclose(fp);
        return 4;
    }
    ehs_uint32 write_size = EhsFwrite(schedule, sizeof(ehs_uint8), size, fp);
    EhsFclose(fp);
    if (write_size != size) return 5;
    return 0;
}

/**
 * Find the slot from the schdule buffer in inxScheduleV1_t format
 * */
inxScheduleV1_t* inxScheduleFindSlot(ehs_uint8* schedule, size_t size, const ehs_uint16 minutesFromSunday)
{
    inxScheduleV1_t * pSlot = (inxScheduleV1_t *)NULL;
    inxScheduleV1_t	* pSlotCheck = (inxScheduleV1_t *)NULL;
    int i= 0;
    for (i = 0; i < size; i = i + sizeof(inxScheduleV1_t))
    {
        pSlotCheck = (inxScheduleV1_t*)&schedule[i];
        if (pSlotCheck->minutesFromSunday == 0 && pSlotCheck->value == 0) break;
        else if (pSlotCheck->minutesFromSunday <= minutesFromSunday) {
            pSlot = pSlotCheck;
        }
    }
    return pSlot;
}

/**
 * Get the temperature from the schedule. Return Error code:
 * 0 Success
 * 1 Schedule type not recognised
 * 2 The input time is out of range
 */
int getTempFromSchedule(ehs_uint8* schedule, size_t size, ehs_sint32 weekday, ehs_sint32 seconds, ehs_sint32 *newTemp, ehs_bool* isOn)
{
    int ret = 0;

    int day = weekday % 7;
    int hour = seconds / 3600;
    int minute = (seconds / 60) % 60;

    int slot = (day * 24) + hour;
    int minutesFromSunday = (slot * 60) + minute;

    ehs_sint32 newTemperature = 0;
    ehs_uint8 value = 0;
    inxScheduleV1_t * pSlot = (inxScheduleV1_t *)NULL;
    if (slot < CONFIG_HOURS_IN_WEEK)
    {
        // INX_SCHEDULE_TYPE_V1 - @TODO update to V3
        pSlot = inxScheduleFindSlot(schedule, size, minutesFromSunday);
        if(pSlot){
            // @TODO - in v3 we no longer need to store value in 5-bits
            value = pSlot->value & 31; // The last 5 bits are value
            // Max value is assumed to be off
            if (value >= 31){ 
                newTemperature = 0;
                *isOn = EHS_FALSE;
            }else{ 
                newTemperature = value;
                *isOn = EHS_TRUE;
            }
        }
    }else{
        ret = 2;
    }

    *newTemp =  newTemperature;
    return ret;
}

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(DevmanScheduler)
EHS_FB_FUNCTION_ENTRY("init_schedule", 0x01, DevmanScheduler_init_schedule)
EHS_FB_FUNCTION_ENTRY("get_schedule", 0x02, DevmanScheduler_get_schedule)
EHS_FB_FUNCTION_ENTRY("set_schedule", 0x03, DevmanScheduler_set_schedule)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_DevmanScheduler_ARG_init_schedule_crc 1
#define INX_DevmanScheduler_ARG_init_schedule_init_done 1
#define INX_DevmanScheduler_ARG_get_schedule_timeOfDayInMinutes 1
#define INX_DevmanScheduler_ARG_get_schedule_dayOfWeek 2
#define INX_DevmanScheduler_ARG_get_schedule_value 1
#define INX_DevmanScheduler_ARG_get_schedule_errno 2
#define INX_DevmanScheduler_ARG_get_schedule_is_on 3
#define INX_DevmanScheduler_ARG_get_schedule_changed 4
#define INX_DevmanScheduler_ARG_get_schedule_OK 1
#define INX_DevmanScheduler_ARG_get_schedule_error 2
#define INX_DevmanScheduler_ARG_set_schedule_binary_data 1
#define INX_DevmanScheduler_ARG_set_schedule_data_size 2
#define INX_DevmanScheduler_ARG_set_schedule_crc 1
#define INX_DevmanScheduler_ARG_set_schedule_set_errono 2
#define INX_DevmanScheduler_ARG_set_schedule_set_OK 1
#define INX_DevmanScheduler_ARG_set_schedule_error_set 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(DevmanScheduler)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_DevmanScheduler_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(DevmanScheduler)
{
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_DevmanScheduler_state_type* inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    const char* pParams = EHS_FB_INIT_PARAMETERS;
    pParams = EhsGetUint32FromString(&(inx_DevmanScheduler_state->channelID), pParams);
    pParams = EhsGetUint8FromString(&(inx_DevmanScheduler_state->autoMode), pParams);
    inx_DevmanScheduler_state->version = 0;
    inx_DevmanScheduler_state->crc32 = 0;
    inx_DevmanScheduler_state->pendingChunks = 0;
    inx_DevmanScheduler_state->dataSize = 0;
    EhsMemset(inx_DevmanScheduler_state->data, 0, CONFIG_MAX_SCHEDULE_SIZE_BYTES);
    inx_DevmanScheduler_state->dataSize = 0;
    inx_DevmanScheduler_state->isOn = EHS_FALSE;
    inx_DevmanScheduler_state->written = EHS_FALSE;

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(DevmanScheduler)
{
    //inx_DevmanScheduler_state_type *inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_DESTROY_CONTEXT;
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION init_schedule MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanScheduler_init_schedule.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanScheduler_init_schedule)
{
	inx_DevmanScheduler_state_type* inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_uint8 read_errno = target_readScheduleFromFile(
        inx_DevmanScheduler_state,
        &inx_DevmanScheduler_state->data[0],
        &inx_DevmanScheduler_state->dataSize,
        inx_DevmanScheduler_state->channelID
    );
    if (read_errno != 0)
    {
        EhsMemset(inx_DevmanScheduler_state->data, 0, CONFIG_MAX_SCHEDULE_SIZE_BYTES);
        inx_DevmanScheduler_state->written = EHS_FALSE;
        EHSH_LOG_ERROR("Scheduler - failed to initalised (%d)\n", read_errno);
    }
    else
    {
        inx_DevmanScheduler_state->written = EHS_TRUE;
    }

    if(inx_DevmanScheduler_state->written == EHS_TRUE){
        if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_init_schedule_crc)){
		    EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_init_schedule_crc) = inx_DevmanScheduler_state->crc32;
        }
        EHS_FB_FINISH(INX_DevmanScheduler_ARG_init_schedule_init_done);
    }

}//ICB FUNCTION init_schedule MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION get_schedule MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanScheduler_get_schedule.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanScheduler_get_schedule)
{
    inx_DevmanScheduler_state_type* inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_sint32 timeOfDay = 0;
    ehs_sint32 weekday = 0;
    ehs_sint32 newTemp = 0;
    ehs_bool isOn = EHS_FALSE;
    ehs_sint32 errno_ = -1;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_timeOfDayInMinutes))
    {
        timeOfDay = EHS_FB_IN_I_API2(INX_DevmanScheduler_ARG_get_schedule_timeOfDayInMinutes) ;
    }
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_dayOfWeek))
    {
        weekday = EHS_FB_IN_I_API2(INX_DevmanScheduler_ARG_get_schedule_dayOfWeek) ;
    }
    if (inx_DevmanScheduler_state->written == EHS_TRUE)
    {
        errno_ = getTempFromSchedule(
            inx_DevmanScheduler_state->data,
            inx_DevmanScheduler_state->dataSize,
            weekday, timeOfDay, &newTemp, &isOn);
    }
    
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_errno))
    {
        EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_get_schedule_errno) = errno_;
    }
    if (errno_ == 0) {
        if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_value))
        {
            EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_get_schedule_value) = newTemp;
        }
        if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_is_on))
        {
            EHS_FB_OUT_B_API2(INX_DevmanScheduler_ARG_get_schedule_is_on) = isOn;
        }
        if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_changed)) 
        {
            EHS_FB_OUT_B_API2(INX_DevmanScheduler_ARG_get_schedule_changed) = 
                (inx_DevmanScheduler_state->temperature != newTemp || inx_DevmanScheduler_state->isOn != isOn) ? EHS_TRUE : EHS_FALSE;
        }
        if (inx_DevmanScheduler_state->written == EHS_TRUE)
        {
            inx_DevmanScheduler_state->temperature = newTemp;
            inx_DevmanScheduler_state->isOn = isOn;
        }
        EHS_FB_FINISH(INX_DevmanScheduler_ARG_get_schedule_OK);
        //printf("newTemp (%d) : %d (%s)\n", inx_DevmanScheduler_state->channelID,  newTemp, (isOn ? "ON" : "OFF"));
    } else {
        EHS_FB_FINISH(INX_DevmanScheduler_ARG_get_schedule_error);
    }
}//ICB FUNCTION get_schedule MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION set_schedule MACRO START -- DO NOT ALTER
/**
 * Definition of DevmanScheduler_set_schedule.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(DevmanScheduler_set_schedule)
{
    inx_DevmanScheduler_state_type* state = (inx_DevmanScheduler_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_sint32 errno_ = 0;
    ehs_sint32 dataSize = 0;
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_data_size)){
        dataSize = EHS_FB_IN_I_API2(INX_DevmanScheduler_ARG_set_schedule_data_size);
    }

    if (dataSize >= CONFIG_SCHEDULE_HEADER_SIZE && EHS_FB_IN_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_binary_data))
    {
        const ehs_uint8* data = (const ehs_uint8 *)EHS_FB_IN_S_API2(INX_DevmanScheduler_ARG_set_schedule_binary_data);
        const ehs_uint8* payloadData = data;
        // read data header
        ehs_uint8 version = *payloadData++;
        ehs_uint16 crc = ((ehs_uint16)(ehs_uint8)payloadData[1] << 8) | (ehs_uint16)(ehs_uint8)payloadData[0];
        payloadData += 2;
        ehs_uint8 id = *payloadData++;
        ehs_uint8 pending = *payloadData++;
        ehs_uint8 eoh = *payloadData++; // end of header

        // calculate payload data size
        ehs_sint32 payloadSize = dataSize-CONFIG_SCHEDULE_HEADER_SIZE;

        // validate header
        if(version != CONFIG_SCHEDULE_VERSION){
            errno_ = 5; // invalid version
        }
        else if(eoh != 0 || (id < 1 || id > CONFIG_SCHEDULE_MAX) || (crc == 0) || (payloadSize > CONFIG_MAX_SCHEDULE_SIZE_BYTES)){
            // end of header should always be 0.
            // pid id range must be within 1 and CONFIG_SCHEDULE_MAX
            // crc value should not be 0
            // schedule payload size should be less than CONFIG_MAX_SCHEDULE_SIZE_BYTES
            errno_ = 4; // invalid data
        }
        
        // check if header data is valid
        if(errno_ == 0 ){
            // check if this data is for this channel
            if(state->channelID != id){
                // this data is not for this scheduler channel. we just ignore it.
                return;
            }
            
            printf("ver=%u,crc=%u,id=%u,paylen=%d\n",version,crc,id,payloadSize);
            
            ehs_bool bAppend = EHS_FALSE;

            if(state->crc32 == crc && state->pendingChunks > 0){ // do we write pending scheduler data ?
                if(pending == state->pendingChunks){ // done - no more pending data
                    state->pendingChunks = 0;
                }else{
                    // check if we missed any pending data
                    if(pending != state->pendingChunks+1){
                        printf("Scheduler error. missed pending data! curr=(%d),next=(%d)\n",state->pendingChunks,pending);
                        errno_ = 4;
                        state->pendingChunks = 0;
                        state->crc32 = 0;
                        // @TODO - remove bin file
                    }else{
                        state->pendingChunks = pending;
                    }
                }
                bAppend = EHS_TRUE; // we always append data in this case
            }else{
                // do we expect more data to be received ?
                state->pendingChunks = pending;
                state->dataSize = 0;
            }
            if(errno_ == 0){
                state->written = EHS_FALSE;
                state->version = version;
                state->crc32 = crc;
                ehs_bool bDone = (state->pendingChunks==0);
                if(bAppend == EHS_TRUE){
                    printf("append more scheduler data id=(%d) size=(%d)\n", id, payloadSize);
                    ehs_sint32 start = state->dataSize;
                    state->dataSize += payloadSize;
                    if(state->dataSize <= CONFIG_MAX_SCHEDULE_SIZE_BYTES){
                        EhsMemcpy(&state->data[start], payloadData, state->dataSize); // copy payload data to buffer
                        errno_ = target_writeScheduleIntoFile(
                            payloadData, payloadSize, // append only payload data without header
                            state->channelID,
                            "ab"
                        );
                    }else{
                        printf("Scheduler error. No more room in the buffer!\n");
                        errno_ = 4;
                        state->pendingChunks = 0;
                        state->crc32 = 0;
                        // @TODO - remove bin file
                    }
                }else{ // write all data
                    printf("write scheduler data id=(%d) size=(%d)\n", id, dataSize);
                    state->dataSize = payloadSize;
                    EhsMemcpy(&state->data[0], payloadData, state->dataSize); // copy payload data to buffer
                    // write both header and payload to the file
                    errno_ = target_writeScheduleIntoFile(
                        data, dataSize,
                        state->channelID,
                        "wb"
                    );
                }

                if(errno_ == 0){
                    if(bDone == EHS_TRUE){
                        state->written = EHS_TRUE;
                    }else{
                        // we expect more data
                        return;
                    }
                }
            }
        }
        if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_set_errono))
        {
            EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_set_schedule_set_errono) = errno_;
        }
        if(errno_ != 0){
            EHS_FB_FINISH(INX_DevmanScheduler_ARG_set_schedule_error_set);
        }else{
            if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_crc))
            {
                EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_set_schedule_crc) = state->crc32;
            }
            EHS_FB_FINISH(INX_DevmanScheduler_ARG_set_schedule_set_OK);
        }
    }
    
}//ICB FUNCTION set_schedule MACRO END -- DO NOT ALTER THIS LINE
