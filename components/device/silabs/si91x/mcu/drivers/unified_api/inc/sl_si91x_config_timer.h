/***************************************************************************/ /**
 * @file sl_si91x_config_timer.h
 * @brief Config-timer API implementation
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_SI91X_CONFIG_TIMER_H
#define SL_SI91X_CONFIG_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"
#include "rsi_ct.h"

/***************************************************************************/ /**
 * @addtogroup CONFIG-TIMER Config Timer
 * @ingroup SI91X_PERIPHERAL_APIS
 * @{ 
 * 
 ******************************************************************************/

/*******************************************************************************
 ***************************  DEFINES / MACROS   ********************************
 ******************************************************************************/
#define CT CT0 ///< pointer to CT base address
// -----------------------------------------------------------------------------
// Data Types

typedef OCU_PARAMS_T sl_config_timer_ocu_params_t;        ///< Renaming OCU parameters structure type
typedef WFG_PARAMS_T sl_config_timer_wfg_config_t;        ///< Renaming WFG parameters structure type
typedef RSI_CT_CALLBACK_T sl_config_timer_pwm_callback_t; ///< Renaming MCPWM callback structure

/***************************************************************************/ /**
 * Typedef for the function pointer of the interrupt callback function
 *
 * @param callback_flag (void *) parameter for updating flag values
 ******************************************************************************/
typedef void (*sl_config_timer_callback_t)(void *callback_flag);

/// @brief Enumeration to represent timer modes
typedef enum {
  SL_COUNTER_16BIT,     ///< enum for CT 16-bit mode
  SL_COUNTER_32BIT,     ///< enum for CT 32-bit mode
  SL_COUNTER_MODE_LAST, ///< Last member of enum for validation
} sl_config_timer_mode_t;

/// @brief Enumeration to represent timer counter numbers
typedef enum {
  SL_COUNTER_0,           ///< enum for CT counter-0
  SL_COUNTER_1,           ///< enum for CT counter-1
  SL_COUNTER_NUMBER_LAST, ///< Last member of enum for validation
} sl_counter_number_t;

/// @brief Enumeration to represent timer counter directions
typedef enum {
  SL_COUNTER0_UP,             ///< CT counter-0 up-counting operation
  SL_COUNTER0_DOWN,           ///< CT counter-0 down-counting operation
  SL_COUNTER0_UP_DOWN,        ///< CT counter-0 up-down counting operation
  SL_COUNTER0_DIRECTION_LAST, ///< Last member of enum for validation
} sl_counter0_direction_t;

typedef enum {
  SL_COUNTER1_UP,             ///< CT counter-1 up-counting operation
  SL_COUNTER1_DOWN,           ///< CT counter-1 down-counting operation
  SL_COUNTER1_UP_DOWN,        ///< CT counter-1 up-down counting operation
  SL_COUNTER1_DIRECTION_LAST, ///< Last member of enum for validation
} sl_counter1_direction_t;

