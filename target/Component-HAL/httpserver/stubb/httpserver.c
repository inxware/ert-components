#include "globals.h"

static void httpServerRequestGet(char* reqName, char* data)
{
   
}

void cgi_responseFunction(void* responseObj, char* data)
{

}

// We may still want a real version of this because we use it for MQTT too sometimes...
//static EHS_BOOl cgi_get_varval(const char *src, char *var_name, char *dst, uint32_t length)
//{
//   
//    return (ehs_true);
//}

/* Example Server Side Include callback. */
static int ssi_date_time(HTTPSRV_SSI_PARAM_STRUCT *param)
{
    /*if (strcmp(param->com_param, "time") == 0)
    {
        HTTPSRV_ssi_write(param->ses_handle, __TIME__, strlen(__TIME__));
    }
    else if (strcmp(param->com_param, "date") == 0)
    {
        HTTPSRV_ssi_write(param->ses_handle, __DATE__, strlen(__DATE__));
    }*/
    return (0);
}

