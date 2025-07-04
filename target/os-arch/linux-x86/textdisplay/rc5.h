/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef __RC5_H__
#define __RC5_H__

#define RC5_SHORT_PULSE		1
#define RC5_LONG_PULSE		2
#define RC5_SHORT_SPACE		3
#define RC5_LONG_SPACE		4

#define RC5_STATE_BEGIN		0
#define RC5_STATE_MID0		1
#define RC5_STATE_MID1		2
#define RC5_STATE_START0	3
#define RC5_STATE_START1	4

#define RC5_COMMAND_LEN 	14
#define RC5_BUTTON_DELAY	60
/*
rc5 decoded command:
    2 start bits
    1 toggle bit
    5 address bits
    6 command bits
*/

typedef struct _rc5decoder_s rc5decoder;
struct _rc5decoder_s
{
    int state;
    int nbits;
    char bits[RC5_COMMAND_LEN]; /* a rc5 decoded command */
    struct timespec command_delay; /* keeps track of delays between rc5 commands */
};

rc5decoder * rc5_init(void);
int rc5_decode(rc5decoder *rc5, unsigned char *data, int len);
void rc5_close(rc5decoder *rc5);
#endif