/// @brief Enumeration to represent timer-config parameters values
typedef enum {
  SL_COUNTER_MODE_32              = COUNTER32_BITMODE,    ///< enum for CT 32-bit mode value
  SL_COUNTER0_SOFT_RESET_ENABLE   = SOFTRESET_COUNTER_0,  ///< enum for counter-0 soft reset enable value
  SL_COUNTER0_PERIODIC_ENABLE     = PERIODIC_ENCOUNTER_0, ///< enum for counter-0 periodic mode enable value
  SL_COUNTER0_TRIGGER_ENABLE      = COUNTER0_TRIG,        ///< enum for counter-0 software trigger enable value
  SL_COUNTER0_SYNC_TRIGGER_ENABLE = COUNTER0_SYNC_TRIG,   ///< enum for counter-0 sync trigger enable value
  SL_COUNTER0_BUFFER_ENABLE       = BUF_REG0EN,           ///< enum for counter-0 buffer enable value
  SL_COUNTER0_UP_DIRECTION        = COUNTER0_UP,          ///< enum for counter-0 up-direction enable value
  SL_COUNTER0_DOWN_DIRECTION      = COUNTER0_DOWN,        ///< enum for counter-0 down-direction enable value
  SL_COUNTER0_UP_DOWN_DIRECTION   = COUNTER0_UP_DOWN,     ///< enum for counter-0 up-down direction enable value
  SL_COUNTER1_SOFT_RESET_ENABLE   = SOFTRESET_COUNTER_1,  ///< enum for counter-1 soft reset enable value
  SL_COUNTER1_PERIODIC_ENABLE     = PERIODIC_ENCOUNTER_1, ///< enum for counter-1 periodic mode enable value
  SL_COUNTER1_TRIGGER_ENABLE      = COUNTER1_TRIG,        ///< enum for counter-1 software trigger enable value
  SL_COUNTER1_SYNC_TRIGGER_ENABLE = COUNTER1_SYNC_TRIG,   ///< enum for counter-1 sync trigger enable value
  SL_COUNTER1_BUFFER_ENABLE       = BUF_REG1EN,           ///< enum for counter-1 buffer enable value
  SL_COUNTER1_UP_DIRECTION        = COUNTER1_UP,          ///< enum for counter-1 up-direction enable value
  SL_COUNTER1_DOWN_DIRECTION      = COUNTER1_DOWN,        ///< enum for counter-1 up-direction enable value
  SL_COUNTER1_UP_DOWN_DIRECTION   = COUNTER1_UP_DOWN,     ///< enum for counter-1 up-direction enable value
  SL_COUNTER_PARAM_LAST,                                  ///< Last member of enum for validation
} sl_config_timer_config_values_t;

/// @brief Enumeration to represent timer OCU-config parameters values
typedef enum {
  SL_COUNTER0_OCU_OUTPUT_ENABLE = OUTPUT_OCU_0,             ///< enum for counter-0 output enable value
  SL_COUNTER0_OCU_DMA_ENABLE    = OCU_DMA_0,                ///< enum for counter-0 OCU-DMA mode enable value
  SL_COUNTER0_OCU_8BIT_ENABLE   = OCU_8_MODE_0,             ///< enum for counter-0 OCU-8bit mode enable value
  SL_COUNTER0_OCU_SYNC_ENABLE   = OCU_SYNC_WITH_0,          ///< enum for counter-0 OCU-sync mode enable value
  SL_COUNTER1_OCU_OUTPUT_ENABLE = OUTPUT_OCU_1,             ///< enum for counter-1 output enable value
  SL_COUNTER1_OCU_DMA_ENABLE    = OCU_DMA_1,                ///< enum for counter-1 OCU-DMA mode enable value
  SL_COUNTER1_OCU_8BIT_ENABLE   = OCU_8_MODE_1,             ///< enum for counter-1 OCU-8bit mode enable value
  SL_COUNTER1_OCU_SYNC_ENABLE   = OCU_SYNC_WITH_1,          ///< enum for counter-1 OCU-sync mode enable value
  SL_OCU_OUTPUT0_TOGGLE_HIGH    = MAKE_OUTPUT_0_HIGH_SEL_0, ///< enum for counter-0 output toggle high
  SL_OCU_OUTPUT0_TOGGLE_LOW     = MAKE_OUTPUT_0_LOW_SEL_0,  ///< enum for counter-0 output toggle low
  SL_OCU_OUTPUT1_TOGGLE_HIGH    = MAKE_OUTPUT_1_HIGH_SEL_1, ///< enum for counter-1 output toggle high select value
  SL_OCU_OUTPUT1_TOGGLE_LOW     = MAKE_OUTPUT_1_LOW_SEL_1,  ///< enum for counter-1 output toggle low select value
  SL_OCU_PARAM_LAST,                                        ///< Last member of enum for validation
} sl_config_timer_ocu_config_values_t;

