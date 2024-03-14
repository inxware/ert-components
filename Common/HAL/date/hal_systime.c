/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


/*
 * Provides date and time utilities for function blocks and system functions
 * 2011 inx limited
 */

/*
 * Writes date string (w3c format) into passed char pointer.
 * @todo add more formats
 */
#include "globals.h"
#include "hal-api.h"
#include <time.h>
//todo2024 - This include shouldn't be in the HAL perhaps? the 'ifndef should be checking for GNU or something more specific to when sys/time.h should be available.
#ifndef EHS_NXP_SUPPORT
    #include <sys/time.h>
#endif
#include <stdlib.h>

/**
 * Set second as the date time.
*/
ehs_bool EhsHSetDateTimeSec(time_t sec)
{
#if defined(EHS_MINGW) || defined(EHS_NXP_SUPPORT)
 //#warning "OS Doesn't  support settimeofday"
#else
    struct timeval tv = {
        .tv_sec = sec,
        .tv_usec = 0
    };
    settimeofday(&tv, NULL);
    return EHS_TRUE;
#endif
    return EHS_FALSE;
}

/* This only works when running from root.

 * The datetime format should be one of the followings:
 *  YYYY/mm/ddTHH:MM:SS
 *  YYYY-mm-ddTHH:MM:SS
 *  YYYY/mm/ddTHH-MM-SS
 *  YYYY-mm-ddTHH-MM-SS
*/
ehs_bool EhsHSetDateTime(ehs_char * date_string, ehs_uint32 unix_time, ehs_char * time_zone)

{
    /* Only ste the time-zone if we have a valid parameter as we don't always care about time zones*/
    
    if (time_zone != NULL && time_zone[0] != '\0' ) {
    /**
    * Set the timezone.According to https://randomnerdtutorials.com/esp32-ntp-timezones-daylight-saving/.
    * The timezone string list as shown here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv.
    */
        
//todo use a more specific #def than this (see above!)
#ifndef EHS_NXP_SUPPORT
        setenv("TZ", time_zone, 1);
        tzset();
#endif
    }
    time_t sec = 0;
    if(date_string != NULL){
        struct tm stm;
        strptime(date_string, "%Y/%m/%dT%H:%M:%S", &stm);
        // In case the date format is YYYY-MM-DD instead of slashes
        strptime(date_string, "%Y-%m-%dT%H:%M:%S", &stm);
        // In case the time format is HH-MM-SS instead of colons
        strptime(date_string, "%Y-%m-%dT%H-%M-%S", &stm);
        sec = mktime(&stm);
    }
    return EhsHSetDateTimeSec((sec == unix_time || unix_time == 0) ? sec : unix_time);
}

/* Returns an arithmetic type time.
 *  and also the string if it is not null.
 * */
ehs_uint64 EhsHGetdateTime(ehs_char* sZtemp, ehs_bool local, ehs_uint32 format)
{
    struct tm *tm;
    time_t xtimet;

    time(&xtimet); // Convert to w3c "%Y-%m-%dT%H:%M:%S", date_temp) //
    if (local == EHS_TRUE)
        tm = localtime(&xtimet);
    else
        tm = gmtime(&xtimet);/* todo - use _r version - tm is a static structure - overwritten by gmtime (nasty!) */
    if (sZtemp != NULL) {
        switch (format) {
            case 1: // Human readable format
                strftime(sZtemp, sizeof(sZtemp)/sizeof(ehs_char) - 1, "%Y %b %d %H:%M:%S %Z", tm);
                break;
            case 2: // Human readable format with weekdays (e.g. Wed 2023 Jun 14 14:21:54 BST)
                strftime(sZtemp, sizeof(sZtemp)/sizeof(ehs_char) - 1, "%a %Y %b %d %H:%M:%S %Z", tm);
                break;
            case 3: // Human readable format // %d-%0*d-%0*dT%0*d:%0*d:%0*d
                EhsSprintf(sZtemp, "%04d-%02d-%02dT%02d:%02d:%02d", (tm->tm_year + 1900), tm->tm_mon+1, tm->tm_mday, 
                                                                     tm->tm_hour, tm->tm_min, tm->tm_sec);
                break;
            case 0:
            default: // for 0 and others, W3C datetime format
                //strftime(sZtemp, sizeof(sZtemp)/sizeof(ehs_char) - 1, "%Y-%m-%dT%H:%M:%S%Z", tm);
                EhsSprintf(sZtemp, "%02d-%02d-%02dT%02d:%02d:%02d", (tm->tm_year) % 100,
                            tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);//, tm.tm_wday, tm.tm_yday);
                break;
        }
    }
    return (ehs_uint64) xtimet;
}

void EhsHDateTimeBreakdown(time_t t, ehs_uint32 *year, ehs_uint32 *month, ehs_uint32 *mday, ehs_uint32 *wday, ehs_uint32 *hour, ehs_uint32 *minute, ehs_uint32 *second)
{
    struct tm *tms = gmtime(&t);
    if (year != NULL)   *year = tms->tm_year;
    if (month != NULL)  *month = tms->tm_mon;
    if (mday != NULL)   *mday = tms->tm_mday;
    if (wday != NULL)   *wday = tms->tm_wday;
    if (hour != NULL)   *hour = tms->tm_hour;
    if (minute != NULL) *minute = tms->tm_min;
    if (second != NULL) *second = tms->tm_sec;
}
