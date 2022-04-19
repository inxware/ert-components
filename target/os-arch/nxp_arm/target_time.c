#include <errno.h>
#include "target_types.h"
#include "timer.h"
#include "heatrod_config.h"

/**
 * This represents the soonest time that any timer will expire
 */
EHS_LOCAL EhsTickType EhsTgtTimerExpiryTime;

EhsTickType EhsTgtTimer_now()
{
	/*EhsTickType ret;
    ehs_uint32 s = ;
	if (s <= 3)
	{
		EhsError(EHS_TGT_ERROR_TIME_FAILURE(errno));
		ret = EHS_TICKTYPE_INVALID;
	}
	else
	{
		ehs_uint32 us =0;
		ret = ( (((ehs_uint32) EHS_TIME_s(s) & EHS_TICKTYPE_MAX) + (((ehs_uint32) EHS_TIME_us(us))) &  EHS_TICKTYPE_MAX) );
	}*/
	return inxEHSGetUSeconds();
}

/**
 * Initialise the target-specific timer (to be not running)
 */
void EhsTgtTimer_reset()
{
	EhsTgtTimer_clear();
}

/*
 * Set the target-specific timer to expire at a specific time.
 * Upon expiry (or periodically if the target's timing facility
 * is unsophisticated), call EhsTimer_tick()
 */
void EhsTgtTimer_set(EhsTickType tExpiryTime)
{
	EhsTgtTimerExpiryTime = tExpiryTime;
}

/**
 * Stop the target-specific timer from calling EhsTimer_tick(), cancel
 * any pending events
 */
void EhsTgtTimer_clear()
{
	EhsTgtTimerExpiryTime = EHS_TICKTYPE_INVALID;
}

/**
 * Report expiry time for current timer
 *
 * @return the time when the timer will call EhsTimer_tick, or EHS_TICKTYPE_INVALID
 * if no timer is running
 */
EhsTickType EhsTgtTimer_expiry()
{
	return EhsTgtTimerExpiryTime;
}

/**
 * This is only needed if we've gone for the application polling method.
 * @returns it *must* return true.
 */
ehs_bool EhsTgtTimer_tick()
{
	//printf("PBB EhsTgtTimer_tick:\n target=%d\t time=%d\n",EhsTgtTimerExpiryTime,EHS_CURRENT_TIME);
	if (EhsTgtTimerExpiryTime != EHS_TICKTYPE_INVALID)
	{
		if (EHS_TARGET_TIME_IS_EARLIER(EhsTgtTimerExpiryTime,EHS_CURRENT_TIME))
		//if (EhsTimer_earlier(EhsTgtTimerExpiryTime,EHS_CURRENT_TIME))
		{
			EhsTimer_tick();
		}
	}
	return EHS_TRUE;
}

/**
 * Sleep the current thread for a specified duration
 *
 * @param tSleepTime Time to sleep for in target-specific ticks
 */
void EhsSleep(EhsTickType tSleepTime)
{
	vTaskDelay(tSleepTime);
}

void EhsSleepUs(ehs_uint32 tSleepTime){
	EhsSleep(tSleepTime/1000);
}