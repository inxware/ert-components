#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "target_ethernet.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_mac.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"

#include "esp_event.h"
//#include "esp_netif_types.h"
#include "esp_netif.h"



#define TAG "target_ethernet"

typedef struct {
    uint8_t spi_cs_gpio;
    uint8_t int_gpio;
    int8_t phy_reset_gpio;
    uint8_t phy_addr;
    uint8_t *mac_addr;
} spi_eth_module_config_t;

/* exported global used to check at boot and startup etc.*/
/* This is probably used by wifi also?? */
//todo2024 - review this - the bts below shouldn't be here probablt
extern volatile ehs_bool gNetworkConnected;

ehs_char* gEthHostNameBuffer[32] = {'\0'};


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

static esp_err_t eth_deinit_spi(esp_eth_handle_t eth_handle, esp_eth_mac_t *mac, esp_eth_phy_t *phy)
{
    esp_err_t ret = ESP_OK;

    if (eth_handle != NULL)
    {
        ESP_GOTO_ON_ERROR(esp_eth_driver_uninstall(eth_handle), err, TAG, "SPI Ethernet driver uninstall failed");
        if (mac != NULL) mac->del(mac);
        if (phy != NULL) phy->del(phy);
    }
    return ret;
err:
    return ret;
}


/*
Initialise the Ethernet MAC chip
This implementation is Specific to W5500 Ehternet MAC chip (Module or chipdown connected to SPI bus 1)
returns ESPRESSIF#s errornumber 
*/





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

esp_err_t target_eth_deinit(esp_eth_handle_t eth_handle)
{
    esp_err_t ret = ESP_OK;

    ESP_GOTO_ON_ERROR(eth_deinit_spi(eth_handle, NULL, NULL), err, TAG, "SPI Ethernet deinit failed");
    ESP_GOTO_ON_ERROR(spi_bus_free(TARGET_ETH_SPI_NUM), err, TAG, "SPI bus free failed");

    return ret;
err:
    return ret;
}


/* TODO - the following looks like it should be common code but has some esp32 specific API 
   calls that may not be abstracted and many random uses of esp32 specific logging.
   Consider if this should be generalised for Ethernet Phy management.
*/

/* Flag gets set to true once the network interface stack has been initalised and started */
volatile ehs_bool gNetworkStarted = EHS_FALSE;//NOT CURRENTLY USED ANYWHERE


static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    uint8_t mac_addr[6] = {0};
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *) event_data;

    switch (event_id) {
    case ETHERNET_EVENT_CONNECTED:
        esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
        ESP_LOGI(TAG, "Ethernet Link Up");
        ESP_LOGI(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
                    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        gNetworkConnected = EHS_TRUE;
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Ethernet Link Down");
        gNetworkConnected = EHS_FALSE;
        break;
    case ETHERNET_EVENT_START:
        ESP_LOGI(TAG, "Ethernet Started");
        gNetworkStarted = EHS_TRUE;
        break;
    case ETHERNET_EVENT_STOP:
        ESP_LOGI(TAG, "Ethernet Stopped");
        gNetworkStarted = EHS_FALSE;
        break;
    default:
        break;
    }
}

static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    ESP_LOGI(TAG, "Ethernet Got IP Address");
    ESP_LOGI(TAG, "#######################");
    ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "#######################");
    // make sure ip address/mac is updated after establishing connection 
    EhsHMetaUpdateDynamic();
}

static esp_eth_handle_t eth_handle = NULL;
static esp_netif_t *eth_netif = NULL;

/* Initialise an Ethernet MAC (if one is fitted)
   Currently this is hardwired to call initialisation of only W5500 ethernet MAC via a specific SPI line)
*/

ehs_bool ehs_eth_init()
{
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK_WITHOUT_ABORT(target_eth_init(&eth_handle));

    // Initialise TCP/IP network interface
    if (sfNetifStatusGet() != EHS_TRUE)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_init());
        sfNetifStatusSet(EHS_TRUE);
    }

    // Create defaultevent loop running in the background
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_create_default());

    // Create instance fo eps-netif for Ethernet
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    eth_netif = esp_netif_new(&cfg);

    // Append unique suffix from MAC
    uint8_t mac[6];
    esp_mac_type_t mac_type = ESP_MAC_EFUSE_FACTORY; // Use the efuse which was burnt by Espressif in production
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_read_mac(mac, mac_type));
    snprintf(gEthHostNameBuffer, sizeof(gEthHostNameBuffer), TARGET_HOSTNAME"-e%02X%02X%02X%02X", mac[2], mac[3], mac[4], mac[5]);
    ESP_LOGI(TAG, "Eth host name : %s", gEthHostNameBuffer);
    // Set the hostname
    esp_netif_set_hostname(eth_netif, gEthHostNameBuffer);

    // Attach Ethernet driver to TCP/IP stack
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_attach(eth_netif, esp_eth_new_netif_glue(eth_handle)));

    // Register user defined event handler
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

    // Start Ethernet driver state machine
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_eth_start(eth_handle));

    return (ret == ESP_OK);
}

ehs_bool eth_deinit()
{
    esp_err_t ret = ESP_OK;
    if(eth_handle){
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_eth_stop(eth_handle));
        ESP_LOGI(TAG, "Unregistering Ethernet event handlers");
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_unregister(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_unregister(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_delete_default());
        if (eth_netif != NULL)
        {
            esp_netif_destroy(eth_netif);
            eth_netif = NULL;
        }
        ESP_ERROR_CHECK_WITHOUT_ABORT(target_eth_deinit(eth_handle));
        eth_handle = NULL;
    }
    return ( ret == ESP_OK );
}
