/*******************************************************************************
* @file  app.c
* @brief
*******************************************************************************
* # License
* <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/
/*************************************************************************
 *
 */

/*================================================================================
 * @brief : This file contains example application for Wlan Station BLE
 * Provisioning
 * @section Description :
 * This application explains how to get the WLAN connection functionality using
 * BLE provisioning.
 * Silicon Labs Module starts advertising and with BLE Provisioning the Access Point
 * details are fetched.
 * Silicon Labs device is configured as a WiFi station and connects to an Access Point.
 =================================================================================*/

/**
 * Include files
 **/
//! SL Wi-Fi SDK includes
#include "sl_net.h"
#include "sl_board_configuration.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"
#include "sl_si91x_driver.h"
//BLE Specific inclusions
#include <rsi_ble_apis.h>
#include "ble_config.h"
#include "wifi_config.h"
#include "rsi_ble_common_config.h"
#include <rsi_common_apis.h>
#include "glib.h"

// APP version
#define APP_FW_VERSION "0.4"

// TCP IP BYPASS feature check
#define RSI_TCP_IP_BYPASS RSI_DISABLE

// Function prototypes
extern void sl_wifi_app_task(void);
extern void rsi_ble_configurator_task(void *argument);
void rsi_ble_configurator_init(void);
extern void memlcd_app_init(void);

uint8_t magic_word;

osSemaphoreId_t wlan_thread_sem;
osSemaphoreId_t ble_thread_sem;
osSemaphoreId_t i2c_sem;
extern volatile int currentLine;
extern GLIB_Context_t glibContext;

static const sl_wifi_device_configuration_t config = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = WORLD_DOMAIN,
  .boot_config = { .oper_mode       = SL_SI91X_CLIENT_MODE,
                   .coex_mode       = SL_SI91X_WLAN_BLE_MODE,
                   .feature_bit_map = (SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE | SL_SI91X_FEAT_DEV_TO_HOST_ULP_GPIO_1
#ifdef SLI_SI91X_MCU_INTERFACE
                                       | SL_SI91X_FEAT_WPS_DISABLE
#endif
                                       ),
                   .tcp_ip_feature_bit_map = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT
#if (!RSI_TCP_IP_BYPASS)
                                              | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID | SL_SI91X_TCP_IP_FEAT_DNS_CLIENT
                                              | SL_SI91X_TCP_IP_FEAT_SSL | SL_SI91X_TCP_IP_FEAT_ICMP
#endif
                                              ),
                   .custom_feature_bit_map = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID),
                   .ext_custom_feature_bit_map =
                     (SL_SI91X_EXT_FEAT_LOW_POWER_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK | MEMORY_CONFIG
#ifdef SLI_SI917
                      | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0
#endif
                      | SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE),
                   .bt_feature_bit_map         = (SL_SI91X_BT_RF_TYPE | SL_SI91X_ENABLE_BLE_PROTOCOL),
                   .ext_tcp_ip_feature_bit_map = (SL_SI91X_CONFIG_FEAT_EXTENTION_VALID
                                                  | SL_SI91X_EXT_TCP_IP_TOTAL_SELECTS(1) | SL_SI91X_EXT_EMB_MQTT_ENABLE
#ifdef RSI_PROCESS_MAX_RX_DATA
                                                  | SL_SI91X_EXT_TCP_MAX_RECV_LENGTH
#endif
                                                  ),
                   //!ENABLE_BLE_PROTOCOL in bt_feature_bit_map
                   .ble_feature_bit_map =
                     ((SL_SI91X_BLE_MAX_NBR_PERIPHERALS(RSI_BLE_MAX_NBR_PERIPHERALS)
                       | SL_SI91X_BLE_MAX_NBR_CENTRALS(RSI_BLE_MAX_NBR_CENTRALS)
                       | SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
                       | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC))
                      | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
                      | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS)
                      | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE
#if RSI_BLE_GATT_ASYNC_ENABLE
                      | SL_SI91X_BLE_GATT_ASYNC_ENABLE
