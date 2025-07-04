/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* @file certificate.h
 *
 * @author: inx limited
 *
 */


#ifndef CERTIFICATE_H_
#define CERTIFICATE_H_

#include <stddef.h> /* for size_t */
#include "httpsrv_tls.h"

/* own local mosquitto server certificate */
extern const char mbedtls_m2mqtt_srv_crt[];
extern const size_t mbedtls_m2mqtt_srv_crt_len;

extern const char mbedtls_m2mqtt_client_crt[];
extern const size_t mbedtls_m2mqtt_client_crt_len;

extern const char mbedtls_provisioning_ca_crt[];

extern const size_t mbedtls_m2mqtt_client_pk_key_len;
extern const char mbedtls_m2mqtt_client_pk_key[];

#if HTTPSRV_CFG_MBEDTLS_ENABLE
extern const char gHttpdCrt[];
extern const size_t gHttpdCrtSize;
extern const char gHttpdCrtKey[];
extern const size_t gHttpdCrtKeySize;
#endif

#endif /* CERTIFICATE_H_ */
