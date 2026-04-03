/*
 * cli.h
 *
 *  Created on: Apr 2, 2026
 *      Author: gsssnehs
 */

#ifndef APP_CLI_INCLUDE_CLI_H_
#define APP_CLI_INCLUDE_CLI_H_

#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define CLI_BUFFER_SIZE	128
#define MAX_ARG			5
/*structure for the cli command format*/
typedef struct {
	char *cmd;
	void (*func)(char *argv[],uint8_t *argc);
}cli_cmd;
/*User defined Prototype*/
void cli_processchar(char ch);
void cli_parse(char *ptr,uint8_t *argc,char *argv[],uint8_t max_arg);
void cli_process(char *data);
void cli_cmd_process(void);
//cli command function
void get_app_version(char *argv[],uint8_t *argc);
void set_dfu_mode(char *argv[],uint8_t *argc);

#endif /* APP_CLI_INCLUDE_CLI_H_ */
