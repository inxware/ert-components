
#include "utils/tflite_utils.h"

#include <stddef.h>
#include <sys/time.h>



unsigned long TfLiteTimeNow_ms() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL);
}
