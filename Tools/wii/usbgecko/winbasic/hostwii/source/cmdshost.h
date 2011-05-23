/*
	USB Gecko SE Basic SDK Example.
 
	cmdshost.h
 
	2010 www.usbgecko.com - code by ian@unrom.com
	
	Code is Public Domain.
*/


#ifndef __CMDSHOST_H__
#define __CMDSHOST_H__

void cmd_receive4k();
void cmd_send4k();
void cmd_send64k();
unsigned int cmd_dump8mb();
unsigned int cmd_upload8mb();
void cmd_ledon();
void cmd_ledoff();

#endif // __CMDSHOST_H__
