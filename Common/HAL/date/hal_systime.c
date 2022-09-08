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

/* This must be run as root
 * this is patform dependent
*/
ehs_bool EhsHSetDateTime(ehs_char * date_string)
{
    //@todo complete:
    /*Needs to be completed in target time code (which should be OS specific (not gnu).
     * Use  #include <sys/time.h>

    int settimeofday (struct timeval *tp,
                  struct timezone *tzp);
     *
     *
     *more at
     *http://publib.boulder.ibm.com/infocenter/iseries/v5r4/index.jsp?topic=%2Fapis%2Fsettod.htm
     *May also  need to run exec( hwclock --systoc stuff to synchronise HW clock). Check for ld.so issue. if this needs applies.
     */
    return EHS_TRUE;
}

/* Returns an arithmetic type time.
 * Warning any function converting the time to 32bit uints will meet the 2038 problem.
 * If converted to signed ints the problem is in
 * */


ehs_uint64 EhsHGetdateTime(ehs_char* sZtemp, ehs_bool local)
{
    struct tm *tm;
    time_t xtimet;
    time(&xtimet); // Convert to w3c "%Y-%m-%dT%H:%M:%S", date_temp) //
    if (local == EHS_TRUE)
        tm = localtime(&xtimet);
    else
        tm = gmtime(&xtimet);/* todo - use _r version - tm is a static structure - overwritten by gmtime (nasty!) */

    EhsSprintf(sZtemp, "%02d-%02d-%02dT%02d:%02d:%02d", (tm->tm_year) % 100,
               tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);//, tm.tm_wday, tm.tm_yday);
    return (ehs_uint64) xtimet;
}


