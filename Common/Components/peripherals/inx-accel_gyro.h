#ifndef INXWARE_accel_gyro
#define INXWARE_accel_gyro
#include "inx-component.h"

/*****************************************************
 * Accelerometer and Gyroscope
 *****************************************************/
#define INXWARE_FB_ID_accel_gyro 0xF2AA
#define INXWARE_FB_NAME_accel_gyro "accel_gyro"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(accel_gyro);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(accel_gyro);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(accel_gyro);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_enable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_disable);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_read_accel);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(accel_gyro_read_gyro);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(accel_gyro)

#endif /* INXWARE_accel_gyro */
