/*******************************************************************************************
 * @file    main_functions.c
 * @author  ka5j
 * @brief   Implementation of terminal I/O and command parsing for STM32F446RE
 * @version 1.1
 * @date    2025-05-16
 *
 * @details
 * This source file contains initialization routines for the USART2 terminal and GPIO LED,
 * as well as a command parser that interprets user input from UART and maps it to hardware
 * actions (e.g., toggling PC5). All operations are performed using custom bare-metal drivers.
 *******************************************************************************************/

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
 * @brief   Initialize USART terminal interface
 *
 * @details
 * Sets up USART2 at 115200 baud for UART communication and clears the screen.
 * Prints a startup header to the connected terminal and displays an input prompt.
 *******************************************************************************************/
void usart_terminal_init(void)
{
    bare_usart_init();         // Initialize USART2 on PA2/PA3
    bare_usart_clear_screen(); // Clear terminal (ANSI escape)
    bare_usart_send_string("STM32 Terminal ready. Type commands:\r\n> ");
}

/*******************************************************************************************
 * @brief   Initialize PC5 for user-controlled LED output
 *
 * @details
 * Configures GPIOC Pin 5 as push-pull output with low speed and no pull resistors.
 * This pin is toggled by UART commands such as "LED1 ON" or "LED1 TOGGLE".
 *******************************************************************************************/
void led1_init(void)
{
    bare_gpio_init(GPIOC, GPIO_PIN5, GPIO_MODE_OUTPUT,
                   GPIO_OTYPE_PP, GPIO_SPEED_LOW, GPIO_NOPULL);
}

/*******************************************************************************************
 * @brief   Initialize user LED on PB6 as GPIO lternate function for TIM4 Channel 1 PWM.
 *
 * @details
 * Configures GPIOB Pin 6 as alternate function (AF2). This pin can be controlled via PWM
 * via command or from within your embedded application logic.
 *******************************************************************************************/
void led2_init(void)
{
    bare_gpio_AF(GPIOB, GPIO_PIN6, AF2);
    bare_tim2_5_PWM(TIM4);
}

/*******************************************************************************************
 * @brief   Parse and execute UART commands for LED1
 *
 * @param   cmd   Null-terminated command string from terminal input
 *
 * @details
 * Supports commands to control GPIOC Pin 5 (user LED):
 * - "LED1 ON"      → Sets PC5 high
 * - "LED1 OFF"     → Sets PC5 low
 * - "LED1 TOGGLE"  → Inverts PC5
 * Unrecognized commands print a default error message.
 *******************************************************************************************/
void led1_process_cmd(const char *cmd)
{
    if (strcmp(cmd, "LED1 ON") == 0)
    {
        bare_gpio_write(GPIOC, GPIO_PIN5, GPIO_PIN_SET);
        bare_usart_send_string("\nLED1 turned ON\r");
    }
    else if (strcmp(cmd, "LED1 OFF") == 0)
    {
        bare_gpio_write(GPIOC, GPIO_PIN5, GPIO_PIN_RESET);
        bare_usart_send_string("\nLED1 turned OFF\r");
    }
    else if (strcmp(cmd, "LED1 TOGGLE") == 0)
    {
        bare_gpio_toggle(GPIOC, GPIO_PIN5);
        bare_usart_send_string("\nLED1 TOGGLED\r");
    }
    else if (strcmp(cmd, "LED1 STATUS") == 0)
    {
        check_led1_state(GPIOC, GPIO_PIN5);
    }
    else
    {
        bare_usart_send_string("\nUNKNOWN COMMAND\r");
    }
}

void led2_process_cmd(const char *cmd)
{
    /***********Incomplete************/
}

void led3_process_cmd(const char *cmd)
{
    /***********Incomplete************/
}

/**
 * @brief  Process and execute received UART command.
 *
 * @param  cmd  Null-terminated string received from terminal.
 *
 * @details
 * Parses recognized commands and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void process_cmd(const char *cmd)
{
    if (cmd[3] == '1')
    {
        led1_process_cmd(cmd); // Execute command
    }
    else if (cmd[3] == '2')
    {
        led2_process_cmd(cmd); // Execute command
    }
    else if (cmd[3] == '3')
    {
        led3_process_cmd(cmd); // Execute command
    }
    else
    {
        bare_usart_send_string("\nUNKNOWN COMMAND\r");
    }

    bare_usart_send_string("\r\n> "); // Prompt for next command
}

/**
 * @brief  check status of LED1.
 *
 * @param GPIOx   Pointer to GPIO peripheral
 * @param pin     GPIO pin number
 * @details
 * Parses recognized commands and performs the corresponding hardware control. Unrecognized
 * commands print a default error message.
 */
void check_led1_state(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin)
{
    int state = bare_gpio_check_state(GPIOC, GPIO_PIN5);
    if (state)
    {
        bare_usart_send_string("\nLED1 ON\r");
    }
    else
    {
        bare_usart_send_string("\nLED1 OFF\r");
    }
}
