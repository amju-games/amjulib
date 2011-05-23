/*
		USB Gecko Special Edition Based Example Application (Client)
		2010 www.usbgecko.com (code by ian@unrom.com)

		This code is Public Domain.
		
		cmdfuncs.h
*/

#ifndef _CMDFUNCS_H_
#define _CMDFUNCS_H_

unsigned int send4k();
unsigned int receive4k();
unsigned int receive64k();
unsigned int dump8mb();
unsigned int upload8mb();
unsigned int led_on();
unsigned int led_off();

#endif
