/*
 * target_gpio.c
 */

#include "globals.h"
#include "target_gpio.h"

#include "hal_file.h"
#include "hal_logger.h"

#define GPIO_PATH_BUFFER_SIZE 512
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define DEFAULT_GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_DIRECTION "/direction"
#define GPIO_VALUE "/value"

/* @brief Linux GPIO using sysfs 
   can configure GPIO and assert values.
*/

/* todo2023 - check if this is going to cause problems being shared across all instances and functions
               assuming for now that if single threaded we are OK? */
/* todo23 - add checks for path lengths - 512? */
/* todo23 - Should we do the exporting of the GPIO here also? We probably should.*/
/* todo2023 - Can we merge this and the linux arm  one - can they both do the export steps like the linux arm version?*/


/* todo2024 - this should use hal_file.h API for file operations */

const ehs_char* get_path_GPIO(ehs_char* gpio_path_buffer, ehs_sint32 gpio, const ehs_char* type)
{
    sprintf(gpio_path_buffer, "%s%d%s", DEFAULT_GPIO_PATH, gpio, type);
    return (const ehs_char*)gpio_path_buffer;
}

ehs_bool set_output_GPIO(ehs_sint32 gpio,ehs_bool intialValue, ehs_bool openDrain)
{
    ehs_bool ret = EHS_TRUE;
    ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE]="";

    // THe process has the write permission to the "export" file
    if (access(GPIO_EXPORT_PATH, W_OK) != -1)
    {
        // The GPIO has not been exported yet (gpioxxx folder does not exist)
        if (access(get_path_GPIO(gpio_path_buffer,gpio, ""), F_OK) == -1)
        {
            int fd_export = open(GPIO_EXPORT_PATH, O_WRONLY);
            ehs_char gpio_str[4];
            sprintf(gpio_str, "%d", gpio);
            int gpio_strlen = (int) strlen(gpio_str);
            if (fd_export == -1)
            {
                ret = EHS_FALSE;
            }
            else
            {
                ret = (write(fd_export, gpio_str, gpio_strlen) == gpio_strlen) ? EHS_TRUE : EHS_FALSE;
                close(fd_export);
            }
        }
    }

    const ehs_char* gpio_direction = get_path_GPIO(gpio_path_buffer,gpio, GPIO_DIRECTION);
    int fd = open(gpio_direction, O_WRONLY);
    if (fd == -1)
    {
        ret = EHS_FALSE;
    }
    else
    {
       /* Apparently we can set to output with an initial values using "high" and "low" instead of 
       "out" https://www.kernel.org/doc/Documentation/gpio/sysfs.txt */
        if (intialValue == EHS_FALSE)
            ret = (write(fd, "low", 3) == 3) ? EHS_TRUE : EHS_FALSE;
        else 
        {
            if (openDrain == EHS_TRUE) ret = (write(fd, "in", 2) == 2) ? EHS_TRUE : EHS_FALSE;
            else ret = (write(fd, "high", 4) == 4) ? EHS_TRUE : EHS_FALSE;
        }
        // try this if initial values are not supported
        if (ret == EHS_FALSE) ret = (write(fd, "out", 3) == 3) ? EHS_TRUE : EHS_FALSE;
        close(fd);
    }
    return ret;
}

ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    ehs_bool ret = EHS_TRUE;
    ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE]="";

    // THe process has the write permission to the "export" file
    if (access(GPIO_EXPORT_PATH, W_OK) != -1)
    {
        // The GPIO has not been exported yet (gpioxxx folder does not exist)
        if (access(get_path_GPIO(gpio_path_buffer,gpio, ""), F_OK) == -1)
        {
            int fd_export = open(GPIO_EXPORT_PATH, O_WRONLY);
            ehs_char gpio_str[4];
            sprintf(gpio_str, "%d", gpio);
            int gpio_strlen = (int) strlen(gpio_str);
            if (fd_export == -1)
            {
                ret = EHS_FALSE;
            }
            else
            {
                ret = (write(fd_export, gpio_str, gpio_strlen) == gpio_strlen) ? EHS_TRUE : EHS_FALSE;
                close(fd_export);
            }
        }
    }

    const ehs_char* gpio_direction = get_path_GPIO(gpio_path_buffer,gpio, GPIO_DIRECTION);
    int fd = open(gpio_direction, O_WRONLY);
    if (fd == -1)
    {
        ret = EHS_FALSE;
    }
    else
    {
        ret = (write(fd, "in", 2) == 2) ? EHS_TRUE : EHS_FALSE;
        close(fd);
    }
    return ret;
}

ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value, ehs_bool openDrain)
{
    ehs_bool ret = EHS_TRUE;
    int fd;
    ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE]="";

    if (openDrain == EHS_TRUE) {
        fd = open(get_path_GPIO(gpio_path_buffer,gpio, GPIO_DIRECTION), O_WRONLY);
    }
    else {
        fd = open(get_path_GPIO(gpio_path_buffer,gpio, GPIO_VALUE), O_WRONLY);
    }

    if (fd == -1)
    {
        ret = EHS_FALSE;
    }
    else
    {
        if (openDrain == EHS_FALSE)
        {
            if (value == EHS_FALSE)
            {
                ret = (write(fd, "0", 1) == 1) ? EHS_TRUE : EHS_FALSE;
            }
            else
            {
                ret = (write(fd, "1", 1) == 1) ? EHS_TRUE : EHS_FALSE;
            }
        }
        else
        {
            if (value == EHS_FALSE)
            {
                ret = (write(fd, "low", 3) == 1) ? EHS_TRUE : EHS_FALSE;
            }
            else
            {
                ret = (write(fd, "in", 2) == 1) ? EHS_TRUE : EHS_FALSE;
            }
        }
        
        close(fd);
    }
    return ret;
}

