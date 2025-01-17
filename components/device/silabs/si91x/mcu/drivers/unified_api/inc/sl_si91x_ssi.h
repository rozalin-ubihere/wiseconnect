/***************************************************************************/ /**
* @file sl_si91x_ssi.h
* @brief SSI Peripheral API implementation
* *******************************************************************************
* * # License
* * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
* *******************************************************************************
* *
* * SPDX-License-Identifier: Zlib
* *
* * The licensor of this software is Silicon Laboratories Inc.
* *
* * This software is provided 'as-is', without any express or implied
* * warranty. In no event will the authors be held liable for any damages
* * arising from the use of this software.
* *
* * Permission is granted to anyone to use this software for any purpose,
* * including commercial applications, and to alter it and redistribute it
* * freely, subject to the following restrictions:
* *
* * 1. The origin of this software must not be misrepresented; you must not
* *    claim that you wrote the original software. If you use this software
* *    in a product, an acknowledgment in the product documentation would be
* *    appreciated but is not required.
* * 2. Altered source versions must be plainly marked as such, and must not be
* *    misrepresented as being the original software.
* * 3. This notice may not be removed or altered from any source distribution.
* *
*******************************************************************************/

#ifndef SL_SI91X_SSI_H
#define SL_SI91X_SSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"
#include "SPI.h"

/***************************************************************************/ /**
 * @addtogroup SSI Synchronous Serial Interface
 * @ingroup SI91X_PERIPHERAL_APIS
 * @{
 * 
 ******************************************************************************/
// -----------------------------------------------------------------------------
// Data Types

typedef ARM_POWER_STATE sl_ssi_power_state_t;        ///< renaming arm power state structure
typedef ARM_SPI_STATUS sl_ssi_status_t;              ///< renaming arm SPI status
typedef ARM_DRIVER_SPI sl_ssi_driver_t;              ///< Renaming SSI driver structure
typedef const void *sl_ssi_handle_t;                 ///< SSI Handle to be passed into APIs
typedef ARM_SPI_SignalEvent_t sl_ssi_signal_event_t; ///< Callback typedef for SSI

/// @brief Structure to hold the version numbers of the peripheral API
typedef struct {
  uint8_t release; ///< Release version number
  uint8_t major;   ///< SQA version number
  uint8_t minor;   ///< DEV version number
} sl_ssi_version_t;

/// @brief Enumeration for different SSI callback events
typedef enum {
  SSI_EVENT_TRANSFER_COMPLETE = ARM_SPI_EVENT_TRANSFER_COMPLETE, ///< Transfer complete event
  SSI_EVENT_DATA_LOST         = ARM_SPI_EVENT_DATA_LOST,         ///< Data lost event
  SSI_EVENT_MODE_FAULT        = ARM_SPI_EVENT_MODE_FAULT,        ///< Mode fault event
} ssi_event_typedef_t;

/// @brief Enumeration for different SSI peripheral clock modes
typedef enum {
  SL_SSI_PERIPHERAL_CPOL0_CPHA0 = ARM_SPI_CPOL0_CPHA0, ///< Mode 0 (CPOL = 0, CPHA = 0)
  SL_SSI_PERIPHERAL_CPOL0_CPHA1 = ARM_SPI_CPOL0_CPHA1, ///< Mode 1 (CPOL = 0, CPHA = 1)
  SL_SSI_PERIPHERAL_CPOL1_CPHA0 = ARM_SPI_CPOL1_CPHA0, ///< Mode 2 (CPOL = 1, CPHA = 0)
  SL_SSI_PERIPHERAL_CPOL1_CPHA1 = ARM_SPI_CPOL1_CPHA1, ///< Mode 3 (CPOL = 1, CPHA = 1)
  SL_SSI_PERIPHERAL_TI_SSI      = ARM_SPI_TI_SSI,      ///< TI mode
  SL_SSI_PERIPHERAL_MICROWIRE   = ARM_SPI_MICROWIRE,   ///< Microwire/Half_duplex mode
  SL_SSI_PERIPHERAL_MODE_LAST                          ///< LSAT member of enum for validation.
} ssi_peripheral_clock_mode_t;

/// @brief Enumeration for different SSI peripheral device modes
typedef enum {
  SL_SSI_MASTER_ACTIVE = ARM_SPI_MODE_MASTER, ///< Primary mode is active
  SL_SSI_SLAVE_ACTIVE  = ARM_SPI_MODE_SLAVE,  ///< Secondary mode is active
  SL_SSI_ULP_MASTER_ACTIVE,                   ///< ULP PRIMARY mode is active
  SL_SSI_INSTANCE_LAST,                       ///< LSAT member of enum for validation.
} sl_ssi_instance_t;

