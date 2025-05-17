/*******************************************************************************************
 * @file    bare_tim2_5.c
 * @author  ka5j
 * @brief   Bare-metal TIM2–TIM5 driver implementation for STM32F446RE
 * @version 1.1
 * @date    2025-05-01
 *
 * @note    Provides high-level TIM2–TIM5 functionality without relying on STM32 HAL.
 *******************************************************************************************/

#include "stm32f446re_addresses.h"
#include "tim2_5_registers.h"
#include "bare_tim2_5.h"
#include "rcc_registers.h"
#include "nvic_registers.h"
#include <stdint.h>

/*******************************************************************************************
 *                               Internal Helper Functions
 *******************************************************************************************/

/**
 * @brief  Enable the RCC peripheral clock for the specified timer
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
static void bare_tim2_5_enable_clock(TIM2_5_TypeDef *TIMx)
{
    if (TIMx == TIM2)
    {
        RCC->APB1ENR |= (1 << 0);
    }
    else if (TIMx == TIM3)
    {
        RCC->APB1ENR |= (1 << 1);
    }
    else if (TIMx == TIM4)
    {
        RCC->APB1ENR |= (1 << 2);
    }
    else if (TIMx == TIM5)
    {
        RCC->APB1ENR |= (1 << 3);
    }
}

/**
 * @brief  Disable the RCC peripheral clock for the specified timer
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
static void bare_tim2_5_disable_clock(TIM2_5_TypeDef *TIMx)
{
    if (TIMx == TIM2)
    {
        RCC->APB1ENR &= ~(1 << 0);
    }
    else if (TIMx == TIM3)
    {
        RCC->APB1ENR &= ~(1 << 1);
    }
    else if (TIMx == TIM4)
    {
        RCC->APB1ENR &= ~(1 << 2);
    }
    else if (TIMx == TIM5)
    {
        RCC->APB1ENR &= ~(1 << 3);
    }
}

/**
 * @brief  Enable the NVIC interrupt for the specified timer
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
static void bare_tim2_5_enable_interrupt(TIM2_5_TypeDef *TIMx)
{
    if (TIMx == TIM2)
    {
        NVIC->ISER[0] |= (1 << 28);
    }
    else if (TIMx == TIM3)
    {
        NVIC->ISER[0] |= (1 << 29);
    }
    else if (TIMx == TIM4)
    {
        NVIC->ISER[0] |= (1 << 30);
    }
    else if (TIMx == TIM5)
    {
        NVIC->ISER[1] |= (1 << (50 - 32)); // IRQ50 is in ISER1
    }
}

/**
 * @brief  Disable the NVIC interrupt for the specified timer
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
static void bare_tim2_5_disable_interrupt(TIM2_5_TypeDef *TIMx)
{
    if (TIMx == TIM2)
    {
        NVIC->ICER[0] |= (1 << 28);
    }
    else if (TIMx == TIM3)
    {
        NVIC->ICER[0] |= (1 << 29);
    }
    else if (TIMx == TIM4)
    {
        NVIC->ICER[0] |= (1 << 30);
    }
    else if (TIMx == TIM5)
    {
        NVIC->ICER[1] |= (1 << (50 - 32)); // IRQ50 is in ICER1
    }
}

/*******************************************************************************************
 *                               Public API Functions
 *******************************************************************************************/

/**
 * @brief  Configure the specified timer's prescaler and auto-reload value
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
void bare_tim2_5_set(TIM2_5_TypeDef *TIMx)
{
    TIMx->PSC = TIM2_5_1KHZ_PRESCALER; // Set prescaler for 1 kHz timer tick
    TIMx->ARR = TIM2_5_1SEC_ARR;       // Set ARR for 1 second interval
}

/**
 * @brief  Start the specified timer (enable clock, interrupt, and counter)
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
void bare_tim2_5_start(TIM2_5_TypeDef *TIMx)
{
    bare_tim2_5_enable_clock(TIMx);     // Enable peripheral clock
    bare_tim2_5_enable_interrupt(TIMx); // Enable interrupt in NVIC
    bare_tim2_5_set(TIMx);              // Set prescaler and ARR
    TIMx->DIER |= (1 << 0);             // Enable update interrupt
    TIMx->CR1 |= (1 << 0);              // Enable counter
}

/**
 * @brief  Stop the specified timer and disable related resources
 * @param  TIMx Pointer to the TIM2–TIM5 peripheral
 */
void bare_tim2_5_stop(TIM2_5_TypeDef *TIMx)
{
    TIMx->CR1 &= ~(1 << 0);              // Disable counter
    bare_tim2_5_disable_interrupt(TIMx); // Disable NVIC interrupt
    bare_tim2_5_disable_clock(TIMx);     // Disable peripheral clock
}

/**
 * @brief Set TIM2–TIM5 timer to PWM mode in channel 1
 *
 * @param TIMx Pointer to timer peripheral (e.g., TIM2, TIM3, etc.)
 */
void bare_tim2_5_PWM(TIM2_5_TypeDef *TIMx)
{
    bare_tim2_5_enable_clock(TIMx);
    bare_tim2_5_set(TIMx);      // Set prescaler and ARR
    TIMx->CCMR1 &= ~(0x7 << 4); // Clear OC1M
    TIMx->CCMR1 |= (0x6 << 4);  // OC1M = 110 (PMW mode 1)
    TIMx->CCMR1 |= (1 << 3);    // preload enable
    TIMx->CCR1 = 0;             // Initial duty cycle 0%
    TIMx->CCER |= (1 << 0);     // Enable channel 1 output
    TIMx->CR1 |= (1 << 7);
    TIMx->EGR |= (1 << 0); // Generate update event
    TIMx->CR1 |= (1 << 0); // start timer
}

/**
 * @brief Set duty cycle of TIM2–TIM5 timer in PWM mode
 *
 * @param TIMx Pointer to timer peripheral (e.g., TIM2, TIM3, etc.)
 * @param percent Duty cycle percentage
 */
void bare_pwm_set_duty(TIM2_5_TypeDef *TIMx, uint8_t percent)
{
    if (percent > 100)
    {
        percent = 100;
    }
    TIMx->CCR1 = (TIMx->ARR * percent) / 100;
}
