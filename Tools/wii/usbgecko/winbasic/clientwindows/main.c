/*
		USB Gecko Test Application (Client App)
		2010 www.usbgecko.com (code by ian@unrom.com)
		
		
		For information on these functions, see the USB Gecko SE Programmers manual.
		
		This code is Public Domain. Part of the USB Gecko SE SDK.
		
		main.c
*/

#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include "gecko.h"
#include "cmdfuncs.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

static void show_usage(char* filename)
{
	printf("Usage:    %s <option>\n",filename);
	printf("Options:    -a send a 4K packet\n");
	printf("            -b receive a 4K packet\n");
	printf("            -c receive a 64K packet\n");
	printf("            -d dump 8MB using 64K packets\n");
	printf("            -e upload 8MB using 4K packets\n");
	printf("            -f Turn on device LED\n");
	printf("            -g Turn off device LED\n");
	
	printf("Example: %s -c \n", filename);
	printf("All receive functions will save files in the working directory.\n");
}

int _tmain(int argc, char* argv[])
{
	unsigned int ret = 0;
	
	printf("USB Gecko SE Basic Test Application\n");
	
	ret = gecko_init();
	
	if(ret == 0) {
		gecko_close();
		return(-1);
	}
	
	if(argc < 2)
 	{
		show_usage(argv[0]);
		gecko_close();
		return (-1);
	}
	
	if(argv[1][0] != '-')
	{
		show_usage(argv[0]);
		gecko_close();
		return (-1);
	}

	switch(argv[1][1])
	{
		case 'a':
		case 'A':
			send4k();
			break;
		
		case 'b':
		case 'B':
			receive4k();
			break;
		
		case 'c':
		case 'C':
			receive64k();
			break;
				
		case 'd':
		case 'D':
			dump8mb();
			break;

		case 'e':
		case 'E':
			upload8mb();
			break;

		case 'f':
		case 'F':
			led_on();
			break;

		case 'g':
		case 'G':
			led_off();
			break;
		
		default:
			show_usage(argv[0]);
			return (-1);
	}

	gecko_close();
	return (-1);
}

