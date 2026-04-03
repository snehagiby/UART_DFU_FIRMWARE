/*
 * dfu.h
 *
 *  Created on: Apr 2, 2026
 *      Author: gsssnehs
 */

#ifndef APP_DFU_INCLUDE_DFU_H_
#define APP_DFU_INCLUDE_DFU_H_

#include <stdint.h>
#include <stdbool.h>

#define BOOTLOADER_ADDR		(uint32_t)0x1FFF0000
#define BOOTLOADER_RESET_HANDLER 0x1FFF0004

#define MAGIC_WORD		0xDECAFBAD				// 32bit constant to be stored at the end of SRAM so it will not be initialized by the startup code.
#define MAGIC_WORD_CLR	0xABCDEDFF


void monitor_request_bootloader(void);
void jump_to_bootloader(void);
void set_sram_magicword(uint32_t data);
void start_dfu(void);


#endif /* APP_DFU_INCLUDE_DFU_H_ */
