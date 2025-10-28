#ifndef INXWARE_accel_gyro
#define INXWARE_accel_gyro
#include "inx-component.h"

/*****************************************************
 * Accelometer and Gyroscope
 *****************************************************/
#define INXWARE_FB_ID_accel_gyro 0xF2AA
#define INXWARE_FB_NAME_accel_gyro "accel_gyro"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(accel_gyro); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(accel_gyro); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(accel_gyro); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_EnableFunc); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_DisableFunc); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_ReadAccelFunc); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_ReadGyroFunc); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(accel_gyro)

#endif /* INXWARE_accel_gyro */
