#include "target_gpio.h"

/**
 * @brief Initialise the GPIO globally when the system starts
 * 
 */
void EhsTOsInitGpio( void )
{
   // gpioInitialise(); @xiasheng needs to sort this out? where is this???
   // gGPIOInitialised = EHS_TRUE; - this is usually done by the target if needed ??/
}

/**
 * @brief De-intiialise the GPIO globally when the system terminates
 * 
 */
void EhsTOsTermGpio( void )
{
    // gpioTerminate();
    // gGPIOInitialised = EHS_FALSE; - this is usually done by the target if needed ??/
}

#if 0
/**
 * @brief Initialise the GPIO globally when the system starts
 * 
 */
void EhsTOsInitGpio( void )
{
}

/**
 * @brief De-intiialise the GPIO globally when the system terminates
 * 
 */
void EhsTOsTermGpio( void )
{
}
#endif


