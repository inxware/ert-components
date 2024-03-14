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

#define CONFIG_MAX_SCHEDULE_SIZE_BYTES 4096
#define CONFIG_HOURS_IN_WEEK 168

#define CONFIG_SCHEDULE_MAX 9

typedef struct {
    ehs_uint16 minutesFromSunday;
    ehs_uint8 value;
} inxScheduleV1_t;

/** TODO: implement EhsFseek from fseek
 * Read the scheduler from file with given schedule ID in the schedule array
 * Return code:
 *  0 Success
 *  1 Schedule buffer is NULL
 *  2 Schedule not found
 *  3 Schedule size overflow
 *  4 Schedule read size does not match the file size
 */
ehs_uint8 target_readScheduleFromFile(ehs_uint8** pSchedule, size_t* pfile_size, ehs_sint32 id)
{
    if (pSchedule == NULL || pfile_size == NULL) return 1;
    if (id > CONFIG_SCHEDULE_MAX) return 2;
    ehs_char filename[20] = {0};
    EhsSprintf(filename, "_sched%d.scd", id);
    ehs_FILE *fp = Ehs_UserFopen(filename, "rb");
    if (fp == NULL) return 2;

    // Get the size of the scheduler file
    fseek(fp, 0L, SEEK_END);
    size_t file_size = EhsFtell(fp);
    if (file_size >= CONFIG_MAX_SCHEDULE_SIZE_BYTES)
    {
        EhsFclose(fp);
        return 3;
    }
    fseek(fp, 0, SEEK_SET);

    ehs_uint8* buff = (ehs_uint8*)malloc(file_size * sizeof(ehs_uint8));

    // Read all from file into the schedule buffer
    size_t read_size = EhsFread(buff, sizeof(ehs_uint8), file_size, fp);
    EhsFclose(fp);
    if (read_size != file_size) {
        free(buff);
        return 4;
    }

    *pSchedule = buff;
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
ehs_uint8 target_writeScheduleIntoFile(ehs_uint8* schedule, size_t size, ehs_sint32 id)
{
    if (schedule == NULL) return 1;
    if (id > CONFIG_SCHEDULE_MAX) return 2;
    ehs_char filename[20] = {0};
    EhsSprintf(filename, "_sched%d.scd", id);
    ehs_FILE *fp = Ehs_UserFopen(filename, "wb");
    if (fp == NULL) return 3;
    if (CONFIG_MAX_SCHEDULE_SIZE_BYTES < size)
    {
        EhsFclose(fp);
        return 4;
    }
    size_t write_size = EhsFwrite(schedule, sizeof(ehs_uint8), size, fp);
    EhsFclose(fp);
    if (write_size != size) return 5;
    return 0;
}

/**
 * Find the slot from the schdule buffer in inxScheduleV1_t format
 * */
inxScheduleV1_t* inxScheduleFindSlot(inxScheduleV1_t* schedule, size_t size, const ehs_uint16 minutesFromSunday)
{
    inxScheduleV1_t * pSlot = (inxScheduleV1_t *)NULL;
    inxScheduleV1_t	* pSlotCheck = (inxScheduleV1_t *)NULL;
    int i= 0;
    for (i = 0; i < size; i = i + sizeof(inxScheduleV1_t))
    {
        pSlotCheck = &schedule[i];
        if (pSlotCheck->minutesFromSunday == 0) break;
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
int getTempFromSchedule(ehs_uint8* schedule, size_t size, ehs_sint32 weekday, ehs_sint32 timeofday, ehs_sint32 *newTemp)
{
    int ret = 0;
    
    int slot = (weekday * 24) + timeofday / 3600;
    int minutesFromSunday = (slot * 60) + timeofday / 60;
    ehs_sint32 newTemperature = 0;
    ehs_uint8 value = 0;
    inxScheduleV1_t * pSlot = (inxScheduleV1_t *)NULL;
    if (slot < CONFIG_HOURS_IN_WEEK && slot < size)
    {
        pSlot = (inxScheduleV1_t *)&schedule[0];
        switch (schedule[0]) {
        case 0: // INX_SCHEDULE_TYPE_DEFAULT
            newTemperature = (float) schedule[slot];
            break;
        case 1: // INX_SCHEDULE_TYPE_V1
            pSlot = inxScheduleFindSlot((inxScheduleV1_t *)schedule, size, minutesFromSunday);
            value = pSlot->value & 31; // The last 5 bits are value
            // Max value is assumed to be off
            if (value >= 31) newTemperature = 0;
            else newTemperature = (float)(value * 5);
            break;
        default:
            ret = 1;
            break;
        }
    }else{
        ret = 2;
    }

    *newTemp =  newTemperature;
    return ret;
}


//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_DevmanScheduler_state
{
    ehs_sint32 ChannelID;
    ehs_bool AutoMode;
    ehs_bool written;
    ehs_uint8* pData;
    ehs_uint8 dataSize;
    ehs_sint32 temperature; // Stored temperature
} inx_DevmanScheduler_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(DevmanScheduler)
EHS_FB_FUNCTION_ENTRY("get_schedule", 0x01, DevmanScheduler_get_schedule)
EHS_FB_FUNCTION_ENTRY("set_schedule", 0x02, DevmanScheduler_set_schedule)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_DevmanScheduler_ARG_get_schedule_timeOfDayInMinutes 1
#define INX_DevmanScheduler_ARG_get_schedule_dayOfWeek 2
#define INX_DevmanScheduler_ARG_get_schedule_value 1
#define INX_DevmanScheduler_ARG_get_schedule_changed 2
#define INX_DevmanScheduler_ARG_get_schedule_errno 3
#define INX_DevmanScheduler_ARG_get_schedule_OK 1
#define INX_DevmanScheduler_ARG_get_schedule_error 2
#define INX_DevmanScheduler_ARG_set_schedule_binary_data 1
#define INX_DevmanScheduler_ARG_set_schedule_data_size 2
#define INX_DevmanScheduler_ARG_set_schedule_set_errono 1
#define INX_DevmanScheduler_ARG_set_schedule_set_OK 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_DevmanScheduler_ChannelID 
#define INX_FB_DevmanScheduler_AutoMode 
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
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
    ehs_sint32 ChannelID;
    ehs_bool AutoMode;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d",&ChannelID,&AutoMode); */
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
    printf("DEVMANSCHEDULER\n\n");
    ehs_sint32 ChannelID;
    ehs_bool AutoMode;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_DevmanScheduler_state_type* inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters */
    EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %d",&ChannelID,&AutoMode);
    inx_DevmanScheduler_state->pData = NULL;
    inx_DevmanScheduler_state->dataSize = -1;
    inx_DevmanScheduler_state->ChannelID = ChannelID;
    inx_DevmanScheduler_state->AutoMode = AutoMode;
    ehs_uint8 read_errno = target_readScheduleFromFile(
        &inx_DevmanScheduler_state->pData,
        &inx_DevmanScheduler_state->dataSize,
        inx_DevmanScheduler_state->ChannelID
    );
    if (read_errno != 0)
    {
        inx_DevmanScheduler_state->written = EHS_FALSE;
    }
    else
    {
        inx_DevmanScheduler_state->written = EHS_TRUE;
    }

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(DevmanScheduler)
{
    inx_DevmanScheduler_state_type *inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_DESTROY_CONTEXT;
    if(inx_DevmanScheduler_state->pData != NULL){
        free(inx_DevmanScheduler_state->pData);
        inx_DevmanScheduler_state->pData = NULL;
    }
    //Your code below here
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
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
            inx_DevmanScheduler_state->pData,
            inx_DevmanScheduler_state->dataSize,
            weekday, timeOfDay, &newTemp);
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_value))
    {
        EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_get_schedule_value) = newTemp;
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_changed)) 
    {
        EHS_FB_OUT_B_API2(INX_DevmanScheduler_ARG_get_schedule_changed) = 
            inx_DevmanScheduler_state->temperature != newTemp ? EHS_TRUE : EHS_FALSE;
    }
    if (inx_DevmanScheduler_state->written == EHS_TRUE)
    {
        inx_DevmanScheduler_state->temperature = newTemp;
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_get_schedule_errno))
    {
        EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_get_schedule_errno) = errno_;
    }
    if (errno_ == 0) {
        EHS_FB_FINISH(INX_DevmanScheduler_ARG_get_schedule_OK);
    } else {
        EHS_FB_FINISH(INX_DevmanScheduler_ARG_get_schedule_error);
    }
    printf("newTemp: %d\n", newTemp);
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
    inx_DevmanScheduler_state_type* inx_DevmanScheduler_state = (inx_DevmanScheduler_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_sint32 errno_ = -1;
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_data_size)){
        inx_DevmanScheduler_state->dataSize = EHS_FB_IN_I_API2(INX_DevmanScheduler_ARG_set_schedule_data_size);
    }

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_binary_data) && inx_DevmanScheduler_state->dataSize > 0)
    {
        errno_ = target_writeScheduleIntoFile(
            (ehs_uint8 *)EHS_FB_IN_S_API2(INX_DevmanScheduler_ARG_set_schedule_binary_data),
            inx_DevmanScheduler_state->dataSize,
            inx_DevmanScheduler_state->ChannelID
        );
        if (errno_ == 0)
        {
            if(inx_DevmanScheduler_state->pData != NULL){
                free(inx_DevmanScheduler_state->pData);
            }
            inx_DevmanScheduler_state->pData = (ehs_uint8*)malloc(inx_DevmanScheduler_state->dataSize * sizeof(ehs_uint8));
            EhsMemcpy(
                inx_DevmanScheduler_state->pData,
                (ehs_uint8 *)EHS_FB_IN_S_API2(INX_DevmanScheduler_ARG_set_schedule_binary_data),
                inx_DevmanScheduler_state->dataSize * sizeof(ehs_uint8)
            );
            inx_DevmanScheduler_state->written = EHS_TRUE;
        }
    }else{
        // @todo - report bad data size
    }
    if (EHS_FB_OUT_CONNECTED_API2(INX_DevmanScheduler_ARG_set_schedule_set_errono))
    {
        EHS_FB_OUT_I_API2(INX_DevmanScheduler_ARG_set_schedule_set_errono) = errno_;
    }
    EHS_FB_FINISH(INX_DevmanScheduler_ARG_set_schedule_set_OK);
}//ICB FUNCTION set_schedule MACRO END -- DO NOT ALTER THIS LINE
