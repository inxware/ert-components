/*
 * target_gpio.c
 */
#include "target.h"
#include "target_gpio.h"
#include "target_config.h"

#define GPIO_PATH_BUFFER_SIZE 512
#define DEFAULT_GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_DIRECTION "/direction"
#define GPIO_VALUE "/value"


ehs_char gpio_path_buffer[GPIO_PATH_BUFFER_SIZE];

const ehs_char* get_path_GPIO(ehs_sint32 gpio, const ehs_char* type)
{
    sprintf(gpio_path_buffer, "%s%d%s", DEFAULT_GPIO_PATH, gpio, type);
    return (const ehs_char*)gpio_path_buffer;
}

ehs_bool set_output_GPIO(ehs_sint32 gpio)
{
    ehs_bool ret = EHS_TRUE;
    const ehs_char* gpio_direction = get_path_GPIO(gpio, GPIO_DIRECTION);
    int fd = open(gpio_direction, O_WRONLY);
    if (fd == -1)
    {
        ret = EHS_FALSE;
    }
    else
    {
        ret = (write(fd, "out", 3) == 3) ? EHS_TRUE : EHS_FALSE;
        close(fd);
    }
    return ret;
}

ehs_bool set_input_GPIO(ehs_sint32 gpio)
{
    ehs_bool ret = EHS_TRUE;
    const ehs_char* gpio_direction = get_path_GPIO(gpio, GPIO_DIRECTION);
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

ehs_bool set_value_GPIO(ehs_sint32 gpio, ehs_bool value)
{
    ehs_bool ret = EHS_TRUE;
    const ehs_char* gpio_value = get_path_GPIO(gpio, GPIO_VALUE);
    int fd = open(gpio_value, O_WRONLY);
    if (fd == -1)
    {
        ret = EHS_FALSE;
    }
    else
    {
        if(value == EHS_FALSE)
        {
            ret = (write(fd, "0", 1) == 1) ? EHS_TRUE : EHS_FALSE;
        }
        else
        {
            ret = (write(fd, "1", 1) == 1) ? EHS_TRUE : EHS_FALSE;
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
    ehs_bool ret = EHS_TRUE;
    const ehs_char* gpio_value = get_path_GPIO(gpio, GPIO_VALUE);
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

EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_sint32 pin_id)
{
    ehs_bool ret = set_output_GPIO(pin_id);
    if(ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO output pin (%d)",pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO output pin (%d)",pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_sint32 pin_id, ehs_bool value)
{
    ehs_bool ret = set_value_GPIO(pin_id, value);
    if(ret == EHS_TRUE)
    {
        //EHSH_LOG_INFO("Write GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to write GPIO (%d,%d).",pin_id,value);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_sint32 pin_id)
{
    EHSH_LOG_INFO("Destroy GPIO output pin (%d)",pin_id);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_sint32 pin_id)
{
    ehs_bool ret = set_input_GPIO(pin_id);
    if(ret == EHS_TRUE)
    {
        EHSH_LOG_INFO("Initialise GPIO input pin (%d)",pin_id);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to initialise GPIO input pin (%d)",pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_sint32 pin_id, ehs_bool* value)
{
    ehs_bool ret = get_value_GPIO(pin_id, value);
    if(ret == EHS_TRUE)
    {
        //EHSH_LOG_INFO("Read GPIO (%d,%d).",pin_id,value);
    }
    else
    {
        EHSH_LOG_ERROR("Failed to read GPIO (%d).",pin_id);
    }
    return ret;
}

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_sint32 pin_id)
{
    EHSH_LOG_INFO("Destroy GPIO input pin (%d)",pin_id);
    return EHS_TRUE;
}