/// @brief Structure to hold the parameters for the configuration of SSI peripheral
typedef struct {
  uint8_t bit_width;             ///< bit width either 8 or 16 bit
  uint32_t device_mode;          ///< mode such as Master or Slave mode
  uint32_t clock_mode;           ///< clock mode such as CPOL0 CPHA1
  uint32_t baud_rate;            ///< baud rate for SSI
  uint32_t receive_sample_delay; ///< Delay for receive input signal
} sl_ssi_control_config_t;

/// @brief Structure to hold the clock configuration parameters
typedef struct {
  uint16_t division_factor;            ///< Clock Division Factor
  uint16_t intf_pll_500_control_value; ///< intf PLL control value
  uint32_t intf_pll_clock;             ///< intf PLL clock frequency
  uint32_t intf_pll_reference_clock;   ///< intf PLL reference clock frequency
  uint32_t soc_pll_clock;              ///< SoC PLL Clock frequency
  uint32_t soc_pll_reference_clock;    ///< SoC PLL reference clock frequency
  uint8_t soc_pll_mm_count_value;      ///< SoC PLL count value
} sl_ssi_clock_config_t;

/// @brief Enumeration for SSI Secondary numbers.
typedef enum {
  SSI_SLAVE_0,                ///< Secondary No. 1
  SSI_SLAVE_1,                ///< Secondary No. 2
  SSI_SLAVE_2,                ///< Secondary No. 3
  SSI_SLAVE_3,                ///< Secondary No. 4
  SSI_SLAVE_NUMBER_LAST_ENUM, ///< Last member of enum for validation
} sl_ssi_slave_number_t;

/***************************************************************************/ /**
 * Set the clock for the SSI peripheral, Configures the PLL clock and
 * SOC clock with the value set by user in the clock configuration structure.
 *
 * @param[in] clock_config Pointer to clock config structure \ref sl_ssi_clock_config_t
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_OK (0x0000) - Success, timer clock-source parameters configured properly
 *            - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer \n
 *            - SL_STATUS_INVALID_PARAMETER (0x0021) - Parameters are invalid \n
 *            - SL_STATUS_FAIL (0x0001) - The function is failed \n
 *            - SL_STATUS_NOT_INITIALIZED (0x0011) - Clock is not initialized
*******************************************************************************/
sl_status_t sl_si91x_ssi_configure_clock(sl_ssi_clock_config_t *clock_config);

/***************************************************************************/ /**
 * Initialize the SSI. If the DMA is enabled, it also initializes the DMA.
 * Pass the address of the pointer for storing the SSI Primary/Secondary/ULP Primary
 * handle, which can be used in future for other function calls.
 *
 * @param[in] instance (Primary/Secondary/ULP Primary) ( \ref sl_ssi_instance_t)
 * @param[in] ssi_handle Double Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_OK (0x0000) - Success, otherwise fail error code as follow
 *            - SL_STATUS_INVALID_PARAMETER (0x0021) - Parameters are invalid \n
 *            - SL_STATUS_BUSY (0x0004) - Driver is busy
*******************************************************************************/
sl_status_t sl_si91x_ssi_init(sl_ssi_instance_t instance, sl_ssi_handle_t *ssi_handle);

