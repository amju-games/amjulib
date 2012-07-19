/*
	USB Gecko SE API - Part of the Gecko Basic SDK
 
	gecko.h
 
	2010 www.usbgecko.com - code by ian@unrom.com
	
	Code is Public Domain.
*/


#ifndef __GECKO_H__
#define __GECKO_H__

#define EXI_CHAN0SR			*(volatile unsigned long*) 0xCD006800 // Channel 0 Status Register
#define EXI_CHAN1SR			*(volatile unsigned long*) 0xCD006814 // Channel 1 Status Register
#define EXI_CHAN2SR			*(volatile unsigned long*) 0xCD006828 // Channel 2 Status Register
#define EXI_CHAN0CR			*(volatile unsigned long*) 0xCD00680c // Channel 0 Control Register
#define EXI_CHAN1CR			*(volatile unsigned long*) 0xCD006820 // Channel 1 Control Register
#define EXI_CHAN2CR			*(volatile unsigned long*) 0xCD006834 // Channel 2 Control Register
#define EXI_CHAN0DATA		*(volatile unsigned long*) 0xCD006810 // Channel 0 Immediate Data
#define EXI_CHAN1DATA		*(volatile unsigned long*) 0xCD006824 // Channel 1 Immediate Data
#define EXI_CHAN2DATA		*(volatile unsigned long*) 0xCD006838 // Channel 2 Immediate Data

#define EXI_TSTART			1

typedef struct gecko_s
{
	unsigned int detect;
	unsigned int slot;
} gecko_t;

gecko_t gecko_state;


unsigned int gecko_init(gecko_t* state);
void gecko_send(const void *buffer, unsigned int size);
void gecko_receive(void *buffer, unsigned int size);
void gecko_ledon();
void gecko_ledoff();


#endif // __GECKO_H__
