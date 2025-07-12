/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2025 - Aleksandr Loktionov */
#ifndef _STM32_INIT_H_
#define _STM32_INIT_H_

int main(void) __attribute__((naked));

#ifdef STM32_INIT_IMPLEMENTATION

#ifndef STACK_SIZE
#define STACK_SIZE 256
#endif

#ifdef STACK_SIZE
unsigned char _stack[STACK_SIZE]; //__attribute__(section(".noinit"));
#endif

void SystemInit();
void Reset_Handler() __attribute__((naked)) __attribute((section(".init")))
__attribute((optimize("-Os")));
#ifndef STM32_INIT_NO_INTERRUPTS
void NMI_Handler() __attribute((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute((weak, alias("Default_Handler")));
void MemManage_Handler() __attribute((weak, alias("Default_Handler")));
void BusFault_Handler() __attribute((weak, alias("Default_Handler")));
void UsageFault_Handler() __attribute((weak, alias("Default_Handler")));
void SVC_Handler() __attribute((weak, alias("Default_Handler")));
void DebugMon_Handler() __attribute((weak, alias("Default_Handler")));
void PendSV_Handler() __attribute((weak, alias("Default_Handler")));
void SysTick_Handler(); //!!! __attribute((weak,alias("Default_Handler")));
void WWDG_IRQHandler() __attribute((weak, alias("Default_Handler")));
void RTC_IRQHandler() __attribute((weak, alias("Default_Handler")));
void FLASH_IRQHandler() __attribute((weak, alias("Default_Handler")));
void RCC_IRQHandler() __attribute((weak, alias("Default_Handler")));
void EXTI0_1_IRQHandler(); //!!!__attribute((weak,alias("Default_Handler")));
void Default_Handler() __attribute__((naked)) __attribute((section(".init")));

void Default_Handler()
{
	for (;;)
		__BKPT(1);
}
#endif /* STM32_INIT_NO_INTERRUPTS */

extern char _estack;
void *vectors[] __attribute__((section(".vectors"))) = {
#ifndef STACK_SIZE
	&_estack,
#else
	&_stack[STACK_SIZE],
#endif
	Reset_Handler,
#ifndef STM32_INIT_NO_INTERRUPTS
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, // reserved
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler, /* Window Watchdog interrupt	*/
	0, /* Reserved                     */
	RTC_IRQHandler, /* RTC through the EXTI line    */
	FLASH_IRQHandler, /* FLASH                        */
	RCC_IRQHandler, /* RCC                          */
	EXTI0_1_IRQHandler, /* EXTI Line 0 and 1            */
#if 0 //!!!
	EXTI2_3_IRQHandler,		/* EXTI Line 2 and 3            */
	EXTI4_15_IRQHandler,		/* EXTI Line 4 to 15            */
	0,				/* Reserved                     */
	DMA1_Channel1_IRQHandler,	/* DMA1 Channel 1               */
	DMA1_Channel2_3_IRQHandler,	/* DMA1 Channel 2 and Channel 3 */
	DMA1_Channel4_5_IRQHandler,	/* DMA1 Channel 4 and Channel 5 */
	ADC1_IRQHandler,		/* ADC1                         */
	TIM1_BRK_UP_TRG_COM_IRQHandler,	/* TIM1 Break, Update, Trigger and Commutation */
	TIM1_CC_IRQHandler,		/* TIM1 Capture Compare         */
	0,				/* Reserved                     */
	TIM3_IRQHandler,		/* TIM3                         */
	0,				/* Reserved                     */
	0,				/* Reserved                     */
	TIM14_IRQHandler,		/* TIM14                        */
	0,				/* Reserved                     */
	TIM16_IRQHandler,		/* TIM16                        */
	TIM17_IRQHandler,		/* TIM17                        */
	I2C1_IRQHandler,		/* I2C1                         */
	0,				/* Reserved                     */
	SPI1_IRQHandler,		/* SPI1                         */
	0,				/* Reserved                     */
	USART1_IRQHandler,		/* USART1                       */
#endif
#endif /* STM32_INIT_NO_INTERRUPTS */
};

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;
void Reset_Handler(void)
{
	uint32_t *p_idata = &_sidata;

	// copy .data section to SRAM
	for (uint32_t *p_data = &_sdata; p_data < &_edata;)
		*p_data++ = *p_idata++;
	// init .bss section with 0 in SRAM
	for (uint32_t *p_bss = &_sbss; p_bss < &_ebss;)
		*p_bss++ = 0;
#ifndef STM32_INIT_NO_SYSTEMINIT
	SystemInit();
#endif /* STM32_INIT_NO_SYSTEMINIT */
	main();
}
#endif /* STM32_INIT_IMPLEMENTATION */
#endif /* _STM32_INIT_H_ */