/// @brief Enumeration to represent various timer input events
typedef enum {
  SL_NO_EVENT,                                ///< enum for no input event
  SL_EVENT0_RISING_EDGE,                      ///< enum for input-0 rising edge event
  SL_EVENT1_RISING_EDGE,                      ///< enum for input-1 rising edge event
  SL_EVENT2_RISING_EDGE,                      ///< enum for input-2 rising edge event
  SL_EVENT3_RISING_EDGE,                      ///< enum for input-3 rising edge event
  SL_EVENT0_FALLING_EDGE,                     ///< enum for input-0 falling edge event
  SL_EVENT1_FALLING_EDGE,                     ///< enum for input-1 falling edge event
  SL_EVENT2_FALLING_EDGE,                     ///< enum for input-2 falling edge event
  SL_EVENT3_FALLING_EDGE,                     ///< enum for input-3 falling edge event
  SL_EVENT0_RISING_FALLING_EDGE,              ///< enum for input-0 rising-falling edge event
  SL_EVENT1_RISING_FALLING_EDGE,              ///< enum for input-1 rising-falling edge event
  SL_EVENT2_RISING_FALLING_EDGE,              ///< enum for input-2 rising-falling edge event
  SL_EVENT3_RISING_FALLING_EDGE,              ///< enum for input-3 rising-falling edge event
  SL_EVENT0_LEVEL0,                           ///< enum for input-0 Level-0 event
  SL_EVENT1_LEVEL0,                           ///< enum for input-0 Level-0 event
  SL_EVENT2_LEVEL0,                           ///< enum for input-0 Level-0 event
  SL_EVENT3_LEVEL0,                           ///< enum for input-0 Level-0 event
  SL_EVENT0_LEVEL1,                           ///< enum for input-0 Level-0 event
  SL_EVENT1_LEVEL1,                           ///< enum for input-0 Level-0 event
  SL_EVENT2_LEVEL1,                           ///< enum for input-0 Level-0 event
  SL_EVENT3_LEVEL1,                           ///< enum for input-0 Level-0 event
  SL_AND_EVENT,                               ///< enum for and-event
  SL_OR_EVENT,                                ///< enum for or-event
  SL_EVENT0_RISING_EDGE_AND_EVENT,            ///< enum for input-0 rising edge and-event
  SL_EVENT0_RISING_EDGE_OR_EVENT,             ///< enum for input-0 rising edge or-event
  SL_EVENT1_RISING_EDGE_AND_EVENT,            ///< enum for input-1 rising edge and-event
  SL_EVENT1_RISING_EDGE_OR_EVENT,             ///< enum for input-1 rising edge or-event
  SL_EVENT2_RISING_EDGE_AND_EVENT,            ///< enum for input-2 rising edge and-event
  SL_EVENT2_RISING_EDGE_OR_EVENT,             ///< enum for input-2 rising edge or-event
  SL_EVENT3_RISING_EDGE_AND_EVENT,            ///< enum for input-3 rising edge and-event
  SL_EVENT3_RISING_EDGE_OR_EVENT,             ///< enum for input-3 rising edge or-event
  SL_EVENT0_RISING_EDGE_REGISTERED_AND_EVENT, ///< enum for input-0 rising edge registered and-event
  SL_EVENT0_RISING_EDGE_REGISTERED_OR_EVENT,  ///< enum for input-0 rising edge registered or-event
  SL_EVENT1_RISING_EDGE_REGISTERED_AND_EVENT, ///< enum for input-1 rising edge registered and-event
  SL_EVENT1_RISING_EDGE_REGISTERED_OR_EVENT,  ///< enum for input-1 rising edge registered or-event
  SL_EVENT2_RISING_EDGE_REGISTERED_AND_EVENT, ///< enum for input-2 rising edge registered and-event
  SL_EVENT2_RISING_EDGE_REGISTERED_OR_EVENT,  ///< enum for input-2 rising edge registered or-event
  SL_EVENT3_RISING_EDGE_REGISTERED_AND_EVENT, ///< enum for input-3 rising edge registered and-event
  SL_EVENT3_RISING_EDGE_REGISTERED_OR_EVENT,  ///< enum for input-3 rising edge registered or-event
  SL_EVENT_LAST,                              ///< Last member of enum for validation
} sl_config_timer_event_t;

/// @brief Enumeration to represent various timer actions
typedef enum {
  START,       ///< enum for timer-start action
  STOP,        ///< enum for timer-stop action
  CONTINUE,    ///< enum for timer-continue action
  HALT,        ///< enum for timer-halt action
  INCREMENT,   ///< enum for timer-increment action
  CAPTURE,     ///< enum for timer-capture action
  INTERRUPT,   ///< enum for timer-interrupt action
  OUTPUT,      ///< enum for timer-output action
  ACTION_LAST, ///< Last member of enum for validation
} sl_config_timer_action_t;

