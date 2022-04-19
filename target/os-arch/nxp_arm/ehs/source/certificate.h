/*
 * certificate.h
 *
 *  Created on: 19.04.2017
 *      Author: Erich Styger
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
