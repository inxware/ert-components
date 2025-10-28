#include "hal_pwm.h"
#include "inx_pwm_port.h"

//TODO2025: this code looks needlessly complex, uses malloc and not really sure why it is so different to other GPIO code.
// we could do with some vague explaination of wht is being attempted with this - Perhaps have a look at docs...
// and should really be part of the ./Common/Components/ code?  It is only for the PWM block so could even go in there? 
// It's also got lots of mallocs in it and doesn't use the bastracted mallocs
// Does it do mallocs only at init or all over the place?

//todo2025 we don't need these surely for PWM:
#include <stddef.h>
#include <stdlib.h>

/* TODO2025 - This code needs reviewing it has mallocs just for doing PWM? 
   - Does it only do this on init?  If not prolly needs re-writing / decomplicating 
*/

/* todo: whY ON EARTH DO WE DO THINGS LIKE THIS? */
#define INX_HWPWM_CHECK_RETURN(f) \
    ret = f; \
    if (ret != 0) return ret;

#define SW_PWM_FREQ_MAX 1000000

/*
* Implementing circular singular linked list. The start node pointer is stored in every node.
*/
static inx_hw_pwm_t *gpHwPwmListHead = NULL;
static inx_hw_pwm_t *gpHwPwmListTail = NULL;
static int gHwPwmListSize = 0;

static inx_hw_pwm_t* searchHwPwmObj(ehs_sint32 channel)
{
    int i = 0;
    inx_hw_pwm_t *current = gpHwPwmListHead;
    if (current == NULL) return NULL;
    for (i = 0; i < gHwPwmListSize ; i++)
    {
        if (current->channel == channel) return current;
        current = current->pNext;
    }
   return NULL;
}

static inx_hw_pwm_t* getLastHwPwmObj( void )
{
    inx_hw_pwm_t *current = gpHwPwmListHead;
    if (current == NULL) return NULL;
    do {
        current = current->pNext;
    } while (gpHwPwmListHead == current);
    return current;
}

static inx_hw_pwm_t* insertHwPwmObjToList(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max, ehs_bool hw)
{
    inx_hw_pwm_t *pNewObj = (inx_hw_pwm_t *) malloc(sizeof(inx_hw_pwm_t));
    pNewObj->pPrev = NULL;
    pNewObj->pNext = NULL;
    pNewObj->channel = channel;
    pNewObj->io_num = io_num;
    pNewObj->max_val = max;
    pNewObj->hw = hw;
    pNewObj->sw_timers[0].tPeriod = SW_PWM_FREQ_MAX / freq;
    pNewObj->sw_timers[1].tPeriod = SW_PWM_FREQ_MAX / freq;
    pNewObj->sw_timer_delay_triggered = 0;
    pNewObj->pStatus = (inx_hw_pwm_status_t *) malloc(sizeof(inx_hw_pwm_status_t));
    pNewObj->pStatus->freq = freq;
    pNewObj->pStatus->duty = 0;
    pNewObj->pStatus->enabled = 0;
    if (gpHwPwmListHead == NULL)
    {
        gpHwPwmListHead = pNewObj;
        gpHwPwmListTail = pNewObj;
        pNewObj->pPrev = pNewObj;
        pNewObj->pNext = pNewObj;
    }
    else {
        gpHwPwmListTail->pNext = pNewObj;
        pNewObj->pPrev = gpHwPwmListTail;
        gpHwPwmListTail = pNewObj;
        gpHwPwmListTail->pNext = gpHwPwmListHead;
        gpHwPwmListHead->pPrev = gpHwPwmListTail;
    }
    gHwPwmListSize++;

    return pNewObj;
}

static void removeFirstHwPwmObjFromList( void )
{
    inx_hw_pwm_t *temp = NULL;
    if (gpHwPwmListHead == NULL) return;
    if (gpHwPwmListHead == gpHwPwmListTail)
    {
        free(gpHwPwmListHead->pStatus);
        free(gpHwPwmListHead);
        gpHwPwmListHead = NULL;
        gpHwPwmListTail = NULL;
    }
    else {
        temp = gpHwPwmListHead;
        gpHwPwmListHead = gpHwPwmListHead->pNext;
        gpHwPwmListHead->pPrev = gpHwPwmListTail;
        gpHwPwmListTail->pNext = gpHwPwmListHead;
        free(temp->pStatus);
        free(temp);
    }
    gHwPwmListSize--;
}

static void removeLastHwPwmObjFromList( void )
{
    inx_hw_pwm_t *temp = NULL;
    if (gpHwPwmListHead == NULL) return;
    if (gpHwPwmListHead ==  gpHwPwmListTail)
    {
        free(gpHwPwmListHead->pStatus);
        free(gpHwPwmListHead);
        gpHwPwmListHead = NULL;
        gpHwPwmListTail = NULL;
    }
    else {
        temp = gpHwPwmListTail;
        gpHwPwmListTail = gpHwPwmListTail->pPrev;
        gpHwPwmListTail->pNext = gpHwPwmListHead;
        gpHwPwmListHead->pPrev = gpHwPwmListTail;
        free(temp->pStatus);
        free(temp);
    }
    gHwPwmListSize--;
}

static void removeHwPwmObjFromList(inx_hw_pwm_t *obj)
{
    if (obj == NULL) return;
    if (obj == gpHwPwmListHead)
    {
        removeFirstHwPwmObjFromList();
        return;
    }
    if (obj == gpHwPwmListTail)
    {
        removeLastHwPwmObjFromList();
        return;
    }
    obj->pPrev->pNext = obj->pNext;
    obj->pNext->pPrev = obj->pPrev;
    free(obj->pStatus);
    free(obj);
    gHwPwmListSize--;
}

