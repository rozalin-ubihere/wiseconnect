/***************************************************************************/ /**
 * @file sl_si91x_button.c
 * @brief  Sample API functions for operating buttons (external interrupts).
 *  Please see the board header file for the definition of the BUTTON*
 *  instances used below.  The BUTTON* defines used here are designed to make
 *  this library generic such that only the board header needs to be modified
 *  to change which GPIO the buttons are on.
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

#include "rsi_chip.h"
#include "sl_common.h"
#include "sl_si91x_button.h"
#include "sl_si91x_button_pin_config.h"
#include "si91x_device.h"
#include "sl_driver_gpio.h"
#include "sl_si91x_driver_gpio.h"
#if defined(DEBUG_UART)
#include "rsi_debug.h"
#endif // DEBUG_UART

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
#define BUTTON_M4_INTR            7 // M4 Pin interrupt number
#define BUTTON_UULP_INTR          2 // UULP GPIO pin interrupt 2
#define AVL_INTR_NO               0 // available interrupt number
#define SL_SI91x_MAX_BUTTON_COUNT SL_SI91x_BUTTON_COUNT

/*******************************************************************************
 **************************   GLOBAL VARIABLES   *******************************
 ******************************************************************************/
sl_status_t status;
sl_gpio_t gpio_port_pin = { SL_BUTTON_BTN1_PORT, SL_BUTTON_BTN1_PIN };

/*******************************************************************************
 **********************  Local Function prototypes   ***************************
 ******************************************************************************/
// This stores the button state so that IRQ ISRs know when to notify buttonIsrs.
#if (SL_SI91x_BUTTON_COUNT > 0)
static int8_t buttonState[SL_SI91x_BUTTON_COUNT] = { 1, 1 };
void sl_si91x_button_internal_isr(uint8_t pin);
void sl_si91x_button_UULP_IRQHandler(uint8_t intr_no);
void sl_si91x_button_HP_IRQHandler(uint8_t intr_no);
#endif //(SL_SI91x_BUTTON_COUNT > 0)

/**
 * @brief Interrupt handler for button
 * @return  Nothing
 */
void sl_si91x_button_UULP_IRQHandler(uint8_t intr_no)
{
  UNUSED_VARIABLE(intr_no);
#if (SLI_SI91X_MCU_CONFIG_RADIO_BOARD_VER2 == 1)
  sl_si91x_button_internal_isr(SL_BUTTON_BTN0_PIN);
#elif SLI_SI91X_MCU_CONFIG_RADIO_BOARD_BASE_VER
  if (intr_no == BUTTON_UULP_INTR) {
    sl_si91x_button_internal_isr(SL_BUTTON_BTN1_PIN);
  } else {
    sl_si91x_button_internal_isr(SL_BUTTON_BTN0_PIN);
  }
#endif
}

void sl_si91x_button_HP_IRQHandler(uint8_t intr_no)
{
  UNUSED_VARIABLE(intr_no);
#if (SLI_SI91X_MCU_CONFIG_RADIO_BOARD_VER2 == 1)
  sl_si91x_button_internal_isr(SL_BUTTON_BTN1_PIN);
#endif
}

void sl_si91x_button_init(const sl_button_t *handle)
{
#if (SLI_SI91X_MCU_CONFIG_RADIO_BOARD_VER2 == 1)
  if (handle->button_number == 0U) {

    /*GPIO clock is enabled*/
    sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)ULPCLK_GPIO);

    /*UULP gpio is selected*/
    sl_si91x_gpio_select_uulp_npss_receiver(handle->pin, 1);

    /*UULP gpio pinmux is selected*/
    sl_si91x_gpio_set_uulp_npss_pin_mux(handle->pin, NPSS_GPIO_PIN_MUX_MODE0);

    /*UULP gpio pin direction is selected*/
    sl_si91x_gpio_set_uulp_npss_direction(handle->pin, (sl_si91x_gpio_direction_t)GPIO_INPUT);

    status =
      sl_si91x_gpio_driver_configure_uulp_interrupt((sl_si91x_gpio_interrupt_config_flag_t)handle->interrupt_config,
                                                    handle->pin,
                                                    (void *)&sl_si91x_button_UULP_IRQHandler);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_gpio_driver_configure_uulp_interrupt, Error code: %lu", status);
    }
    DEBUGOUT("UULP interrupt configuration is successful \n");
  }
  /*Clear interrupt to eliminate false interrupts after unmasking*/
  sl_si91x_gpio_clear_uulp_interrupt(BUTTON_UULP_INTR);

  /*un mask the NPSS GPIO interrupt*/
  sl_si91x_gpio_unmask_uulp_npss_interrupt(BUTTON_UULP_INTR);

  if (handle->button_number == 1U) {

    /*Enable clock for EGPIO module*/
    sl_si91x_gpio_driver_enable_clock((sl_si91x_gpio_select_clock_t)M4CLK_GPIO);

    /*PAD selection*/
    sl_si91x_gpio_driver_enable_pad_selection(handle->pad);

    /*Set pin Direction*/
    sl_si91x_gpio_driver_set_pin_direction(handle->port, handle->pin, (sl_si91x_gpio_direction_t)GPIO_INPUT);

    /*REN enable */
    sl_si91x_gpio_driver_enable_pad_receiver(handle->pin);

    status = sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                                BUTTON_M4_INTR,
                                                (sl_gpio_interrupt_flag_t)handle->interrupt_config,
                                                (void *)&sl_si91x_button_HP_IRQHandler,
                                                AVL_INTR_NO);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_gpio_driver_configure_interrupt, Error code: %lu", status);
    }
    DEBUGOUT("GPIO interrupt configuration is successful \n");
  }
