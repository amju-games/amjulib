/*
		USB Gecko Test Application (Client App)
		2010 www.usbgecko.com (code by ian@unrom.com)
		
		
		For information on these functions, see the USB Gecko SE Programmers manual.
		
		This code is Public Domain. Part of the USB Gecko SE SDK.
		
		gecko.c
*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "FTD2XX.lib")
#include "FTD2XX.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

FT_HANDLE fthandle;	
FT_STATUS status;


void gecko_close()
{
	FT_Close(fthandle);
}

void gecko_purgetx()
{
	FT_Purge(fthandle,FT_PURGE_TX);
}

unsigned int gecko_init()
{

	static unsigned long read_timeout_ms = 5000;
	static unsigned long write_timeout_ms = 5000;
	static unsigned long packet_size_in = 65536;
	static unsigned char latency_timer = 1;
	
	status = FT_OpenEx("GECKUSB0", FT_OPEN_BY_SERIAL_NUMBER, &fthandle);
	if(status != FT_OK) 
	{
		printf("Error: Failed to find device.\n");
		return 0;
	}
	
	status = FT_ResetDevice(fthandle);
	if(status != FT_OK)
	{
		printf("Error: Failed to reset device\n");
		return 0;
	}
	
	status = FT_SetLatencyTimer(fthandle, latency_timer);
	if(status != FT_OK)
	{
		printf("Error: Failed to set latency timer\n");
		return 0;
	}

	status = FT_SetTimeouts(fthandle,read_timeout_ms,write_timeout_ms);
	if(status != FT_OK)
	{
		printf("Error: Failed to set USB timeouts\n");
		return 0;
	}	
	
	status = FT_Purge(fthandle,FT_PURGE_RX);
	if(status != FT_OK)
	{
		printf("Error: Failed to purge rx buffer\n");
		return 0;
	}
		
	status = FT_Purge(fthandle,FT_PURGE_TX);
	if(status != FT_OK)
	{
		printf("Error: Failed to purge tx buffer\n");
		return 0;
	}
	
	status = FT_SetUSBParameters(fthandle,packet_size_in,0);
	if(status != FT_OK)
	{
		printf("Error: Failed to set USB Parameters\n");
		return 0;
	}
	
	// Wait 150ms to allow chip to init
	Sleep(150);
	return 1;
}


unsigned int gecko_read(void *buffer, unsigned int no_bytes)
{
	unsigned long bytes_read;
	
	status = FT_Read(fthandle, buffer, no_bytes, &bytes_read);
	if (status != FT_OK) {
		return 0;
	}

	return 1;
}


unsigned int gecko_write(void *buffer, unsigned int no_bytes)
{
	unsigned long bytes_written;
	
	status = FT_Write(fthandle, buffer, no_bytes, &bytes_written);
	if (status != FT_OK) {
		return 0;
	}

	return 1;
}

unsigned int gecko_set_timeout(unsigned long read_timeout, unsigned long write_timeout)
{
	status = FT_SetTimeouts(fthandle,read_timeout,write_timeout);
	if(status != FT_OK)
	{
		return 0;
	}
	
	return 1;
}

unsigned int gecko_check_rxqueue()
{
	unsigned long rx_queue;
	
	status = FT_GetQueueStatus (fthandle, &rx_queue);
	if(status != FT_OK)
	{
		return 0;
	}
	
	return rx_queue;
}
