/*******************************************************************************************
 * @file    bare_gpio.h
 * @author  ka5j
 * @brief   Bare-metal GPIO driver for STM32F446RE
 * @version 1.1
 * @date    2025-05-01
 *
 * @note    Provides high-level GPIO functionality without relying on STM32 HAL drivers.
 *******************************************************************************************/

#ifndef BARE_GPIO_H_
#define BARE_GPIO_H_

#include "stm32f446re_addresses.h" // Include low-level register definitions
#include "gpio_registers.h"
#include "rcc_registers.h"
#include <stdint.h> // Include standard integer types

/*******************************************************************************************
 * GPIO Configuration Enumerations
 *******************************************************************************************/

/**
 * @brief GPIO Ports
 */
typedef enum
{
    GPIO_PORTA = 0U,
    GPIO_PORTB = 1U,
    GPIO_PORTC = 2U,
    GPIO_PORTD = 3U,
    GPIO_PORTE = 4U,
    GPIO_PORTF = 5U,
    GPIO_PORTG = 6U,
    GPIO_PORTH = 7U
} GPIO_Ports_t;

/**
 * @brief GPIO Pins
 */
typedef enum
{
    GPIO_PIN0 = 0U,
    GPIO_PIN1 = 1U,
    GPIO_PIN2 = 2U,
    GPIO_PIN3 = 3U,
    GPIO_PIN4 = 4U,
    GPIO_PIN5 = 5U,
    GPIO_PIN6 = 6U,
    GPIO_PIN7 = 7U,
    GPIO_PIN8 = 8U,
    GPIO_PIN9 = 9U,
    GPIO_PIN10 = 10U,
    GPIO_PIN11 = 11U,
    GPIO_PIN12 = 12U,
    GPIO_PIN13 = 13U,
    GPIO_PIN14 = 14U,
    GPIO_PIN15 = 15U
} GPIO_Pins_t;

/**
 * @brief Alternate functions
 */
typedef enum
{
    AF0 = 0x0U,
    AF1 = 0x1U,
    AF2 = 0x2U,
    AF3 = 0x3U,
    AF4 = 0x4U,
    AF5 = 0x5U,
    AF6 = 0x6U,
    AF7 = 0x7U,
    AF8 = 0x8U,
    AF9 = 0x9U,
    AF10 = 0xAU,
    AF11 = 0xBU,
    AF12 = 0xCU,
    AF13 = 0xDU,
    AF14 = 0xEU,
    AF15 = 0xFU
} GPIO_AFs_t;

/**
 * @brief GPIO Pin States
 */
typedef enum
{
    GPIO_PIN_RESET = 0x00U, /*!< Pin is low */
    GPIO_PIN_SET = 0x01U    /*!< Pin is high */
} GPIO_PinState_t;

/**
 * @brief GPIO Pin Modes
 */
typedef enum
{
    GPIO_MODE_INPUT = 0x00U,  /*!< Input Floating */
    GPIO_MODE_OUTPUT = 0x01U, /*!< Output Push-Pull/Open-Drain */
    GPIO_MODE_AF = 0x02U,     /*!< Alternate Function */
    GPIO_MODE_ANALOG = 0x03U  /*!< Analog Mode */
} GPIO_Mode_t;

/**
 * @brief GPIO Output Types
 */
typedef enum
{
    GPIO_OTYPE_PP = 0x00U, /*!< Push-Pull */
    GPIO_OTYPE_OD = 0x01U  /*!< Open-Drain */
} GPIO_OType_t;

/**
 * @brief GPIO Output Speeds
 */
typedef enum
{
    GPIO_SPEED_LOW = 0x00U,    /*!< Low Speed */
    GPIO_SPEED_MEDIUM = 0x01U, /*!< Medium Speed */
    GPIO_SPEED_FAST = 0x02U,   /*!< Fast Speed */
    GPIO_SPEED_HIGH = 0x03U    /*!< High Speed */
} GPIO_Speed_t;

/**
 * @brief GPIO Pull-up/Pull-down Configurations
 */
typedef enum
{
    GPIO_NOPULL = 0x00U,  /*!< No Pull-up or Pull-down */
    GPIO_PULLUP = 0x01U,  /*!< Pull-up Enabled */
    GPIO_PULLDOWN = 0x02U /*!< Pull-down Enabled */
} GPIO_Pull_t;

/*******************************************************************************************
 * API Function Prototypes
 *******************************************************************************************/

/**
 * @brief Initialize a GPIO pin with specified configurations
 *
 * @param GPIOx   Pointer to GPIO peripheral (e.g., GPIOA, GPIOB, etc.)
 * @param pin     GPIO pin number (0-15)
 * @param mode    Mode of the GPIO pin
 * @param otype   Output type configuration
 * @param speed   Output speed configuration
 * @param pull    Pull-up/Pull-down configuration
 */
void bare_gpio_init(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin, GPIO_Mode_t mode,
                    GPIO_OType_t otype, GPIO_Speed_t speed, GPIO_Pull_t pull);

/**
 * @brief Write a HIGH or LOW value to a GPIO pin
 *
 * @param GPIOx   Pointer to GPIO peripheral
 * @param pin     GPIO pin number
 * @param state   GPIO_PIN_HIGH or GPIO_PIN_LOW
 */
void bare_gpio_write(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin, GPIO_PinState_t state);

/**
 * @brief Read the current state of a GPIO pin
 *
 * @param GPIOx   Pointer to GPIO peripheral
 * @param pin     GPIO pin number
 * @return uint8_t GPIO_PIN_HIGH or GPIO_PIN_LOW
 */
GPIO_PinState_t bare_gpio_read(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin);

/**
 * @brief Toggle the current state of a GPIO pin
 *
 * @param GPIOx   Pointer to GPIO peripheral
 * @param pin     GPIO pin number
 */
void bare_gpio_toggle(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin);

/**
 * @brief Initialize the GPIO pin to be in alternate function mode
 *
 * @param GPIOx   Pointer to GPIO peripheral
 * @param pin     GPIO pin number
 * @param AF      The alternate function number (0-15)
 */
void bare_gpio_AF(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin, GPIO_AFs_t AF);

/**
 * @brief  Enable RCC Clock for a given GPIO port
 * @param  GPIOx: pointer to GPIO peripheral base address
 * @retval None
 *
 * @note   Must be called before accessing GPIO registers.
 */
void bare_gpio_enable_clock(GPIO_TypeDef *GPIOx);

/**
 * @brief  Enable RCC Clock for a given GPIO port
 * @param  GPIOx: pointer to GPIO peripheral base address
 * @param pin     GPIO pin number
 * @return if LED on return 1 else 0
 *
 * @note   Must be called before accessing GPIO registers.
 */
int bare_gpio_check_state(GPIO_TypeDef *GPIOx, GPIO_Pins_t pin);

#endif /* BARE_GPIO_H_ */