/***************************************************************************/ /**
 * Uninitialize the SSI. If the DMA is enabled, it also uninitializes the
 * DMA
 *
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_OK (0x0000) - Success, otherwise fail error code as follow
 *            - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer
*******************************************************************************/
sl_status_t sl_si91x_ssi_deinit(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Control and configure the SSI.
 *   - Mode (Primary/Secondary/ULP Primary)
 *   - bit_width (4 bit to 16 bit)
 *   - clock_mode/Frame format (mode0 to mode3, TI and Microwire)
 *   - bitrate (10 Mbps to 40 Mbps)
 *   - Rx sample delay (0 to 63)
 *
 * @param[in]  ssi_handle pointer to ssi instance handle. ( \ref sl_ssi_handle_t)
 * @param[in]  control_configuration pointer to control config structure. ( \ref sl_ssi_control_config_t)
 * @param[in]  slave_number In the current implementation variable slave_number is un-used
 *                              ( \ref sl_ssi_slave_number_t)
 * 
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_OK (0x0000) - Success 
 *            - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer 
 *            - SL_STATUS_INVALID_PARAMETER (0x0021) - Parameters are invalid 
 *            - SL_STATUS_FAIL (0x0001) - The function is failed 
 *            - SL_STATUS_NOT_SUPPORTED (0x000F) - Parameter is not supported 
 *            - SL_STATUS_BUSY (0x0004) - Driver is busy 
 *            - SL_STATUS_INVALID_MODE (0x0024) - Slave select Mode is invalid 
 *            - SL_STATUS_INVALID_TYPE (0x0026) - SPI frame format is not valid 
 *            - SL_STATUS_INVALID_RANGE (0x0028) - Data bits (frame length) is not in range
*******************************************************************************/
sl_status_t sl_si91x_ssi_set_configuration(sl_ssi_handle_t ssi_handle,
                                           sl_ssi_control_config_t *control_configuration,
                                           sl_ssi_slave_number_t slave_number);

/***************************************************************************/ /**
 * Start receiving the data from the SSI interface.
 *
 * @pre \ref sl_si91x_ssi_configure_clock \n
 * @pre \ref sl_si91x_ssi_init \n
 * @pre \ref sl_si91x_ssi_set_configuration \n
 * @pre \ref sl_si91x_ssi_set_slave_number
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @param[in] data pointer to the variable which will store the received data
 * @param[in] data_length (uint32_t) number of data items to receive
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_OK (0x0000) - Success 
 *            - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer 
 *            - SL_STATUS_INVALID_PARAMETER (0x0021) - Parameters are invalid 
 *            - SL_STATUS_FAIL (0x0001) - The function is failed 
 *            - SL_STATUS_BUSY (0x0004) - Driver is busy
*******************************************************************************/
sl_status_t sl_si91x_ssi_receive_data(sl_ssi_handle_t ssi_handle, void *data, uint32_t data_length);

/***************************************************************************/ /**
 * Start sending the data from the SSI interface.
 *
 * @pre \ref sl_si91x_ssi_configure_clock \n
 * @pre \ref sl_si91x_ssi_init \n
 * @pre \ref sl_si91x_ssi_set_configuration \n
 * @pre \ref sl_si91x_ssi_set_slave_number
 * 
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @param[in] data pointer to the variable which will store the received data
 * @param[in] data_length (uint32_t) number of data items to receive
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_OK (0x0000) - Success 
 *            - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer 
 *            - SL_STATUS_INVALID_PARAMETER (0x0021) - Parameters are invalid  
 *            - SL_STATUS_FAIL (0x0001) - The function is failed 
 *            - SL_STATUS_BUSY (0x0004) - Driver is busy
*******************************************************************************/
sl_status_t sl_si91x_ssi_send_data(sl_ssi_handle_t ssi_handle, const void *data, uint32_t data_length);

/***************************************************************************/ /**
 * Start to send and receive bi-directional full duplex data to/from the SPI secondary.
 *
 * @pre \ref sl_si91x_ssi_configure_clock \n
 * @pre \ref sl_si91x_ssi_init \n
 * @pre \ref sl_si91x_ssi_set_configuration \n
 * @pre \ref sl_si91x_ssi_set_slave_number
 * 
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @param[in] data_out const pointer to the variable that has data which needs to be sent
 * @param[in] data_in pointer to the variable which will store the received data
 * @param[in] data_length (uint32_t) number of data items to receive
 * @return status 0 if successful, else error code
 *         - SL_STATUS_OK (0x0000) - Success 
 *         - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer 
 *         - SL_STATUS_INVALID_PARAMETER (0x0021) - Parameters are invalid 
 *         - SL_STATUS_FAIL (0x0001) - The function is failed 
 *         - SL_STATUS_BUSY (0x0004) - Driver is busy
*******************************************************************************/

sl_status_t sl_si91x_ssi_transfer_data(sl_ssi_handle_t ssi_handle,
                                       const void *data_out,
                                       void *data_in,
                                       uint32_t data_length);

/***************************************************************************/ /**
 * Get the SSI status.
 *
 * @param[in] ssi handle
 * @return    busy, data lost or mode fault returns as a bit field.
 *
*******************************************************************************/
sl_ssi_status_t sl_si91x_ssi_get_status(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Get the SSI version.
 * It returns the API version of SSI.
 *
 * @param[in] None
 * @return    driver version.
 *
*******************************************************************************/
sl_ssi_version_t sl_si91x_ssi_get_version(void);

/***************************************************************************/ /**
 * Get the transfer status SSI.
 * It returns the \ref sl_ssi_status_t type structure. The members are:
 * - Busy
 * - Data Lost
 * - Mode Fault
 * It is generally used to poll the busy status of SSI Master/ULP Master.
 * 
 * @pre \ref sl_si91x_ssi_configure_clock \n
 * @pre \ref sl_si91x_ssi_init \n
 * @pre \ref sl_si91x_ssi_set_configuration \n
 * 
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return ( \ref sl_ssi_status_t) type structure
*******************************************************************************/
sl_ssi_status_t sl_si91x_ssi_get_status(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Get data receive count of the SSI.
 * It returns the number of data received at the time of function call.
 * 
 * @pre \ref sl_si91x_ssi_configure_clock \n
 * @pre \ref sl_si91x_ssi_init \n
 * @pre \ref sl_si91x_ssi_set_configuration \n
 *  
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return uint32_t value of the RX data count
*******************************************************************************/
uint32_t sl_si91x_ssi_get_rx_data_count(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Get the transmit data count of SSI.
 * It returns the number of data transmit at the time of function call
 * 
 * @pre \ref sl_si91x_ssi_configure_clock \n
 * @pre \ref sl_si91x_ssi_init \n
 * @pre \ref sl_si91x_ssi_set_configuration \n
 * 
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return uint32_t value of the tx data count
*******************************************************************************/
uint32_t sl_si91x_ssi_get_tx_data_count(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Register the user event callback.
 * It registers the callback, i.e., stores the callback function address
 * and pass to the variable that is called in Interrupt Handler.
 * If another callback is registered without unregistering previous callback then, it
 * returns an error code as follow, so it is mandatory to unregister the callback before registering
 * another callback.
 *
 * @param[in] ssi handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @param[in] callback_event Pointer to the function which needs to be called at the time of interrupt
 * @return    status 0 if successful, else error code as follow.
 *            - SL_STATUS_BUSY (0x0004) - The callback is already registered, unregister previous callback before registering new one.
 *            - SL_STATUS_OK (0x0000) - Success
 *            - SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer
 ******************************************************************************/
sl_status_t sl_si91x_ssi_register_event_callback(sl_ssi_handle_t ssi_handle, sl_ssi_signal_event_t callback_event);

/***************************************************************************/ /**
 * Unregister the user event callback.
 * It unregisters the callback, i.e., clears the callback function address
 * and passes a NULL value to the variable.
 *
 * @pre \ref sl_si91x_ssi_register_event_callback
 * @param[in] None
 ******************************************************************************/
void sl_si91x_ssi_unregister_event_callback(void);

/***************************************************************************/ /**
 * Fetch the clock division factor.
 * 
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return factor(int32_t) The value of clock division fac
 ******************************************************************************/
uint32_t sl_si91x_ssi_get_clock_division_factor(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Fetch the frame length i.e., bit width.
 * The frame length ranges between 4 and 16.
 * 
 * @param[in] none
 * @return frame_length (uint32_t) The value of frame length
 ******************************************************************************/
uint32_t sl_si91x_ssi_get_frame_length(sl_ssi_handle_t ssi_handle);

/*******************************************************************************/ /**
 * To fetch the transmit FIFO threshold value, this value controls the level of
 * entries at which the transmit FIFO controller triggers an interrupt.
 *
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return Transmit fifo threshold (uint32_t) The value of transmit fifo threshold
 ***********************************************************************************/
uint32_t sl_si91x_ssi_get_tx_fifo_threshold(sl_ssi_handle_t ssi_handle);

/******************************************************************************/ /**
 * To fetch the receiver FIFO threshold value, this value controls the level of
 * entries at which the receive FIFO controller triggers an interrupt.
 *
 * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return Receiver fifo threshold (uint32_t) The value of receiver fifo threshold
 **********************************************************************************/
uint32_t sl_si91x_ssi_get_rx_fifo_threshold(sl_ssi_handle_t ssi_handle);

/******************************************************************************/ /**
 * To fetch the receiver sample delay value, it used to delay the sample of the
 * RX input signal. Each value represents a single ssi_clk delay on the sample
 * of the rxd signal.
 *
  * @param[in] ssi_handle Pointer to the SSI driver handle ( \ref sl_ssi_handle_t)
 * @return Receiver sample delay (uint32_t) The value of receiver sample delay
 **********************************************************************************/
uint32_t sl_si91x_ssi_get_receiver_sample_delay(sl_ssi_handle_t ssi_handle);

/***************************************************************************/ /**
 * Set the secondary number in multi-secondary operation.
 * For single secondary also, this API needs to be called before transferring the
 * data.
 * 
 * @param[in] number Secondary number ( \ref sl_ssi_slave_number_t )
 * @return none
 ******************************************************************************/
__STATIC_INLINE sl_status_t sl_si91x_ssi_set_slave_number(uint8_t number)
{
  sl_status_t status;
  if (number >= SSI_SLAVE_NUMBER_LAST_ENUM) {
    status = SL_STATUS_INVALID_PARAMETER;
  } else {
    RSI_SPI_SetSlaveSelectNumber(number);
    status = SL_STATUS_OK;
  }
  return status;
}

#ifdef __cplusplus
}
#endif

#endif /* SL_SI91X_SSI_H */
