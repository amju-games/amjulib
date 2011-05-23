/*
	USB Gecko SE API - Part of the Gecko Basic SDK
 
	gecko.c
 
	2010 www.usbgecko.com - code by ian@unrom.com
	
	All the functions below are used to interface with the USB Gecko SE device, the functions are highly optimized to use with the hardware
	and you should never have to modify them.
	
	Code is Public Domain.
*/


#include "gecko.h"


static unsigned int gecko_sendbyte(gecko_t* gecko_state, char data)
{
	unsigned int i = 0;
	
	if(gecko_state->slot == 1)
	{
		EXI_CHAN0SR = 0x000000D0;						
		EXI_CHAN0DATA = 0xB0000000 | (data << 20);	
		EXI_CHAN0CR = 0x19;			

		while((EXI_CHAN0CR) & EXI_TSTART);						
		i = EXI_CHAN0DATA;
		EXI_CHAN0SR = 0;
	}
	else if(gecko_state->slot == 2)
	{
		EXI_CHAN1SR = 0x000000D0;						
		EXI_CHAN1DATA = 0xB0000000 | (data << 20);	
		EXI_CHAN1CR = 0x19;			
	
		while((EXI_CHAN1CR) & EXI_TSTART);						
		i = EXI_CHAN1DATA;	
		EXI_CHAN1SR = 0;
	}
	
	if (i&0x04000000)
	{
		return 1;									
	}
	
    return 0;										
}


static unsigned int gecko_receivebyte(gecko_t* gecko_state, char* data)
{
	unsigned int i = 0;

	if(gecko_state->slot == 1)
	{
		EXI_CHAN0SR = 0x000000D0;			
		EXI_CHAN0DATA = 0xA0000000;		
		EXI_CHAN0CR = 0x19;		
	
		while((EXI_CHAN0CR) & EXI_TSTART);				
		i = EXI_CHAN0DATA;					
		EXI_CHAN0SR = 0;
	}
	else if(gecko_state->slot == 2)
	{
		EXI_CHAN1SR = 0x000000D0;			
		EXI_CHAN1DATA = 0xA0000000;		
		EXI_CHAN1CR = 0x19;		
	
		while((EXI_CHAN1CR) & EXI_TSTART);				
		i = EXI_CHAN1DATA;					
		EXI_CHAN1SR = 0;
	} 
	
	if (i&0x08000000)
	{
		*data = (i>>16)&0xff;
		return 1;						
	} 
	
	return 0;							
}


// return 1, it is ok to send data to PC
// return 0, FIFO full
static unsigned int gecko_checktx(gecko_t* gecko_state)
{
	unsigned int i  = 0;
	
	if(gecko_state->slot == 1)
	{
		EXI_CHAN0SR = 0x000000D0;					
		EXI_CHAN0DATA = 0xC0000000;						
		EXI_CHAN0CR = 0x09;								
	
		while((EXI_CHAN0CR) & EXI_TSTART);		
		i = EXI_CHAN0DATA;									
		EXI_CHAN0SR = 0x0;
	}
	else if(gecko_state->slot == 2)
	{
		EXI_CHAN1SR = 0x000000D0;					
		EXI_CHAN1DATA = 0xC0000000;						
		EXI_CHAN1CR = 0x09;								
	
		while((EXI_CHAN1CR) & EXI_TSTART);		
		i = EXI_CHAN1DATA;									
		EXI_CHAN1SR = 0x0;
	}

	if (i&0x04000000)
	{
		return 1;
	}
	
    return 0;										
}


// return 1, there is data in the FIFO to recieve
// return 0, FIFO is empty
static unsigned int gecko_checkrx(gecko_t* gecko_state)
{
	unsigned int i = 0;
	
	if(gecko_state->slot == 1)
	{
		EXI_CHAN0SR = 0x000000D0;						
		EXI_CHAN0DATA = 0xD0000000;					
		EXI_CHAN0CR = 0x09;								
	
		while((EXI_CHAN0CR) & EXI_TSTART);							   	
		i = EXI_CHAN0DATA;								
		EXI_CHAN0SR = 0x0;  
	}
	else if(gecko_state->slot == 2)
	{
		EXI_CHAN1SR = 0x000000D0;						
		EXI_CHAN1DATA = 0xD0000000;			
		EXI_CHAN1CR = 0x09;								
	
		while((EXI_CHAN1CR) & EXI_TSTART);							   	
		i = EXI_CHAN1DATA;								
		EXI_CHAN1SR = 0x0;
	}
	
	if (i&0x04000000)
	{
		return 1;
	}							

    return 0;										
}


