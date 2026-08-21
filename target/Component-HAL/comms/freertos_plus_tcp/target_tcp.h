/***************************************************************
 * Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_tcp.h
 * Target-specific TCP/IP declarations for the FreeRTOS-Plus-TCP comms HAL.
 *
 * Currently a placeholder: required because Common/HAL/include/hal_network.h
 * unconditionally includes this header. The XMOS xcore.ai platform consumes
 * FreeRTOS-Plus-TCP (fwk_rtos/modules/FreeRTOS/FreeRTOS-Plus-TCP) via its
 * FreeRTOS_Sockets.h API — not standard BSD sockets — so a dedicated
 * Component-HAL implementation will live here when networking is enabled.
 *
 * Until then the platform sets EHS_COMMS_API_SUPPORT=none, which keeps the
 * EHS_COMMS_API_SUPPORT-gated code in hal.c et al. inactive. Only this
 * header needs to be visible on the include path to satisfy hal_network.h.
 *
 * @author: inx limited
 */

#ifndef EHS_TARGET_TCP_H
#define EHS_TARGET_TCP_H

/* TODO: When networking is implemented, define the EhsTgtTcp* types and
 * EHS_TGT_TCP_* macros here against FreeRTOS_Sockets.h, mirroring the
 * lwip/bsdsockets variants, and add target_tcp.c / targetcomms_init.c /
 * comms.mk so this directory is a complete Component-HAL. */

#endif /* EHS_TARGET_TCP_H */