static void removeSearchHwPwmObjFromList(ehs_sint32 channel)
{
    inx_hw_pwm_t *pSearch = searchHwPwmObj(channel);
    if (pSearch != NULL) removeHwPwmObjFromList(pSearch);
}

static void removeAllHwPwmObjList( void )
{
    inx_hw_pwm_t *temp = gpHwPwmListHead, *temp_next = NULL;
    int i = 0;
    if (temp == NULL) return;
    for (i = 0 ; i < gHwPwmListSize ; i++)
    {
        temp_next = temp->pNext;
        free(temp->pStatus);
        free(temp);
        temp = temp_next;
    }
    gHwPwmListSize = 0;
    gpHwPwmListHead = NULL;
    gpHwPwmListTail = NULL;
}

ehs_sint32 EhsTPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val, ehs_bool hw, inx_hw_pwm_t **obj)
{
    int ret = 0;
    if (obj == NULL) return -1;
    if (searchHwPwmObj(channel) != NULL) return -2;
    if (!hw && freq > SW_PWM_FREQ_MAX) return -3;
    if (hw) ret = EhsTPortPwmSetup(channel, io_num, freq, max_val);
    if (ret == 0) *obj = insertHwPwmObjToList(channel, io_num, freq, max_val, hw);
    return ret;
}

//TODO Add Software PWM support with the EhsTimerxxx functions
ehs_sint32 EhsTPwmDeinit(inx_hw_pwm_t *obj)
{
    int ret = 0;
    if (obj == NULL) return -1;
    if (obj->hw) ret = EhsTPortPwmDeinit(obj->channel);
    else
    {
        ///
    }
    if (ret == 0) removeHwPwmObjFromList(obj);
    return ret;
}

ehs_sint32 EhsTPwmSetFreq(inx_hw_pwm_t *obj, ehs_sint32 freq)
{
    int ret = 0;
    if (obj == NULL) return -1;
    ret = EhsTPortPwmSetFreq(obj->channel, freq);
    if (ret != 0) return ret;
    obj->pStatus->freq = freq;
    ret = EhsTPortPwmSetMaxValue(obj->channel, freq, obj->max_val);
    if (ret != 0) return ret;
    ret = EhsTPortPwmGetMaxValue(obj->channel, &obj->max_val);
    return ret;
}

ehs_sint32 EhsTPwmSetDuty(inx_hw_pwm_t *obj, ehs_uint32 duty)
{
    int ret = 0;
    if (obj == NULL) return -1;
    if (duty > obj->max_val) return -2;
    ret = EhsTPortPwmSetDuty(obj->channel, duty);
    if (ret == 0) obj->pStatus->duty = duty;
    return ret;
}

ehs_sint32 EhsTPwmSetMaxRes(inx_hw_pwm_t *obj, ehs_uint32 duty)
{
    int ret = 0;
    ehs_sint32 max_val = 0;
    ehs_sint32 real_duty = 0;
    if (obj == NULL) return -1;
    // Cannot set the max value to be 0 or less than current duty
    if (duty == 0 || duty < obj->pStatus->duty) return -2;
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmSetMaxValue(obj->channel, obj->pStatus->freq, (ehs_sint32)duty));
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmGetMaxValue(obj->channel, &duty));
    obj->max_val = duty;
    return EhsTPwmSetDuty(obj, obj->pStatus->duty);
}

ehs_sint32 EhsTPwmEnable( inx_hw_pwm_t *obj )
{
    int ret = 0;
    if (obj == NULL) return -1;
    ret = EhsTPortPwmEnable(obj->channel);
    if (ret == 0) obj->pStatus->enabled = EHS_TRUE;
    return ret;
}

ehs_sint32 EhsTPwmDisable( inx_hw_pwm_t *obj )
{
    int ret = 0;
    if (obj == NULL) return -1;
    ret = EhsTPortPwmDisable(obj->channel);
    if (ret == 0) obj->pStatus->enabled = EHS_FALSE;
    return ret;
}

ehs_sint32 EhsTPwmSetStatus(inx_hw_pwm_t *obj, inx_hw_pwm_status_t status)
{
    int ret = 0;
    if (obj == NULL) return -1;
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmSetFreq(obj->channel, status.freq));
    obj->pStatus->freq = status.freq;
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmSetDuty(obj->channel, status.duty));
    obj->pStatus->duty = status.duty;
    if (status.enabled == EHS_FALSE)
    {
        INX_HWPWM_CHECK_RETURN(EhsTPortPwmDisable(obj->channel));
        obj->pStatus->enabled = EHS_FALSE;
    }
    else {
        INX_HWPWM_CHECK_RETURN(EhsTPortPwmEnable(obj->channel));
        obj->pStatus->enabled = EHS_TRUE;
    }
    return 0;
}

ehs_sint32 EhsTPwmGetStatus(inx_hw_pwm_t *obj, inx_hw_pwm_status_t *pStatus)
{
    int ret = 0;
    if (obj == NULL) return -1;
    if (pStatus == NULL) return -2;
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmGetFreq(obj->channel, &(obj->pStatus->freq)));
    pStatus->freq = obj->pStatus->freq;
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmGetDuty(obj->channel, &(obj->pStatus->duty)));
    pStatus->duty = obj->pStatus->duty;
    INX_HWPWM_CHECK_RETURN(EhsTPortPwmEnabled(obj->channel, &(obj->pStatus->enabled)));
    pStatus->enabled = obj->pStatus->enabled;
    return 0;
}