/// @brief Enumeration to represent various timer interrupt flag values
typedef enum {
  SL_CT_EVENT_INTR_0_FLAG = RSI_CT_EVENT_INTR_0_l,      ///< enum for counter-0 event interrupt enable value
  SL_CT_FIFO_0_FULL_FLAG  = RSI_CT_EVENT_FIFO_0_FULL_l, ///< enum for counter-0 FIFO full interrupt enable value
  SL_CT_COUNTER_0_IS_ZERO_FLAG =
    RSI_CT_EVENT_COUNTER_0_IS_ZERO_l, ///< enum for counter-0 zero-count-value interrupt enable value
  SL_CT_COUNTER_0_IS_PEAK_FLAG =
    RSI_CT_EVENT_COUNTER_0_IS_PEAK_l,                   ///< enum for counter-0 match-value interrupt enable value
  SL_CT_EVENT_INTR_1_FLAG = RSI_CT_EVENT_INTR_1_l,      ///< enum for counter-1 event interrupt enable value
  SL_CT_FIFO_1_FULL_FLAG  = RSI_CT_EVENT_FIFO_1_FULL_l, ///< enum for counter-1 FIFO full interrupt enable value
  SL_CT_COUNTER_1_IS_ZERO_FLAG =
    RSI_CT_EVENT_COUNTER_1_IS_ZERO_l, ///< enum for counter-1 zero-count-value interrupt enable value
  SL_CT_COUNTER_1_IS_PEAK_FLAG =
    RSI_CT_EVENT_COUNTER_1_IS_PEAK_l, ///< enum for counter-1 match-value interrupt enable value
} sl_config_timer_interrupt_flags_values_t;

/// @brief Structure to hold the parameters of timer configurations
typedef struct {
  boolean_t is_counter_mode_32bit_enabled;    ///< CT mode, \ref sl_config_timer_mode_t for its values
  boolean_t is_counter0_soft_reset_enabled;   ///< counter-0 soft reset, true to enable & false to disable it
  boolean_t is_counter0_periodic_enabled;     ///< counter-0 periodic mode, true to enable & false to disable it
  boolean_t is_counter0_trigger_enabled;      ///< counter-0 software trigger, true to enable & false to disable it
  boolean_t is_counter0_sync_trigger_enabled; ///< counter-0 sync trigger, true to enable & false to disable it
  boolean_t is_counter0_buffer_enabled;       ///< counter-0 buffer, true to enable & false to disable it
  boolean_t is_counter1_soft_reset_enabled;   ///< counter-1 soft reset, true to enable & false to disable it
  boolean_t is_counter1_periodic_enabled;     ///< counter-1 periodic mode, true to enable & false to disable it
  boolean_t is_counter1_trigger_enabled;      ///< counter-1 software trigger, true to enable & false to disable it
  boolean_t is_counter1_sync_trigger_enabled; ///< counter-1 sync trigger, true to enable & false to disable it
  boolean_t is_counter1_buffer_enabled;       ///< counter-1 buffer, true to enable & false to disable it
  uint8_t counter0_direction; ///< counter-0 direction \ref sl_config_timer_counter0_direction_t for possible values
  uint8_t counter1_direction; ///< counter-1 direction \ref sl_config_timer_counter1_direction_t for possible values
} sl_config_timer_config_t;

/// @brief Structure to hold the parameters of timer output compare unit (OCU) configurations
typedef struct {
  boolean_t is_counter0_ocu_output_enabled;    ///< true to enable OCU output of counter-0, false to disable it
  boolean_t is_counter0_ocu_dma_enabled;       ///< true to enable OCU DMA support of counter-0, false to disable it
  boolean_t is_counter0_ocu_8bit_mode_enabled; ///< true to enable OCU 8-bit mode of counter-0, false to disable it
  boolean_t
    is_counter0_ocu_sync_enabled; ///< true to enable counter-0 output sync with other channel, false to disable it
  boolean_t is_counter1_ocu_output_enabled; ///< true to enable OCU output of counter-1, false to disable it
  boolean_t is_counter1_ocu_dma_enabled;    ///< true to enable OCU DMA support of counter-1, false to disable it
  boolean_t is_counter1_ocu_mode_enabled;   ///< true to enable OCU 8-bit mode of counter-1, false to disable it
  boolean_t
    is_counter1_ocu_sync_enabled; ///< true to enable counter-1 output sync with other channel, false to disable it
  boolean_t
    is_counter0_toggle_output_high_enabled;        ///< true to enable counter-0 output toggle high, false to disable it
  boolean_t is_counter0_toggle_output_low_enabled; ///< true to enable counter-0 output toggle high, false to disable it
  boolean_t
    is_counter1_toggle_output_high_enabled;        ///< true to enable counter-1 output toggle high, false to disable it
  boolean_t is_counter1_toggle_output_low_enabled; ///< true to enable counter-1 output toggle high, false to disable it
} sl_config_timer_ocu_config_t;

