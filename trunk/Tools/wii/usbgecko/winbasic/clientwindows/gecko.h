/*
		USB Gecko Special Edition Based Example Application (Client)
		2010 www.usbgecko.com
		
		This software is Public Domain.
		
		gecko.h
*/

#ifndef _GECKO_H_
#define _GECKO_H_

unsigned int gecko_init();
void gecko_close();
unsigned int gecko_read(void *buffer, int no_bytes);
unsigned int gecko_write(void *buffer, int no_bytes);
void geck_set_timeouts(int read_timeout, int write_timeout);


#endif