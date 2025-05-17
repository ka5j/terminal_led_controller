/*******************************************************************************************
 * @file    main_functions.h
 * @author  ka5j
 * @brief   Function prototypes and macros used in main.c
 * @version 1.1
 * @date    2025-05-16
 *
 * @details
 * This header provides the interface to support a modular main.c, including:
 * - USART terminal initialization
 * - GPIO output setup for user-controlled LED
 * - UART-based command parsing function
 *
 * All functions are designed to operate at register level (bare-metal) with no HAL/CMSIS.
 *******************************************************************************************/

#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "main_functions.h"
#include "stm32f446re_addresses.h" // STM32 memory and base addresses
#include "rcc_registers.h"         // RCC peripheral access macros
#include "gpio_registers.h"        // GPIO register structure definitions
#include "systick_registers.h"     // SysTick register access macros
#include "usart_registers.h"       // USART register definitions
#include "tim2_5_registers.h"      // TIM2-TIM5 register definitions
#include "bare_systick.h"          // SysTick driver (bare-metal)
#include "bare_gpio.h"             // GPIO driver (bare-metal)
#include "bare_usart.h"            // USART2 driver (bare-metal)
#include "bare_tim2_5.h"           // TIM2-TIM5 (bare-metal)

/*******************************************************************************************
 *                                       Macros
 *******************************************************************************************/
#define CMD_BUFFER_SIZE 64 /*!< Maximum number of characters allowed in UART command buffer */

/*******************************************************************************************
 *                                   Function Prototypes
 *******************************************************************************************/

/**
 * @brief  Initialize USART2 terminal interface.
 *
 * @details
 * Configures USART2 (PA2 TX / PA3 RX) for 115200 baud communication.
 * Sends terminal header, performs optional delay or input sync,
 * and prepares the device for interactive command entry.
 */
void usart_terminal_init(void);

/**
 * @brief  Initialize user LED on PC5 as GPIO output.
 *
 * @details
 * Configures GPIOC Pin 5 as push-pull output. This pin can be toggled
 * via command or from within your embedded application logic.
 */
void led1_init(void);

/**
 * @brief  Initialize user LED on PC4 as GPIO output.
 *
 * @details
 * Configures GPIOC Pin 4 as push-pull output. This pin can be controlled via PWM
 * via command or from within your embedded application logic.
 */
void led2_init(void);

/**
 * @brief  Process and execute received UART command.
 *
 * @param  cmd  Null-terminated string received from terminal.
 *
 * @details
 * Parses recognized commands like "LED ON", "LED OFF", or "STATUS"
 * and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void led1_process_cmd(const char *cmd);

/**
 * @brief  Process and execute received UART command.
 *
 * @param  cmd  Null-terminated string received from terminal.
 *
 * @details
 * Parses recognized commands like "LED ON", "LED OFF", or "STATUS"
 * and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void led2_process_cmd(const char *cmd);

/**
 * @brief  Process and execute received UART command.
 *
 * @param  cmd  Null-terminated string received from terminal.
 *
 * @details
 * Parses recognized commands like "LED ON", "LED OFF", or "STATUS"
 * and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void led3_process_cmd(const char *cmd);

/**
 * @brief  Process and execute received UART command.
 *
 * @param  cmd  Null-terminated string received from terminal.
 *
 * @details
 * Parses recognized commands and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void process_cmd(const char *cmd);

/**
 * @brief  check status of LED1.
 *
 * @param GPIOx   Pointer to GPIO peripheral
 * @param pin     GPIO pin number
 * @details
 * Parses recognized commands and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void check_led1_state(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin);

#endif /* MAIN_FUNCTIONS_H_ */
