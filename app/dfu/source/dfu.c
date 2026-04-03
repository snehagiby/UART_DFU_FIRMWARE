/*
 * dfu.c
 *
 *  Created on: Apr 2, 2026
 *      Author: gsssnehs
 */
#include "dfu.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include <stdio.h>
#include "usart.h"
uint32_t magic_addr __attribute__((section(".noinit")));

void jump_to_bootloader(void)
{
	void (*SysMemBootJump)(void);

	//system address defined
	volatile uint32_t addr = 0x1FFF0000;

	//disable all interrupt request and stop SysTick
	__disable_irq();


	//set the clock to default state
	HAL_RCC_DeInit();
	// IMPORTANT! Disable Systick AFTER HAL_RCC_Deinit
	// Some devices were locking up inside of HAL_RCC_DeInit
	//disable systick timer
	SysTick->CTRL = 0;
	SysTick->LOAD	= 0;
	SysTick->VAL = 0;

	HAL_DeInit();
	//clear interrupt enable register and interrupt pending register
	for(int i =0; i < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]) ; i++) {
		NVIC -> ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	}

	//jump system_memory
	SysMemBootJump = (void (*)(void))(*((uint32_t*)((addr + 4))));

	//set msp
	__set_MSP(*(uint32_t*)addr);

	// REMAP
	// SYSCFG->MEMRMP = 0x01;

	//re-enable all interupts
	__enable_irq();

	SysMemBootJump();
	while(1)
	{
		//code should not reach here
	}
}

void monitor_request_bootloader(void)
{
	//printf("0x%08lX \r\n",magic_addr);
	if(magic_addr == MAGIC_WORD)
	{
		//printf("Bootloader monitor_request_bootloader\r\n");
		magic_addr = MAGIC_WORD_CLR;
		jump_to_bootloader();

	}
}
void set_sram_magicword(uint32_t data)
{
	magic_addr = data;
	if(magic_addr == MAGIC_WORD)
	{
		printf("Application -device enters in bootloader mode\r\n");
	}
}

void start_dfu(void)
{
	printf("Application- Inside start_dfu\r\n");
	set_sram_magicword(MAGIC_WORD);				//write magic word to the SRAM
	printf("0x%08lX \r\n",magic_addr);
}
