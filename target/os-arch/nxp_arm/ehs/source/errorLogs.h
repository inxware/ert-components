#ifndef INX_ERROR_LOGS_H_
#define INX_ERROR_LOGS_H_

#include "MK64F12.h"
#include "heatrod_config.h"

errorLogsError_t errorLogsRead();
err_t errorLogsClear();
void errorLogsClearSingleError(const errorLogsError_t error);
void errorLogsWrite(const errorLogsError_t error);
void errorLogsWritePayload(char* errorLogsPayload);

#endif /* INX_ERROR_LOGS_H_ */
