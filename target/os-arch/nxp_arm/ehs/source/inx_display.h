/*
 * inx_display.h
 *
 *  Created on: 6 Dec 2019
 *      Author: patrick
 */

#ifndef INX_DISPLAY_H_
#define INX_DISPLAY_H_

#include "heatrod_config.h"
#include "errorLogs.h"

void inxDisplayPoll(const bool ignoreErrors);
void inxDisplaySet(const char char1,const char char2, const char char3, const char char4,const bool async);
void inxDisplayClearError();
int8_t inxDisplayGetErrorCode();
void inxDisplaySetError();
void inxDisplaySetErrorCode(const errorLogsError_t error,const bool async);
void inxDisplaySetFromTemperature(float temperature,const uint8_t displayNo);
void inxDisplaySetForScheduleMode(const Schedule_Mode_t scheduleMode);
void inxDisplaySetChar(const uint8_t displayNo,const uint8_t index,const uint8_t value);
#ifdef INX_HEATROD_IOT_DIAGNOSTIC_BUILD
void diagnostic_hw_test_display();
#endif
#endif /* INX_DISPLAY_H_ */
