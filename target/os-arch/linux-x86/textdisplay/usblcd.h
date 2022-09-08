/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef __USBLCD_H__
#define __USBLCD_H__

#include <usb.h>
#include "driver.h"

#define VERSION				"0.1.5"

/* public defs */
unsigned int debug_level;

typedef int hid_return;

typedef struct _hid_device_s hid_device;
struct _hid_device_s
{
    unsigned int id;
    struct usb_device *device;
    struct usb_dev_handle *handle;
};

typedef struct _hid_params_s hid_params;
struct _hid_params_s
{
    unsigned int endpoint;
    unsigned int packetlen;
    unsigned int timeout;
    void *packet;
};

typedef struct _hid_operations_s hid_operations;
struct _hid_operations_s
{

    hid_device *hiddev;
    hid_return (*debug)(int level);
    hid_return (*init)(hid_device **hiddev);
    hid_return (*interrupt_read)(void *hiddev, hid_params *params);
    hid_return (*interrupt_write)(void *hiddev, hid_params *params);
    hid_return (*controlled_read)(void);  /* not implemented */
    hid_return (*controlled_write)(void); /* not implemented */
    hid_return (*close)(void *hiddev);
};

typedef struct _usblcd_event_s usblcd_event;
struct _usblcd_event_s
{
    /* 0 keypad data, 1 infrared data */
    unsigned int type;
    int length;
    unsigned char *data;
};

typedef struct _usblcd_state_s usblcd_state;
struct _usblcd_state_s
{
    unsigned int usblcd_switch;
    unsigned int usblcd_cursor;
    unsigned int usblcd_cursor_blink;
};

typedef struct _usblcd_splash_data_s usblcd_splash_data;
struct _usblcd_splash_data_s
{
    int min;
    int sec;
    int jump;
    int repeat;
    unsigned int leds;
    char *line[_USBLCD_MAX_ROWS + 1];
};

typedef struct _usblcd_font_data_s usblcd_font_data;
struct _usblcd_font_data_s
{
    int id;
    char *line[_USBLCD_FONT_MAX_ROWS];
};

typedef struct _usblcd_operations_s usblcd_operations;
struct _usblcd_operations_s
{

    /* lcd and cursor status */
    usblcd_state *state;
    /* leds state */
    unsigned int leds;
    /* misc functions */
    void (*match)(void);
    void (*init)(usblcd_operations *self);
    void (*debug)(int level);

    /* input functions */
    void (*setled)(usblcd_operations *self, unsigned int led, unsigned int status);
    void (*backlight)(usblcd_operations *self, unsigned int status);
    void (*contrast)(usblcd_operations *self, unsigned int level);
    void (*set_cursor)(usblcd_operations *self, unsigned int status);
    void (*set_cursor_blink)(usblcd_operations *self, unsigned int status);
    void (*set_switch)(usblcd_operations *self, unsigned int status);
    usblcd_event * (*read_events)(usblcd_operations *self);

    void (*enter_flasher_mode)(usblcd_operations *self);
    void (*exit_flasher_mode)(usblcd_operations *self);
    void (*clear)(usblcd_operations *self);
    void (*setchar)(usblcd_operations *self, unsigned int row, unsigned int column, char character);
    void (*settext)(usblcd_operations *self, unsigned int row, unsigned int column, char *text);
    void (*setfont)(usblcd_operations *self, char *filename);
    void (*setfontmemory)(usblcd_operations *self, int fontlines[], int nrchars);
    void (*setsplash)(usblcd_operations *self, char *filename);
    void (*flash)(usblcd_operations *self);
    void (*getversion)(usblcd_operations *self);
    /* output functions */
    void (*powerstate)(usblcd_operations *self);
    void (*keystate)(usblcd_operations *self);
    void (*irdata)(usblcd_operations *self);

    void (*close)(usblcd_operations *self);

    /* should not be called directly */
    void (*control)(usblcd_operations *self);

    /* pointer to deeper level hid functions */
    hid_operations *hid;

};

/* private class */
/* hid operations constructor */
//static hid_operations *new_hid_operations(void);
hid_operations *new_hid_operations(void);
/* public classes */
/* usbdriver operations constructor */

usblcd_operations *new_usblcd_operations(void);

#endif  //__USBLCD_H__
