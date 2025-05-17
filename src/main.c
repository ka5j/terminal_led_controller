/*******************************************************************************************
 * @file    main.c
 * @author  ka5j
 * @brief   Bare-metal example: SysTick-based LED toggling on STM32F446RE (GPIOC Pin 8)
 * @version 1.0
 * @date    2025-05-16
 *
 * @details
 * This example demonstrates a minimal bare-metal system using custom register-level drivers
 * to blink an LED (PC8) using the SysTick timer interrupt. It assumes no HAL/CMSIS
 * dependency and is structured for portability in a VS Code + Makefile environment.
 *******************************************************************************************/

#include <stdint.h>
#include <string.h>
#include "stm32f446re_addresses.h" // Memory base address definitions
#include "rcc_registers.h"         // RCC register macros
#include "gpio_registers.h"        // GPIO register macros
#include "systick_registers.h"     // SysTick register macros
#include "usart_registers.h"       // USART register macros
#include "bare_systick.h"          // Bare-metal SysTick driver
#include "bare_gpio.h"             // Bare-metal GPIO driver
#include "bare_usart.h"            // Bare-metal USART driver

/**
 * @brief  Configure GPIO pin as output and enable SysTick interrupt.
 *
 * Initializes GPIOC Pin with push-pull output configuration,
 * sets the output high initially, and configures the SysTick
 * timer to fire periodically (approximately every 83ms).
 *
 * @param GPIOx   Pointer to GPIO peripheral (e.g., GPIOA, GPIOB, etc.)
 * @param pin     GPIO pin number (0-15)
 */
void program_status_led(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin);

/**
 * @brief  Configure usart terminal to start reading and writing
 *
 */
void usart_terminal_init(void);

/**
 * @brief  Configure GPIO pin PC5 as output.
 *
 */
void led1_init();

/**
 * @brief  Configure GPIO pin PC5 as output.
 *
 * @param cmd String repersentation of command
 */
void process_cmd(const char *cmd);

/**
 * @brief  Application entry point.
 *
 * Initializes GPIOC Pin 8 as a digital output and configures the SysTick
 * timer to toggle the pin periodically. Intended as a sanity check for
 * GPIO + timer interrupt system on STM32F446RE.
 *
 * @return int (never returns)
 */
int main(void)
{
    usart_terminal_init();

    program_status_led(GPIOC, GPIO_PIN8); // Initialize LED GPIO and start SysTick timer

    led1_init();

    uint32_t CMD_BUFFER_SIZE = 64;
    char cmd_buffer[CMD_BUFFER_SIZE];
    uint8_t cmd_index = 0;

    while (1)
    {
        char c = bare_usart_read_char();
        bare_usart_send_char(c);

        if (c == '\r' || c == '\n')
        {
            cmd_buffer[cmd_index] = '\0';
            process_cmd(cmd_buffer);
            cmd_index = 0;
            bare_usart_send_string("\r\n>");
        }
        else if (cmd_index < CMD_BUFFER_SIZE - 1)
        {
            cmd_buffer[cmd_index++] = c;
        }
    } // Main loop does nothing; toggling handled in SysTick ISR
}

/**
 * @brief  Configure GPIO pin as output and enable SysTick interrupt.
 *
 * Initializes GPIOC Pin with push-pull output configuration,
 * sets the output high initially, and configures the SysTick
 * timer to fire periodically (approximately every 83ms).
 *
 * @param GPIOx   Pointer to GPIO peripheral (e.g., GPIOA, GPIOB, etc.)
 * @param pin     GPIO pin number (0-15)
 */
void program_status_led(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin)
{
    // Initialize PC8 as output (push-pull, low speed, no pull-up/down)
    bare_gpio_init(GPIOx, pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_LOW, GPIO_NOPULL);

    // Set PC8 high to turn on LED initially
    bare_gpio_write(GPIOx, pin, GPIO_PIN_SET);

    // Configure SysTick to trigger an interrupt approximately every 1/12 second
    SysTick_Init(SYSTICK_1SEC_RELOAD_16MHZ / 12, SYSTICK_PROCESSOR_CLK, SYSTICK_ENABLE_INTERRUPT);
}

/**
 * @brief  Configure usart terminal to start reading and writing
 *
 */
void usart_terminal_init(void)
{
    bare_usart_init();
    bare_usart_clear_screen();
    bare_usart_send_string("STM32 Terminal ready. Type commands: \r\n>");
}

/**
 * @brief  Configure GPIO pin PC5 as output.
 *
 */
void led1_init()
{
    // Initialize PC5 as output (push-pull, low speed, no pull-up/down)
    bare_gpio_init(GPIOC, GPIO_PIN5, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_LOW, GPIO_NOPULL);
}

/**
 * @brief  Configure GPIO pin PC5 as output.
 *
 * @param cmd String repersentation of command
 */
void process_cmd(const char *cmd)
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
    else
    {
        bare_usart_send_string("\nUKNOWN COMMAND\r");
    }
}

/**
 * @brief  SysTick Interrupt Service Routine (ISR).
 *
 * Toggles the output state of PC8 every time the SysTick interrupt fires.
 * This creates a periodic LED blink without requiring active polling.
 */
void SysTick_Handler(void)
{
    bare_gpio_toggle(GPIOC, GPIO_PIN8);
}
