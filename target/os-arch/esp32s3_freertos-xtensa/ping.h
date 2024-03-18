/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef __ping_h__
#define __ping_h__

#include "ping/ping_sock.h"

void cmd_ping_on_ping_success(esp_ping_handle_t hdl, void *args);
void cmd_ping_on_ping_timeout(esp_ping_handle_t hdl, void *args);
void cmd_ping_on_ping_end(esp_ping_handle_t hdl, void *args);
esp_err_t initialize_ping(uint32_t interval_ms, uint32_t task_prio,
                          char *target_host);

#endif /* __ping_h__ */
