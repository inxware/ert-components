#include "target_config.h"

#if TARGET_USE_ETHERNET == 1
#include "target_ethernet.h"
#include "target_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_mac.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"
#include <stdio.h>
#include <stdlib.h>

#define TAG "target_ethernet"

typedef struct {
    uint8_t spi_cs_gpio;
    uint8_t int_gpio;
    int8_t phy_reset_gpio;
    uint8_t phy_addr;
    uint8_t *mac_addr;
} spi_eth_module_config_t;

/**
 * @brief SPI bus initialisation
 * 
 * @return
 *          - ESP_OK on success
 */
static esp_err_t spi_bus_init(void)
{
    esp_err_t ret = ESP_OK;

    ret = gpio_install_isr_service(0);
    if (ret != ESP_OK)
    {
        if (ret == ESP_ERR_INVALID_STATE)
        {
            ESP_LOGW(TAG, "GPIO ISR handler has been already installed");
            ret = ESP_OK;
        }
        else {
            ESP_LOGE(TAG, "GPIO ISR handler install failed");
            goto err;
        }
    }

    // Init SPI bus
    spi_bus_config_t buscfg = {
        .miso_io_num = TARGET_ETH_SPI_MISO,
        .mosi_io_num = TARGET_ETH_SPI_MOSI,
        .sclk_io_num = TARGET_ETH_SPI_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    ESP_GOTO_ON_ERROR(spi_bus_initialize(TARGET_ETH_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO),
                        err, TAG, "SPI host #%d ini failed", TARGET_ETH_SPI_NUM);
    
err:
    return ret;
}

/**
 * @brief Ethernet SPI module initialisation
 * 
 * @param[in] spi_eth_module_config specific SPI Ethernet module configuration
 * @param[out] mac_out optionally returns Ethernet MAC object
 * @param[out] phy_out optionally returns Ethernet PHY object
 * 
 * @return
 *          - esp_eth_handle_t if init succeeded
 *          - NULL if init failed
 */
static esp_eth_handle_t eth_init_spi(spi_eth_module_config_t *spi_eth_module_config, esp_eth_mac_t **mac_out, esp_eth_phy_t **phy_out)
{
    esp_eth_handle_t ret = NULL;

    // Init common MAC and PHY configs to default
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
    mac_config.rx_task_stack_size = 3072; /* TODO - THIS SHOULD BE A CONFIG PARAMTER OVERRIDABLE PER PLATFORM? */
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();

    // Update PHY config based on board specific configurations
    phy_config.phy_addr = spi_eth_module_config->phy_addr;
    phy_config.reset_gpio_num = spi_eth_module_config->phy_reset_gpio;

    //Config SPI interface for specific SPI module
    spi_device_interface_config_t spi_devcfg = {
        .mode = 0,
        .clock_speed_hz = TARGET_ETH_SPI_MHZ * 1000 * 1000,
        .queue_size = 20,
        .spics_io_num = spi_eth_module_config->spi_cs_gpio
    };

    // Config the W5500 module
    eth_w5500_config_t w5500_config = ETH_W5500_DEFAULT_CONFIG(TARGET_ETH_SPI_NUM, &spi_devcfg);
    w5500_config.int_gpio_num = spi_eth_module_config->int_gpio;
    esp_eth_mac_t *mac = esp_eth_mac_new_w5500(&w5500_config, &mac_config);
    esp_eth_phy_t *phy = esp_eth_phy_new_w5500(&phy_config);

    // Init Ethernet driver to default and install it
    esp_eth_handle_t eth_handle = NULL;
    esp_eth_config_t eth_config_spi = ETH_DEFAULT_CONFIG(mac, phy);
    ESP_GOTO_ON_FALSE(esp_eth_driver_install(&eth_config_spi, &eth_handle) == ESP_OK, NULL, err, TAG, "SPI Ethernet driver install failed");

    // Set the local MAC address if the Ethernet module does not have a burnt factory MAC address
    if (spi_eth_module_config->mac_addr != NULL)
    {
        ESP_GOTO_ON_FALSE(esp_eth_ioctl(eth_handle, ETH_CMD_S_MAC_ADDR, spi_eth_module_config->mac_addr) == ESP_OK,
                            NULL, err, TAG, "SPI Ethernet MAC address config failed");
    }

    // Output the optional MAC and PHY objects
    if (mac_out != NULL) *mac_out = mac;
    if (phy_out != NULL) *phy_out = phy;

    return eth_handle;
    err:
    if (eth_handle != NULL) esp_eth_driver_uninstall(eth_handle);
    if (mac != NULL) mac->del(mac);
    if (phy != NULL) phy->del(phy);
    return ret;
}

esp_err_t target_eth_init(esp_eth_handle_t *eth_handles_out)
{
    esp_err_t ret = ESP_OK;
    esp_eth_handle_t *eth_handle = NULL;

    eth_handle = calloc(1, sizeof(esp_eth_handle_t));
    ESP_GOTO_ON_FALSE(eth_handle != NULL, ESP_ERR_NO_MEM, err, TAG, "no memory");

    ESP_GOTO_ON_ERROR(spi_bus_init(), err, TAG, "SPI bus init failed");
    spi_eth_module_config_t spi_eth_module_config = {
        .spi_cs_gpio = TARGET_ETH_SPI_CS,
        .int_gpio = TARGET_ETH_INT,
        .phy_reset_gpio = TARGET_ETH_RESET,
        .phy_addr = 1
    };

    uint8_t base_mac_addr[ETH_ADDR_LEN];
    ESP_GOTO_ON_ERROR(esp_efuse_mac_get_default(base_mac_addr), err, TAG, "get EFUSE MAC failed");
    uint8_t local_mac_1[ETH_ADDR_LEN];
    esp_derive_local_mac(local_mac_1, base_mac_addr);
    spi_eth_module_config.mac_addr= local_mac_1;

    eth_handle = eth_init_spi(&spi_eth_module_config, NULL, NULL);
    ESP_GOTO_ON_FALSE(eth_handle, ESP_FAIL, err, TAG, "SPI Ethernet init failed");

    bool autonego = false;
    esp_eth_ioctl(eth_handle, ETH_CMD_S_AUTONEGO, &autonego);
    bool flowcontrol = true;
    esp_eth_ioctl(eth_handle, ETH_CMD_S_FLOW_CTRL, &flowcontrol);
    eth_speed_t ethernet_speed = ETH_SPEED_10M;
    esp_eth_ioctl(eth_handle, ETH_CMD_G_SPEED, &ethernet_speed);
    eth_duplex_t ethernet_duplex = ETH_DUPLEX_HALF;
    esp_eth_ioctl(eth_handle, ETH_CMD_S_DUPLEX_MODE, &ethernet_duplex);

    *eth_handles_out = eth_handle;

    return ret;
err:
    if (eth_handle != NULL) free(eth_handle);
    return ret;
}

#endif // TARGET_USE_ETHERNET == 1
