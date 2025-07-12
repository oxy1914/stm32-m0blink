/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 - Aleksandr Loktionov */
#include <stm32f0xx.h>
#define STM32_INIT_IMPLEMENTATION
#define STM32_INIT_NO_INTERRUPTS
#define STM32_INIT_NO_SYSTEMINIT
#include <stm32_init.h>

void dummy_loop(volatile uint32_t count)
{
	while (count--)
		;
}

static void init_gpio()
{
#ifndef MYBRD
	//no need to wait for HSIRDY no need HSI by default	RCC->CR |= 0x1;				// set HSI
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // enable GPIOA port
	GPIOA->MODER |= GPIO_MODER_MODER4_0; // set PA4 as output

//no need	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4_0; // sel PA4 low speed
//no need	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;	// set PA4 as push-pull
//no need	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_0;	// set PA4 as pull-up
#else
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN; // enable GPIOA port
	GPIOF->MODER |= GPIO_MODER_MODER0_0 |
			GPIO_MODER_MODER1_0; // set PF0 & PF1 as output
#endif /* MYBRD */
}
#ifndef MYBRD
#define LED_ON() GPIOA->BRR = GPIO_BRR_BR_4
#define LED_OFF() GPIOA->BSRR = GPIO_BSRR_BS_4
#else
#define LED_ON() GPIOF->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BR_1
#define LED_OFF() GPIOF->BSRR = GPIO_BSRR_BS_1 | GPIO_BSRR_BR_0
#endif /* MYBRD */

int main()
{
	init_gpio();

	for (;;) {
		LED_ON();
		dummy_loop(600000);
		LED_OFF();
		dummy_loop(600000);
	}

	return 0;
}
