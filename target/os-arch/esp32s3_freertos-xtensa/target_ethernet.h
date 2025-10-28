#ifndef _TARGET_ETHERNET_H_
#define _TARGET_ETHERNET_H_

#include "esp_err.h"
#include "esp_eth_driver.h"

#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise Ethernet driver
 * 
 * @param[out] eth_handles_out initialised Ethernet driver handles
 * 
 * @return
 *          - ESP_OK on success
 *          - ESP_ERR_INVALID_ARG when passed invalid-pointers
 *          - ESP_ERR_NO_MEM when there is no memory to allocate for Ethernet drivern handles array
 *          - ESP_FAIL on any other failure
 */
esp_err_t target_eth_init(esp_eth_handle_t *eth_handles_out);

/**
 * @brief Deinitialise Ethernet driver
 * 
 * @param[in] eth_handle Ethernet driver handle to deinitialise
 *
 * @return
 *          - ESP_OK on success
 */
esp_err_t target_eth_deinit(esp_eth_handle_t eth_handle);

#ifdef __cplusplus
}
#endif

#endif//_TARGET_ETHERNET_H_