#elif SLI_SI91X_MCU_CONFIG_RADIO_BOARD_BASE_VER
  /*GPIO clock is enabled*/
  sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)ULPCLK_GPIO);

  /*UULP gpio is selected*/
  sl_si91x_gpio_select_uulp_npss_receiver(handle->pin, 1);

  /*UULP gpio pinmux is selected*/
  sl_si91x_gpio_set_uulp_npss_pin_mux(handle->pin, NPSS_GPIO_PIN_MUX_MODE0);

  /*UULP gpio pin direction is selected*/
  sl_si91x_gpio_set_uulp_npss_direction(handle->pin, (sl_si91x_gpio_direction_t)GPIO_INPUT);

  status =
    sl_si91x_gpio_driver_configure_uulp_interrupt((sl_si91x_gpio_interrupt_config_flag_t)handle->interrupt_config,
                                                  handle->pin,
                                                  (void *)&sl_si91x_button_UULP_IRQHandler);
  if (status != SL_STATUS_OK) {
    DEBUGOUT("sl_si91x_gpio_driver_configure_uulp_interrupt, Error code: %lu", status);
  }
  DEBUGOUT("UULP interrupt configuration is successful \n");
#endif
}

int8_t sl_si91x_button_state_get(uint8_t pin)
{
  // Note: this returns the "soft" state rather than reading the port
  //  so it gives with the interrupts and their callbacks
  if (pin == SL_BUTTON_BTN0_PIN) {
    return buttonState[SL_BUTTON_BTN0_NUMBER];
  } else if (pin == SL_BUTTON_BTN1_PIN) {
    return buttonState[SL_BUTTON_BTN1_NUMBER];
  }
  //if no matching button was found
  return BUTTON_STATE_INVALID;
}

void sl_si91x_button_state_set(uint8_t pin, int8_t state)
{
  // Note: this sets the "soft" state
  //  so it gives with the interrupts and their callbacks
  uint8_t index = 0;

  if (pin == SL_BUTTON_BTN0_PIN) {
    index = SL_BUTTON_BTN0_NUMBER;
  } else if (pin == SL_BUTTON_BTN1_PIN) {
    index = SL_BUTTON_BTN1_NUMBER;
  }

  if (index < SL_SI91x_MAX_BUTTON_COUNT) {
    buttonState[index] = state;
  }
}

void sl_si91x_button_state_toggle(uint8_t pin)
{
  // Note: this toggles the "soft" state
  //  so it gives with the interrupts and their callbacks
  uint8_t index = 0;

  if (pin == SL_BUTTON_BTN0_PIN) {
    index = SL_BUTTON_BTN0_NUMBER;
  } else if (pin == SL_BUTTON_BTN1_PIN) {
    index = SL_BUTTON_BTN1_NUMBER;
  }

  if (index < SL_SI91x_MAX_BUTTON_COUNT) {
    buttonState[index] = !buttonState[index];
  }
}

int8_t sl_si91x_button_pin_state(uint8_t pin)
{
  if (pin == SL_BUTTON_BTN1_PIN)
    return RSI_EGPIO_GetPin(EGPIO, SL_BUTTON_BTN1_PORT, pin) ? BUTTON_RELEASED : BUTTON_PRESSED;
  else
    return RSI_NPSSGPIO_GetPin(pin) ? BUTTON_RELEASED : BUTTON_PRESSED;
}

/***************************************************************************/ /**
*
* DEBOUNCE operation is based upon the theory that when multiple reads in a row
* return the same value, we have passed any debounce created by the mechanical
* action of a button.  The define "DEBOUNCE" says how many reads in a row
* should return the same value.  The value '5', below, is the recommended value
* since this should require the signal to have stabalized for approximately
* 100us which should be much longer than any debounce action.
* Uncomment the following line to enable software debounce operation:
* #define DEBOUNCE 5
*
 ******************************************************************************/

//Typically, software debounce is disabled by defaulting to a value of '0'
//which will cause the preprocessor to strip out the debounce code and save
//flash space.
#ifndef DEBOUNCE
#define DEBOUNCE 0
#endif //DEBOUNCE

#if (SL_SI91x_BUTTON_COUNT > 0)
void sl_si91x_button_internal_isr(uint8_t pin)
{
  int8_t buttonStateNow;

#if (DEBOUNCE > 0)
  uint8_t buttonStatePrev;
  uint32_t debounce;
#endif //(DEBOUNCE > 0)

  buttonStateNow = sl_si91x_button_pin_state(pin);

#if (DEBOUNCE > 0)
  //read button until get "DEBOUNCE" number of consistent readings
  for (debounce = 0; debounce < DEBOUNCE; debounce = (buttonStateNow == buttonStatePrev) ? debounce + 1 : 0) {
    buttonStatePrev = buttonStateNow;
    buttonStateNow  = sl_si91x_button_pin_state(pin);
  }
#endif //(DEBOUNCE > 0)

  if (buttonStateNow != sl_si91x_button_state_get(pin)) {
    //state changed, notify app
    sl_si91x_button_isr(pin, BUTTON_PRESSED);
  } else {
    //state changed, notify app
    sl_si91x_button_isr(pin, BUTTON_RELEASED);
  }
}

SL_WEAK void sl_si91x_button_isr(uint8_t pin, int8_t state)
{
  (void)pin;
  (void)state;
}
#endif // (SL_SI91x_BUTTON_COUNT > 0)
