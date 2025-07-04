/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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
#if defined(EHS_MINGW) || defined(EHS_NXP_SUPPORT) || defined(EHS_ARDUINO_SUPPORT)
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
 *  client trust:
 *  it is crash tolerant to null string inputs.
*/
ehs_bool EhsHSetDateTime(ehs_char * date_string, ehs_uint32 unix_time, ehs_char * time_zone, ehs_uint32 *time_sec)

{
    /* Only ste the time-zone if we have a valid parameter as we don't always care about time zones*/
    
    if (time_zone != NULL && time_zone[0] != '\0' ) {
    /**
    * Set the timezone.According to https://randomnerdtutorials.com/esp32-ntp-timezones-daylight-saving/.
    * The timezone string list as shown here: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv.
    */    
//todo use a more specific #def than this (see above!)
#ifndef EHS_NXP_SUPPORT
#ifndef EHS_ARDUINO_SUPPORT
        setenv("TZ", time_zone, 1);
        tzset();
#endif
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
    if (time_sec != NULL) *time_sec = (sec == unix_time || unix_time == 0) ? sec : unix_time;
    return EhsHSetDateTimeSec((sec == unix_time || unix_time == 0) ? sec : unix_time);
}

/* Returns an arithmetic type unix time.
 *  and also the string if it is not null.
 * */
ehs_uint64 EhsHGetdateTime(ehs_char* sZtemp, ehs_uint32 sZtempSize, ehs_bool local, ehs_uint32 format)
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
                strftime(sZtemp, sZtempSize, "%Y %b %d %H:%M:%S %Z", tm);
                break;
            case 2: // Human readable format with weekdays (e.g. Wed 2023 Jun 14 14:21:54 BST)
                strftime(sZtemp, sZtempSize, "%a %Y %b %d %H:%M:%S %Z", tm);
                break;
            case 3: // Human readable format // %d-%0*d-%0*dT%0*d:%0*d:%0*d
                EhsSnprintf(sZtemp, sZtempSize, "%04d-%02d-%02dT%02d:%02d:%02d", (tm->tm_year + 1900), tm->tm_mon+1, tm->tm_mday, 
                                                                     tm->tm_hour, tm->tm_min, tm->tm_sec);
                break;
            case 0:
            default: // for 0 and others, W3C datetime format
                //strftime(sZtemp, sizeof(sZtemp)/sizeof(ehs_char) - 1, "%Y-%m-%dT%H:%M:%S%Z", tm);
                EhsSnprintf(sZtemp, sZtempSize, "%02d-%02d-%02dT%02d:%02d:%02d", (tm->tm_year) % 100,
                            tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);//, tm.tm_wday, tm.tm_yday);
                break;
        }
    }
    return (ehs_uint64) xtimet;
}

/* https://stackoverflow.com/a/58037981 with CC-BY-SA 4.0 */
// Algorithm: http://howardhinnant.github.io/date_algorithms.html
static int days_from_epoch(int y, int m, int d)
{
    y -= m <= 2;
    int era = y / 400;
    int yoe = y - era * 400;                                   // [0, 399]
    int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
    int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;           // [0, 146096]
    return era * 146097 + doe - 719468;
}

/* https://stackoverflow.com/a/58037981 with CC-BY-SA 4.0 */
// It  does not modify broken-down time
static time_t EhsTimegm(struct tm const* t)     
{
    int year = t->tm_year + 1900;
    int month = t->tm_mon;          // 0-11
    if (month > 11)
    {
        year += month / 12;
        month %= 12;
    }
    else if (month < 0)
    {
        int years_diff = (11 - month) / 12;
        year -= years_diff;
        month += 12 * years_diff;
    }
    int days_since_epoch = days_from_epoch(year, month + 1, t->tm_mday);

    return 60 * (60 * (24L * days_since_epoch + t->tm_hour) + t->tm_min) + t->tm_sec;
}

void EhsHDateTimeBreakdown(time_t t, ehs_bool local, ehs_uint32 *year, ehs_uint32 *month, ehs_uint32 *mday, ehs_uint32 *wday, ehs_uint32 *hour, ehs_uint32 *minute, ehs_uint32 *second)
{
    struct tm *tms;
    if (local == EHS_TRUE) tms = localtime(&t);
    else tms = gmtime(&t);
    if (year != NULL)   *year = tms->tm_year;
    if (month != NULL)  *month = tms->tm_mon;
    if (mday != NULL)   *mday = tms->tm_mday;
    if (wday != NULL)   *wday = tms->tm_wday;
    if (hour != NULL)   *hour = tms->tm_hour;
    if (minute != NULL) *minute = tms->tm_min;
    if (second != NULL) *second = tms->tm_sec;
}

/**
 * @brief This returns the local timestamp from unix one
 * 
 * @param t pointer to the unix timestamp
 * @return ehs_uint64 local timestamp
 */
ehs_uint64 EhsHGetLocalTimestampFromUnix(time_t *t)
{
    struct tm *_tms;

    _tms = localtime(t);

    return (ehs_uint64)EhsTimegm(_tms);
}

/**
 * @brief This returns the signed difference between local and unix time (localtime - gmtime)
 * 
 * @param t pointer to the unix timestamp
 * @return ehs_int32 local timestamp - unix timestamp
 */
ehs_sint32 EhsHDiffLocalUnix(time_t *t)
{
    return (ehs_sint32)((ehs_sint64)EhsHGetLocalTimestampFromUnix(t) - (ehs_sint64)(*t));
}