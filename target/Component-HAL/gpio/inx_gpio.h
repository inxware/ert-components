#ifndef __EHS_COMPONENT_HAL_GPIO__H
#define __EHS_COMPONENT_HAL_GPIO__H


/* THe following might be used in our code, but probably shouldn't be... Leaving them here for now...*/
#define EHS_PERIPHERALS_GPIO_TYPE_STUBBED 0
#define EHS_PERIPHERALS_GPIO_TYPE_NXP_K64 1
#define EHS_PERIPHERALS_GPIO_TYPE_SYSFS_LINUX_ARM 2
#define EHS_PERIPHERALS_GPIO_TYPE_ESP32_IDF 3
#define EHS_PERIPHERALS_GPIO_TYPE_ESP32S3_IDF 4
#define EHS_PERIPHERALS_GPIO_TYPE_GUI 5
#define EHS_PERIPHERALS_GPIO_TYPE_ARDUINO 6
#define EHS_PERIPHERALS_GPIO_TYPE_SFERALABS 7
#define EHS_PERIPHERALS_GPIO_TYPE_UNKNOWN 1000000


/**
 * @brief Initialise the GPIO globally when the system starts
 * 
 */
void EhsTOsInitGpio( void );

/**
 * @brief De-intiialise the GPIO globally when the system terminates
 * 
 */
void EhsTOsTermGpio( void );


#endif//__EHS_COMPONENT_HAL_GPIO__H