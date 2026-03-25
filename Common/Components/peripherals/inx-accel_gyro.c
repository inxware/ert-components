//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-accel_gyro.h"
#include "hal_accel_gyro.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_accel_gyro_state
{
} inx_accel_gyro_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(accel_gyro)
EHS_FB_FUNCTION_ENTRY("enable",     0x01, accel_gyro_enable)
EHS_FB_FUNCTION_ENTRY("disable",    0x02, accel_gyro_disable)
EHS_FB_FUNCTION_ENTRY("read_accel", 0x03, accel_gyro_read_accel)
EHS_FB_FUNCTION_ENTRY("read_gyro",  0x04, accel_gyro_read_gyro)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_accel_gyro_ARG_enable_enabled   1
#define INX_accel_gyro_ARG_enable_error_id  1
#define INX_accel_gyro_ARG_disable_disabled 1
#define INX_accel_gyro_ARG_disable_error_id 1
#define INX_accel_gyro_ARG_read_accel_done  1
#define INX_accel_gyro_ARG_read_accel_Ax    1
#define INX_accel_gyro_ARG_read_accel_Ay    2
#define INX_accel_gyro_ARG_read_accel_Az    3
#define INX_accel_gyro_ARG_read_gyro_done   1
#define INX_accel_gyro_ARG_read_gyro_Gx     1
#define INX_accel_gyro_ARG_read_gyro_Gy     2
#define INX_accel_gyro_ARG_read_gyro_Gz     3
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
EHS_FB_IDENTIFY_FUNCTION(accel_gyro)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_accel_gyro_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(accel_gyro)
{
    return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(accel_gyro)
{
    EhsTAccelGyroDisable();
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION enable MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(accel_gyro_enable)
{
    ehs_sint32 error_id = EhsTAccelGyroEnable();
    if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_enable_error_id))
        EHS_FB_OUT_I_API2(INX_accel_gyro_ARG_enable_error_id) = error_id;
    if (error_id == EHS_PERIPH_OK)
        EHS_FB_FINISH_API2(INX_accel_gyro_ARG_enable_enabled);
}
//ICB FUNCTION enable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION disable MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(accel_gyro_disable)
{
    EhsTAccelGyroDisable();
    if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_disable_error_id))
        EHS_FB_OUT_I_API2(INX_accel_gyro_ARG_disable_error_id) = 0;
    EHS_FB_FINISH_API2(INX_accel_gyro_ARG_disable_disabled);
}
//ICB FUNCTION disable MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION read_accel MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(accel_gyro_read_accel)
{
    ehs_float x, y, z;
    if (EhsTAccelRead(&x, &y, &z)) {
        if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_read_accel_Ax))
            EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_read_accel_Ax) = x;
        if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_read_accel_Ay))
            EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_read_accel_Ay) = y;
        if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_read_accel_Az))
            EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_read_accel_Az) = z;
        EHS_FB_FINISH_API2(INX_accel_gyro_ARG_read_accel_done);
    }
}
//ICB FUNCTION read_accel MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION read_gyro MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(accel_gyro_read_gyro)
{
    ehs_float x, y, z;
    if (EhsTGyroRead(&x, &y, &z)) {
        if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_read_gyro_Gx))
            EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_read_gyro_Gx) = x;
        if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_read_gyro_Gy))
            EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_read_gyro_Gy) = y;
        if (EHS_FB_OUT_CONNECTED_API2(INX_accel_gyro_ARG_read_gyro_Gz))
            EHS_FB_OUT_F_API2(INX_accel_gyro_ARG_read_gyro_Gz) = z;
        EHS_FB_FINISH_API2(INX_accel_gyro_ARG_read_gyro_done);
    }
}
//ICB FUNCTION read_gyro MACRO END -- DO NOT ALTER THIS LINE