#endif
                      ),
                   .ble_ext_feature_bit_map =
                     ((SL_SI91X_BLE_NUM_CONN_EVENTS(RSI_BLE_NUM_CONN_EVENTS)
                       | SL_SI91X_BLE_NUM_REC_BYTES(RSI_BLE_NUM_REC_BYTES))
#if RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST
                      | SL_SI91X_BLE_INDICATE_CONFIRMATION_FROM_HOST //indication response from app
#endif
#if RSI_BLE_MTU_EXCHANGE_FROM_HOST
                      | SL_SI91X_BLE_MTU_EXCHANGE_FROM_HOST //MTU Exchange request initiation from app
#endif
#if RSI_BLE_SET_SCAN_RESP_DATA_FROM_HOST
                      | (SL_SI91X_BLE_SET_SCAN_RESP_DATA_FROM_HOST) //Set SCAN Resp Data from app
#endif
#if RSI_BLE_DISABLE_CODED_PHY_FROM_HOST
                      | (SL_SI91X_BLE_DISABLE_CODED_PHY_FROM_HOST) //Disable Coded PHY from app
#endif
#if BLE_SIMPLE_GATT
                      | SL_SI91X_BLE_GATT_INIT
#endif
                      ),
                   .config_feature_bit_map = (SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP) }
};

const osThreadAttr_t thread_attributes = {
  .name       = "application_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityNormal,
  .tz_module  = 0,
  .reserved   = 0,
};

const osThreadAttr_t ble_thread_attributes = {
  .name       = "ble_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 2048,
  .priority   = osPriorityNormal,
  .tz_module  = 0,
  .reserved   = 0,
};

void rsi_wlan_ble_app_init(void *argument)
{
  UNUSED_PARAMETER(argument);
  int32_t status = RSI_SUCCESS;

  memlcd_app_init();
  GLIB_drawStringOnLine(&glibContext, "Demo Started", currentLine++, GLIB_ALIGN_LEFT, 5, 5, true);
  DMD_updateDisplay();

  status = sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE, &config, NULL, network_event_handler);
  if (status != SL_STATUS_OK && status != SL_STATUS_ALREADY_INITIALIZED) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
    GLIB_drawStringOnLine(&glibContext, "Wireless Initialization Failed", currentLine++, GLIB_ALIGN_LEFT, 5, 5, true);
    DMD_updateDisplay();
    return;
  }
  LOG_PRINT("\r\nWireless Initialization Success\n");
  currentLine = 0;
  GLIB_drawStringOnLine(&glibContext, "Wireless interface", currentLine++, GLIB_ALIGN_LEFT, 5, 5, true);
  GLIB_drawStringOnLine(&glibContext, "initialized", currentLine++, GLIB_ALIGN_LEFT, 5, 5, true);
  GLIB_drawStringOnLine(&glibContext, "successfully", currentLine++, GLIB_ALIGN_LEFT, 5, 5, true);
  DMD_updateDisplay();

  wlan_thread_sem = osSemaphoreNew(1, 0, NULL);
  if (wlan_thread_sem == NULL) {
    LOG_PRINT("Failed to create wlan_thread_sem\n");
    return;
  }

  ble_thread_sem = osSemaphoreNew(1, 0, NULL);
  if (ble_thread_sem == NULL) {
    LOG_PRINT("Failed to create ble_thread_sem\n");
    return;
  }

#ifdef SLI_SI91X_MCU_INTERFACE
  uint8_t xtal_enable = 1;
  status              = sl_si91x_m4_ta_secure_handshake(SL_SI91X_ENABLE_XTAL, 1, &xtal_enable, 0, NULL);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to bring m4_ta_secure_handshake: 0x%lx\r\n", status);
    return;
  }
#endif

  if (osThreadNew((osThreadFunc_t)rsi_ble_configurator_task, NULL, &ble_thread_attributes) == NULL) {
    LOG_PRINT("Failed to create BLE thread\n");
  }

  // BLE initialization
  rsi_ble_configurator_init();

  sl_wifi_app_task();

  return;
}

void app_init(void)
{
  osThreadNew((osThreadFunc_t)rsi_wlan_ble_app_init, NULL, &thread_attributes);
}
