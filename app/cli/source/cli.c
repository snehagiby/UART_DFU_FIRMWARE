/*
 * cli.c
 *
 *  Created on: Apr 2, 2026
 *      Author: gsssnehs
 */
#include "cli.h"
#include "version.h"
#include "dfu.h"

static char cli_buff[CLI_BUFFER_SIZE];
static uint8_t cli_idx;
uint8_t argc_len = 0;
char *argv_char[CLI_BUFFER_SIZE];
static bool data_received = false;

cli_cmd cmd_list[] = {
		{"version",get_app_version},
		{"dfu",set_dfu_mode},
		{NULL,NULL}
};
void cli_processchar(char ch)
{
	if((ch == '\r') ||(ch == '\n')){
		cli_buff[cli_idx] = '\0';
		if(cli_idx > 0){
		cli_process(cli_buff);
		cli_idx = 0;
		data_received = true;
		}
	}else {
		 if(cli_idx < CLI_BUFFER_SIZE-1){
			 cli_buff[cli_idx] = ch;
			 cli_idx++;
		 }
	}
}

void cli_parse(char *ptr,uint8_t *argc,char *argv[],uint8_t max_arg)
{
	while(*ptr && *argc < max_arg)						//we still have data and arguments are less than max
	{
		while(*ptr == ' ' || *ptr == '\t')						//white space and horizontal tab
		{	ptr++;}

			if(*ptr == '\0')
				return;
			argv[*argc] = ptr;
			(*argc)++;

			while(*ptr && *ptr != ' ' && *ptr !='\r' && *ptr != '\n' && *ptr != '\t' && *ptr != ',' )
				ptr++;
			if(*ptr)
			{
				*ptr = '\0';
				ptr++;
			}

		}
	}

void cli_process(char *data)
{
	if(data == NULL || strlen(data) == 0 || data[0] == '\r' || data[0] == '\n')
		return;
	uint8_t argc = 0;
	char *argv[CLI_BUFFER_SIZE];
	cli_parse(data,&argc,argv,CLI_BUFFER_SIZE);
	argc_len = argc;

	    for (int i = 0; i < argc; i++) {
	        argv_char[i] = argv[i];
	    }
}

void cli_cmd_process(void)
{
	if(data_received){
		data_received = false;

		if (argc_len == 0 || argv_char[0] == NULL) {
		        return;
		    }

		printf("received arg_len %d:%s\r\n",argc_len,argv_char[0]);
		for(int i =0 ; cmd_list[i].cmd != NULL; i++)
			{
				if(strcasecmp(argv_char[0],cmd_list[i].cmd) == 0)
				{
					cmd_list[i].func(argv_char,&argc_len);
					return;

				}
			}
			printf("command not found\r\n");
	}

}

void get_app_version(char *argv[],uint8_t *argc)
{
	printf("version request\r\n");
	printf("Starting Application (%d.%d)\n\r",MAJOR,MINOR);
}

void set_dfu_mode(char *argv[],uint8_t *argc)
{
	printf("dfu request\r\n");
	start_dfu();

	//start soft reset
	NVIC_SystemReset();
}