ehs_bool get_value_GPIO(ehs_sint32 gpio, ehs_bool* value)
{
    if(value == NULL)
    {
        return EHS_FALSE;
    }
    ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE]="";
    ehs_bool ret = EHS_TRUE;
    const ehs_char* gpio_value = get_path_GPIO(gpio_path_buffer,gpio, GPIO_VALUE);
    int fd = open(gpio_value, O_RDONLY);
    if (fd == -1)
    {
        ret = EHS_FALSE;
    }
    else
    {
        int off = lseek(fd, 0, SEEK_SET);
        if(off != -1)
        {
            char data;
            size_t num = read(fd, &data, 1);
            if(num == 1)
            {
                *value = (data == '1') ? EHS_TRUE : EHS_FALSE;
            }
            else
            {
                ret = EHS_FALSE;
            }
        }
        close(fd);
    }
    return ret;
}

ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    ehs_bool ret = set_output_GPIO(pGPIO->pin_id, pGPIO->initial_state, pGPIO->open_drain_mode);
    if(ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialised GPIO output pin (%d)", pGPIO->pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO output pin (%d)", pGPIO->pin_id);
    }
    return ret;
}

/* A rather onerous extra level of indirection to get to the target's GPIO functions. */
ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    ehs_bool ret = set_value_GPIO(pGPIO->pin_id, pGPIO->pin_value, pGPIO->open_drain_mode);
    if(ret == EHS_TRUE)
    {
        //EHSH_LOG_INFO("Write GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to write GPIO (%d,%d).", pGPIO->pin_id, pGPIO->pin_value);
    }
    return ret;
}

ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO)
{
    EHSH_LOG_INFO("Destroy GPIO output pin (%d)", pGPIO->pin_id);
    return EHS_TRUE;
}

ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_bool ret = set_input_GPIO(pGPIO->pin_id);
    if(ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO input pin (%d)", pGPIO->pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO input pin (%d)", pGPIO->pin_id);
    }
    return ret;
}

ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    ehs_bool ret = get_value_GPIO(pGPIO->pin_id, &pGPIO->pin_value);
    if(ret == EHS_TRUE)
    {
        //EHSH_LOG_INFO("Read GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to read GPIO (%d).", pGPIO->pin_id);
    }
    return ret;
}

ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO)
{
    EHSH_LOG_INFO("Destroy GPIO input pin (%d)", pGPIO->pin_id);
    return EHS_TRUE;
}