/// @brief Structure to hold the parameters of timer OCU control parameters like compare values, DMA state
typedef struct {
  boolean_t is_counter_number_1;            ///< CT counter number \ref sl_counter_number_t
  boolean_t is_dma_state_enabled;           ///< DMA state for counter, true to enable and false to disable it
  sl_config_timer_ocu_params_t *params;     ///< pointer to OCU control parameters structure
  sl_config_timer_pwm_callback_t *callback; ///< pointer to MCPWM callback structure
} sl_config_timer_ocu_control_t;

/// @brief Structure to hold the parameters of timer action's AND-event & OR-event configurations
typedef struct {
  uint8_t action;                        ///< CT action \ref sl_config_timer_action_t
  uint8_t and_event_counter0;            ///< AND-event for counter-0 action\ref sl_config_timer_event_t
  uint8_t or_event_counter0;             ///< OR-event for counter-0 action\ref sl_config_timer_event_t
  uint8_t and_event_valid_bits_counter0; ///< valid bits for counter-0 action AND-event, possible values 0 to 16
  uint8_t or_event_valid_bits_counter0;  ///< valid bits for counter-0 action OR-event, possible values 0 to 16
  uint8_t and_event_counter1;            ///< AND-event for counter-1 action \ref sl_config_timer_event_t
  uint8_t or_event_counter1;             ///< OR-event for counter-0 action \ref sl_config_timer_event_t
  uint8_t and_event_valid_bits_counter1; ///< valid bits for counter-0 action AND-event, possible values 0 to 16
  uint8_t or_event_valid_bits_counter1;  ///< valid bits for counter-0 action OR-event, possible values 0 to 16
} sl_config_action_event_t;

/// @brief Structure to hold various interrupt flags of config-timer
typedef struct {
  boolean_t
    is_counter0_event_interrupt_enabled; ///< true to enable counter-0 interrupt on event occurrence, false to disable it
  boolean_t
    is_counter0_fifo_full_interrupt_enabled; ///< true to enable counter-0 interrupt on FIFO full, false to disable it
  boolean_t
    is_counter0_hit_zero_interrupt_enabled; ///< true to enable counter-0 interrupt on zero count value, false to disable it
  boolean_t
    is_counter0_hit_peak_interrupt_enabled; ///< true to enable counter-0 interrupt on match-value, false to disable it
  boolean_t
    is_counter1_event_interrupt_enabled; ///< true to enable counter-1 interrupt on event occurrence, false to disable it
  boolean_t
    is_counter1_fifo_full_interrupt_enabled; ///< true to enable counter-1 interrupt on FIFO full, false to disable it
  boolean_t
    is_counter1_hit_zero_interrupt_enabled; ///< true to enable counter-1 interrupt on zero count value, false to disable it
  boolean_t
    is_counter1_hit_peak_interrupt_enabled; ///< true to enable counter-1 interrupt on match-value, false to disable it
} sl_config_timer_interrupt_flags_t;

/// @brief Structure to hold the versions of peripheral API
typedef struct {
  uint8_t release; ///< Release version number
  uint8_t major;   ///< sqa-version number
  uint8_t minor;   ///< dev-version number
} sl_config_timer_version_t;

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************/ /**
* Initializes config-timer peripheral.
* Configures its output GPIO pins.
* Configures clock as 16 MHz.
*
* @param[in]   none
* @return      none
*******************************************************************************/
void sl_si91x_config_timer_init(void);

