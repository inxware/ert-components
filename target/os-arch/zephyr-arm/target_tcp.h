/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_tcp.h
 * Target-specific network/TCP declarations for Zephyr ARM.
 *
 * This file has to exist because Common/HAL/include/hal_network.h includes
 * "target_tcp.h" UNCONDITIONALLY, and hal_network.h is reached from hal-api.h
 * on every target. Zephyr boards with no sockets backend still need the
 * include to resolve.
 *
 * When a backend IS selected (EHS_COMMS_API_SUPPORT), the real declarations
 * live in target/Component-HAL/comms/<backend>/target_tcp.h. That directory is
 * on the include path, but this os-arch directory is added to INC_DIRS earlier
 * (target.mk runs before ehs.mk's EHS_TARGET_COMMS_API_PATH append), so an
 * unqualified #include "target_tcp.h" lands HERE and shadows it. Hence the
 * explicit delegation below.
 *
 * @author: inx limited
 */

/* Delegate BEFORE this file's own include guard is defined. The backend header
 * uses the same EHS_TARGET_TCP_H guard name, so defining ours first would
 * silently suppress it and leave every socket type and macro undeclared - with
 * the errors landing in console_server.c rather than pointing here. */
#ifdef EHS_COMMS_API_SUPPORT
/* bsdsockets is the only backend that fits Zephyr: plain POSIX
 * (socket/bind/listen/accept, netinet/in.h, netdb.h), which Zephyr supplies via
 * CONFIG_POSIX_API + CONFIG_NET_SOCKETS. lwip / winsock / freertos_plus_tcp
 * would each bring their own stack. EHS_COMMS_API_SUPPORT is defined without a
 * value, so if a second Zephyr backend is ever added the build will have to
 * pass the name separately and this must switch on it. */
#include "../../Component-HAL/comms/bsdsockets/target_tcp.h"
#endif /* EHS_COMMS_API_SUPPORT */

#ifndef EHS_TARGET_TCP_H
#define EHS_TARGET_TCP_H

/* Reached only when no sockets backend is configured for this board - nothing
 * to declare, hal_network.h just needs the include to succeed. */

#endif /* EHS_TARGET_TCP_H */