static void gecko_flush()
{
 	char tempbyte;
	int i;

	for(i = 0;i < 256;i++)
	{
		gecko_receivebyte(&gecko_state, &tempbyte);
	}
}

static void ledoff(gecko_t* gecko_state)
{
	if(gecko_state->slot == 1)
	{
		EXI_CHAN0SR = 0x000000D0;					
		EXI_CHAN0DATA = 0x70000000;						
		EXI_CHAN0CR = 0x19;								
	
		while((EXI_CHAN0CR) & EXI_TSTART);												
		EXI_CHAN0SR = 0x0;
	}
	else if(gecko_state->slot == 2)
	{
		EXI_CHAN1SR = 0x000000D0;					
		EXI_CHAN1DATA = 0x70000000;						
		EXI_CHAN1CR = 0x19;								

		while((EXI_CHAN1CR) & EXI_TSTART);												
		EXI_CHAN1SR = 0x0;
	}
}

static void ledon(gecko_t* gecko_state)
{
	if(gecko_state->slot == 1)
	{
		EXI_CHAN0SR = 0x000000D0;					
		EXI_CHAN0DATA = 0x80000000;						
		EXI_CHAN0CR = 0x19;								
	
		while((EXI_CHAN0CR) & EXI_TSTART);												
		EXI_CHAN0SR = 0x0;
	}
	else if(gecko_state->slot == 2)
	{
		EXI_CHAN1SR = 0x000000D0;					
		EXI_CHAN1DATA = 0x80000000;						
		EXI_CHAN1CR = 0x19;								

		while((EXI_CHAN1CR) & EXI_TSTART);												
		EXI_CHAN1SR = 0x0;
	}
}


unsigned int gecko_init(gecko_t* gecko_state)
{
	unsigned int ret = 0;
	
	// Check slot A
	EXI_CHAN0SR = 0x000000D0;					
	EXI_CHAN0DATA = 0x90000000;	
	EXI_CHAN0CR = 0x19;								

	while((EXI_CHAN0CR) & EXI_TSTART);			
	ret = EXI_CHAN0DATA;		
	EXI_CHAN0SR = 0x0;
	
	if(ret == 0x04700000)
	{
		gecko_state->detect = 1;
		gecko_state->slot = 1;
		gecko_flush();
		return 1;
	}
	else 
	{
		gecko_state->detect = 0;
		gecko_state->slot = 0;
	}
	
	ret = 0;
	
	// Check slot B if nothing found in slot A
	EXI_CHAN1SR = 0x000000D0;					
	EXI_CHAN1DATA = 0x90000000;	
	EXI_CHAN1CR = 0x19;								

	while((EXI_CHAN1CR) & EXI_TSTART);			
	ret = EXI_CHAN1DATA;		
	EXI_CHAN1SR = 0x0;
	
 	if(ret == 0x04700000)
	{
		gecko_state->detect = 1;
		gecko_state->slot = 2;
		gecko_flush();
		return 1;
	}
	else 
	{
		gecko_state->detect = 0;
		gecko_state->slot = 0;
	}				

	return 0;									
}


void gecko_send(const void *buffer, unsigned int size)
{
	char *sendbyte = (char*) buffer;				
	unsigned int ret = 0;

	while (size  > 0)
	{
		if(gecko_checktx(&gecko_state))
		{
			ret = gecko_sendbyte(&gecko_state, *sendbyte);		
			if(ret == 1)
			{							
				sendbyte++;							
				size--;						
			}
		}
	}
}


void gecko_receive(void *buffer, unsigned int size)
{
	char *receivebyte = (char*)buffer;					
	unsigned int ret = 0;

	while (size > 0)
	{
		if(gecko_checkrx(&gecko_state))
		{
			ret = gecko_receivebyte(&gecko_state, receivebyte);	
			if(ret == 1)
			{							
				receivebyte++;							
				size--;						
			}
		}
	}
}


void gecko_ledoff()
{
	ledoff(&gecko_state);	
}


void gecko_ledon()
{
	ledon(&gecko_state);	
}