/***************************************************************************/ /**
 * Set Config-timer mode as 32-bit or 16-bit counters.
 * In 32-bit mode Counter_1 and Counter_0 will be merged and used as a single 32 bit counter.
 * In this mode, Counter_0 modes/triggers/enables will be used.
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
 *
 * @param[in]  mode \ref sl_config_timer_mode_t for possible values
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_MODE (0x0024) - 'mode' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, timer-mode is set properly
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_mode(sl_config_timer_mode_t mode);

/***************************************************************************/ /**
 * Configures Config-timer parameters.
 * Such as 32-bit or 16-bit mode, periodic mode, Counter trigger enable, 
 * soft reset enable, buffer enable, sync trigger enable and
 * sets direction for counter0 and counter1.
 * Counter trigger enable is enabled to start the counter.
 * Sync trigger enables the counter to run/active when sync is found.
 * Buffer enable will copy buffer data to Counter Match register.
 * Soft reset valid only when the counter is in two 16 bit counters mode,
 * this resets the counter on the write.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
 *
 * @param[in]  timer_config_ptr Pointer to CT config structure \ref sl_config_timer_config_t
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer.
*-
 *         \ref SL_STATUS_INVALID_PARAMETER (0x0021) - Counter direction parameter has invalid value.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, timer configurations are set properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_configuration(sl_config_timer_config_t *timer_config_ptr);

/***************************************************************************/ /**
* Reset config-timer parameters and sets default parameter values.
* Sets 16-bit mode, sets up-counter direction
* Disables periodic mode, soft reset, buffer, sync & software trigger of counters.
*
* @param[in]   none
* @return      none
*******************************************************************************/
void sl_si91x_config_timer_reset_configuration(void);

/***************************************************************************/ /**
 * Set Config-timer OCU configurations.
 * It enables outputs in OCU mode, OCU-DMA mode,
 * channel sync with OCU outputs, 8-bit mode for OCU outputs for both counters.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref sl_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  ocu_config_ptr Pointer to CT OCU-config structure \ref sl_config_timer_ocu_config_t
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, OCU configurations are set properly.\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_ocu_configuration(sl_config_timer_ocu_config_t *ocu_config_ptr);

/***************************************************************************/ /**
* Reset config-timer OCU parameters 
* Sets 16-bit mode, sets up-counter direction
* Disables DMA mode, channel sync and 8-bit mode for OCU outputs.
*
* @param[in]   none
* @return      none
*******************************************************************************/
void sl_si91x_config_timer_reset_ocu_configuration(void);

