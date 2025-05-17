/*******************************************************************************************
 * @file    main.c
 * @author  ka5j
 * @brief   Bare-metal example: UART command terminal + SysTick-based LED toggling
 * @version 1.1
 * @date    2025-05-16
 *
 * @details
 * This program demonstrates a register-level embedded system using the STM32F446RE.
 * It toggles an LED (PC8) via SysTick timer interrupt and allows terminal-based
 * user interaction via USART2. Commands entered through the UART interface are parsed
 * and executed in real-time. The project uses no HAL or CMSIS and is designed for
 * bare-metal builds using a Makefile on VS Code (e.g., Raspberry Pi 5 toolchain).
 *******************************************************************************************/

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
 * @brief   Configure PC8 as output and enable SysTick interrupt for LED blinking.
 *
 * @param   GPIOx Pointer to GPIO port (e.g., GPIOC)
 * @param   pin   GPIO pin number (e.g., GPIO_PIN8)
 *
 * @note    This function initializes the LED status pin and sets up SysTick to toggle
 *          it periodically (approx. every 83ms). Used to indicate program activity.
 *******************************************************************************************/
void program_status_led(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin);

/*******************************************************************************************
 * @brief   Application entry point
 *
 * @details
 * - Initializes USART2 for serial terminal communication
 * - Initializes PC8 as output and toggles it via SysTick interrupt
 * - Enters an infinite loop waiting for user commands entered via UART
 * - Commands are parsed and executed via `process_cmd()`
 *
 * @return  int  Always returns 0 (not used in bare-metal systems)
 *******************************************************************************************/
int main(void)
{
    // Initialize USART2 and print terminal header
    usart_terminal_init();

    // Initialize PC8 LED and start periodic toggling via SysTick
    program_status_led(GPIOC, GPIO_PIN8);

    // Initialize an additional LED1 connected to PC5
    led1_init();

    // Initialize an additional LED2 connected to PC4
    led2_init();

    // Buffer to store UART command input
    char cmd_buffer[CMD_BUFFER_SIZE];
    uint8_t cmd_index = 0;

    // --- UART Command Processing Loop ---
    while (1)
    {
        // Read one character from terminal
        char c = bare_usart_read_char();

        // Echo character back to terminal
        bare_usart_send_char(c);

        // On Enter key (CR or LF), terminate string and parse command
        if (c == '\r' || c == '\n')
        {
            cmd_buffer[cmd_index] = '\0'; // Null-terminate command string
            process_cmd(cmd_buffer);      // Figure out what the command is
            cmd_index = 0;                // Reset buffer index
        }
        else if (cmd_index < CMD_BUFFER_SIZE - 1)
        {
            // Store character into buffer
            cmd_buffer[cmd_index++] = c;
        }
    }
}

/*******************************************************************************************
 * @brief   Configure GPIO pin and start SysTick timer for LED heartbeat.
 *
 * @details
 * - Initializes PC8 in push-pull output mode
 * - Starts SysTick timer to fire approx. every 83ms
 * - LED toggling is handled inside the SysTick ISR
 *******************************************************************************************/
void program_status_led(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin)
{
    bare_gpio_init(GPIOx, pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_LOW, GPIO_NOPULL);
    bare_gpio_write(GPIOx, pin, GPIO_PIN_SET); // Turn on LED
    SysTick_Init(SYSTICK_1SEC_RELOAD_16MHZ / 12, SYSTICK_PROCESSOR_CLK, SYSTICK_ENABLE_INTERRUPT);
}

/*******************************************************************************************
 * @brief   SysTick interrupt handler
 *
 * @details
 * Called every time the SysTick timer expires (approx. 83ms interval).
 * Toggles PC8 to blink LED as a program-alive indicator.
 *******************************************************************************************/
void SysTick_Handler(void)
{
    bare_gpio_toggle(GPIOC, GPIO_PIN8);
}
