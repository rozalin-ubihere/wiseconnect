/***************************************************************************/ /**
 * @file adc_example.c
 * @brief ADC example
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
#include "sl_si91x_adc.h"
#include "adc_example.h"
#include "rsi_board.h"
#include "rsi_chip.h"
#include "sl_adc_instances.h"

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/
/* Core clock modification Macros */
#define PS4_SOC_FREQ          180000000 /*<! PLL out clock 180MHz            */
#define SOC_PLL_REF_FREQUENCY 32000000  /*<! PLL input REFERENCE clock 32MHZ */
#define DVISION_FACTOR        0         // Division factor
#define CHANNEL_SAMPLE_LENGTH 1023      // Number of ADC sample collect for operation
#define ADC_MAX_OP_VALUE      4096      // Maximum output value get from adc data register
#define VREF_VALUE            3.3       // reference voltage

/*******************************************************************************
 *************************** LOCAL VARIABLES   *******************************
 ******************************************************************************/
sl_adc_config_t sl_adc_config;
uint32_t intr_cnt       = 0;
static float vref_value = VREF_VALUE;
static int16_t adc_output[CHANNEL_SAMPLE_LENGTH];
static float adc_input_eqv_vltg[CHANNEL_SAMPLE_LENGTH];
/*******************************************************************************
 **********************  Local Function prototypes   ***************************
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event);
static boolean_t chnl0_complete_flag = false;

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*******************************************************************************
 * ADC example initialization function
 ******************************************************************************/
void adc_example_init(void)
{
  sl_adc_version_t version;
  sl_status_t status;
  sl_adc_clock_config_t clock_config;
  volatile float battery_status        = 0;
  clock_config.soc_pll_clock           = PS4_SOC_FREQ;
  clock_config.soc_pll_reference_clock = SOC_PLL_REF_FREQUENCY;
  clock_config.division_factor         = DVISION_FACTOR;

  sl_adc_config.num_of_channel_enable        = 1;
  sl_adc_config.operation_mode               = SL_ADC_FIFO_MODE;
  sl_adc_channel_config.opamp_gain[0]        = 2;
  sl_adc_channel_config.rx_buf[0]            = adc_output;
  sl_adc_channel_config.chnl_ping_address[0] = ADC_PING_BUFFER; /* ADC Ping address */
  sl_adc_channel_config.chnl_pong_address[0] =
    ADC_PING_BUFFER + (sl_adc_channel_config.num_of_samples[0]); /* ADC Pong address */
  do {
    // Version information of ADC driver
    version = sl_si91x_adc_get_version();
    DEBUGOUT("ADC version is fetched successfully \n");
    DEBUGOUT("API version is %d.%d.%d\n", version.release, version.major, version.minor);
    battery_status = sl_si91x_adc_get_chip_voltage();
    if (sl_adc_config.operation_mode == 0) {
      // Configure ADC clock
      status = sl_si91x_adc_configure_clock(&clock_config);
      if (status != SL_STATUS_OK) {
        DEBUGOUT("sl_si91x_adc_clock_configuration: Error Code : %lu \n", status);
        break;
      }
      DEBUGOUT("Clock configuration is successful \n");
    }
    status = sl_si91x_adc_init(sl_adc_channel_config, sl_adc_config);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_init: Error Code : %lu \n", status);
      break;
    }
    DEBUGINIT();
    DEBUGOUT("ADC Initialization Success\n");
    /* Configure reference voltage for analog peripheral ,here till 2.8V generate by using
        AUX_LDO so more than 2.8V enable LDO bypass mode */
    status = sl_si91x_adc_configure_reference_voltage(vref_value, battery_status);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_reference_voltage_configuration: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC reference voltage configured Success\n");
    status = sl_si91x_adc_channel_set_configuration(sl_adc_channel_config, sl_adc_config);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_channel_set_configuration: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC Channel Configuration Successfully \n");
    // Register user callback function
    status = sl_si91x_adc_register_event_callback(callback_event);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_register_event_callback: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC user event callback registered successfully \n");
    status = sl_si91x_adc_start(sl_adc_config);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_start: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC started Successfully\n");
  } while (false);
}

/*******************************************************************************
 * Function will run continuously and will wait for trigger
 ******************************************************************************/
void adc_example_process_action(void)
{
  sl_status_t status;
  uint32_t sample_length;
  uint16_t adc_value;
  uint8_t chnl_num    = 0;
  volatile float vout = 0;
  if (chnl0_complete_flag) {
    chnl0_complete_flag = false;
    // ADC operation mode if FIFO then it will execute, here it will give equivalent voltage of 12 bit adc output.
    if (!sl_adc_config.operation_mode) {
      for (chnl_num = 0; chnl_num < sl_adc_config.num_of_channel_enable; chnl_num++) {
        status = sl_si91x_adc_read_data(sl_adc_channel_config, chnl_num);
        if (status != SL_STATUS_OK) {
          DEBUGOUT("sl_si91x_adc_read_data: Error Code : %lu \n", status);
        }
        for (sample_length = 0; sample_length < CHANNEL_SAMPLE_LENGTH; sample_length++) {
          if (adc_output[sample_length] & BIT(11)) {
            adc_output[sample_length] = (adc_output[sample_length] & (ADC_MASK_VALUE));
          } else {
            adc_output[sample_length] = adc_output[sample_length] | BIT(11);
          }

          vout = (((float)adc_output[sample_length] / (float)ADC_MAX_OP_VALUE) * vref_value);
          if (sl_adc_channel_config.input_type[chnl_num]) {
            vout = vout - (vref_value / 2);
          }
          adc_input_eqv_vltg[sample_length] = vout;
          DEBUGOUT("ADC Measured input[%d] :%0.2fV \n", sample_length, (float)vout);
        }
      }
    } else {
      status = sl_si91x_adc_read_data_static(sl_adc_channel_config, sl_adc_config, &adc_value);
      if (status != SL_STATUS_OK) {
        DEBUGOUT("sl_si91x_adc_read_data_static: Error Code : %lu \n", status);
      }
      adc_output[0] = adc_value;
      if (adc_output[0] & BIT(11)) {
        adc_output[0] = (int16_t)(adc_output[0] & (ADC_MASK_VALUE));
      } else {
        adc_output[0] = adc_output[0] | BIT(11);
      }

      DEBUGOUT("ADC Output :%d\n", adc_output[0]);

      vout = (((float)adc_output[0] / (float)ADC_MAX_OP_VALUE) * vref_value);

      if (sl_adc_channel_config.input_type[0]) {
        vout = vout - (vref_value / 2);
        DEBUGOUT("Differential ended input  :%lf\n", (double)vout);
      } else {
        DEBUGOUT("Single ended input :%lf\n", (double)vout);
      }
    }
  }
}

/*******************************************************************************
 * Callback event function
 * It is responsible for the event which are triggered by ADC interface
 * @param  event       : INTERNAL_DMA => Single channel data acquisition done.
 *                       ADC_STATIC_MODE_CALLBACK => Static mode adc data
 *                       acquisition done.
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event)
{
  if (event == SL_INTERNAL_DMA) {
    if (channel_no == 0) {
      chnl0_complete_flag = true;
    }

  } else if (event == SL_ADC_STATIC_MODE_EVENT) {
    chnl0_complete_flag = true;
  }
}