/***************************************************************************/ /**
 * Set Config-timer OCU mode control params
 * Sets first and next threshold values for counter outputs.
 * Enable DMA support for counters.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref sl_si91x_config_timer_set_ocu_configuration();
 *
 * @param[in]  ocu_config_ptr Pointer to CT OCU-config structure \ref sl_config_timer_ocu_control_t
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer.
*-
 *        \ref SL_STATUS_OK (0x0000) - Success, OCU mode control params are set properly.\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_ocu_control(sl_config_timer_ocu_control_t *ocu_params);

/***************************************************************************/ /**
 * Set Config-timer WFG mode configurations 
 * Such as select toggle high, low and peak for counter-0 & counter-1 outputs.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*
 *
 * @param[in]  wfg_config_ptr Pointer to CT wfg-config structure \ref sl_config_timer_wfg_config_t
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, WFG mode configurations are set properly.\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_wfg_configuration(sl_config_timer_wfg_config_t *wfg_config_ptr);

/***************************************************************************/ /**
 * Set Config-timer initial count as per timer mode.
 * For 32-bit mode, counter0_initial_value is passes to count value register
 * For 32-bit mode pass counter1 initial-value as zero
 * For 16-bit mode counters, ored value of both initial value is passed to the register
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  mode \ref sl_config_timer_mode_t for possible values
 * @param[in]  counter0_initial_value (uint16_t)
 * @param[in]  counter1_initial_value (uint16_t)
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_MODE (0x0024) - 'mode' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, initial-count is set properly
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_initial_count(sl_config_timer_mode_t mode,
                                                    uint32_t counter0_initial_value,
                                                    uint32_t counter1_initial_value);

/***************************************************************************/ /**
 * Set Config-timer match-count as per timer mode and counter-number.
 * If mode is 32-bit, use counter0
 * If mode is 16-bit as per passed counter number it updates the match value
 * For 16-bit mode it takes 16-bit match-value only, else through error
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 * @param[in]  mode \ref sl_config_timer_mode_t for possible values
 * @param[in]  match_value (uint32_t)
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_MODE (0x0024) - 'mode' parameter value is invalid.
*-
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, match-value is set properly
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_match_count(sl_config_timer_mode_t mode,
                                                  sl_counter_number_t counter_number,
                                                  uint32_t match_value);

/***************************************************************************/ /**
 * Get Config-timer current count as per timer mode and counter-number.
 * Updates the count value to count_value variable input parameter.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 * @param[in]  mode \ref sl_config_timer_mode_t for possible values
 * @param[in]  count_value *(uint32_t), pointer to the variable to store count value
 *
 * @return status 0 if successful, else errorcode
 *         \ref SL_STATUS_INVALID_MODE (0x0024) - 'mode' parameter value is invalid.
*-
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter 'count_value' is null pointer.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, count-value is read properly
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_get_count(sl_config_timer_mode_t mode,
                                            sl_counter_number_t counter_number,
                                            uint32_t *count_value);

/***************************************************************************/ /**
 * Resets Config-timer counter register value.
 * Resets the Count values of selected counter.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, counter resets properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_reset_counter(sl_counter_number_t counter_number);

/***************************************************************************/ /**
 * Starts selected config-timer counter.
 * Starts the counter by enabling counter trigger
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration(),keep software trigger disable here
*
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, timer started properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_start_on_software_trigger(sl_counter_number_t counter_number);

/***************************************************************************/ /**
 * Selects config timer input events for triggering selected timer-actions. 
 * Different timer actions are start, stop, continue, halt, increment, capture, 
 * interrupt and output.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref sl_si91x_config_timer_set_configuration(), keep software trigger disable here
*-
 *      \ref sl_si91x_config_timer_register_callback(), keep event interrupt flag enable for
 *      respective counter
 *
 * @param[in]  action \ref sl_config_timer_action_t for possible values
 * @param[in]  select_event_counter0 \ref sl_config_timer_event_t for possible values,
 *             (selects input event for triggering counter-0 action  )
 * @param[in]  select_event_counter1 \ref sl_config_timer_event_t for possible values,
 *             (selects input event for triggering counter-1 action  )
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - Selected input event or action  parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, input event is set properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_select_action_event(sl_config_timer_action_t action,
                                                      sl_config_timer_event_t select_event_counter0,
                                                      sl_config_timer_event_t select_event_counter1);

/***************************************************************************/ /**
 * Configure config timer input-event's AND-event and OR-event for triggering
 * selected timer-action.
 * Different timer actions are start, stop, continue, halt, increment, capture, 
 * interrupt and output.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref sl_si91x_config_timer_set_configuration(), keep software trigger disable here
 *      \ref sl_si91x_config_timer_register_callback(), keep event interrupt flag enable for
 *      respective counter
 *      \ref sl_si91x_config_timer_select_action_event(), first selects the input event for
 *      respective action for respective counter
 *
 * @param[in]  event_config_handle Pointer to CT configure action events structure
 *             \ref sl_config_action_event_t
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter is null pointer.
*-
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - and-event or or-event or
 *                                                    event-valid-bits value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, AND event & OR event set properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_configure_action_event(sl_config_action_event_t *event_config_handle);

/***************************************************************************/ /**
 * Register callback of config timer interrupts.
 * Enabling respective interrupts as per selected interrupt flag.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref sl_si91x_config_timer_set_configuration(), keep software trigger disable here
 *      \ref sl_si91x_config_timer_unregister_timeout_callback(), if already registered for any interrupt
 *
 *
 * @param[in]  on_config_timer_callback (function pointer) Callback function pointer
 *             to be called when timer interrupt occurred \ref sl_config_timer_callback_t
 * @param[in]  callback_flag (void *) pointer to interrupt flag value variable \ref sl_config_timer_callback_t
 * @param[in]  interrupt_flags pointer to interrupt flags structure \ref sl_config_timer_interrupt_flags_t
 * 
 * @return status 0 if successful, else error-code as follow
 *         SL_STATUS_NULL_POINTER (0x0022) - parameter is a null pointer.
*-
 *         SL_STATUS_BUSY (0x0004) - The callback is already registered, unregister
 *                                   previous callback before registering new one.
*-
 *         SL_STATUS_OK (0x0000) - Successfully registered timer timer-out callback.
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_register_callback(sl_config_timer_callback_t on_config_timer_callback,
                                                    void *callback_flag_value,
                                                    sl_config_timer_interrupt_flags_t *interrupt_flags);

/***************************************************************************/ /**
* Unregisters timer interrupt callback
* Disables interrupts as per selected interrupt flag.
*
* @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_register_callback(), first register particular interrupt flag
*
* @param[in]  interrupt_flags pointer to interrupt flags structure \ref sl_config_timer_interrupt_flags_t
*
* @return status 0 if successful, else error-code as follow
*         SL_STATUS_NULL_POINTER (0x0022) - parameter is a null pointer.
*-
*         SL_STATUS_OK (0x0000) - Successfully unregistered timer interrupt callback.
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_unregister_callback(sl_config_timer_interrupt_flags_t *interrupt_flags);

/***************************************************************************/ /**
 * Resume halt operation of Config-timer counter.
 * Counter will start running from the count where it was halted previously.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref sl_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, halt operation resumed properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_resume_halt_event(sl_counter_number_t counter_number);

/***************************************************************************/ /**
 * Gets Config-timer count value when capture occurs
 * Updates the capture count value of timer to capture_value variable input parameter.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*-
 *      \ref sl_si91x_config_timer_select_action_event(), first select events for capture action
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 * @param[in]  capture_value (uint16_t*), pointer to the variable to store count
 *             value at capture event.
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_NULL_POINTER (0x0022) - The parameter 'capture_value' is null pointer.
*-
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, capture count-value is read properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_read_capture(sl_counter_number_t counter_number, uint16_t *capture_value);

/***************************************************************************/ /**
 * Sync counter output with other channels, as per sync_counter_value.
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  counter_number \ref sl_counter_number_t for possible values
 * @param[in]  sync_counter (uint8_t)
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'counter_number' parameter value is invalid.
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, counter output synced properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_counter_sync(sl_counter_number_t counter_number, uint8_t sync_counter_value);

/***************************************************************************/ /**
 * This API is used to configure config timer output events for ADC trigger
 * Configures input events for counter0 and counter1 outputs to trigger ADC
 *
 * @pre Pre-conditions:
 * - \ref sl_si91x_config_timer_init();
*-
 *      \ref l_si91x_config_timer_set_configuration();
*
 *
 * @param[in]  pin1 Counter0 output event for ADC trigger (0 to 31) \ref sl_config_timer_event_t
 * @param[in]  pin2 Counter1 output event for ADC trigger (0 to 31) \ref sl_config_timer_event_t
 *
 * @return status 0 if successful, else error-code as follow
 *         \ref SL_STATUS_INVALID_PARAMETER 0x0021) - 'pin_1' or 'pin_2' parameter value is invalid.\n
*-
 *         \ref SL_STATUS_OK (0x0000) - Success, ADC trigger events set properly\n
*
*******************************************************************************/
sl_status_t sl_si91x_config_timer_set_output_adc_pin(uint8_t pin1, uint8_t pin2);

//todo: usecase not clear
sl_status_t sl_si91x_config_timer_set_wfg_compare_values(sl_counter_number_t counter_number,
                                                         sl_config_timer_ocu_params_t *ocu_params);

/***************************************************************************/ /**
* De-initialize config-timer by disabling its clock.
* @param[in]   none
* @return      none
* @note Unregisters the callback and disables all config timer interrupts.
*******************************************************************************/
void sl_si91x_config_timer_deinit(void);

/***************************************************************************/ /**
 * Gets Config timer version
 * It returns API version of Config timer
 *
 * @param[in] none
 * @return (sl_config_version_t) type structure
 ******************************************************************************/
sl_config_timer_version_t sl_si91x_config_timer_get_version(void);

/** @} (end addtogroup Config-timer) */

#ifdef __cplusplus
}
#endif

#endif /* SL_SI91X_CONFIG_TIMER_H */
