/*
	USB Gecko SE Basic SDK Example.
 
	cmdshost.c
 
	2010 www.usbgecko.com - code by ian@unrom.com
	
	Code is Public Domain.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include "gecko.h"

// Below values are the optimized packet sizes for the FTDI chip. See AN232B-03	Optimising D2XX Data Throughput.

#define FTDI_4K  0xF80 		// 3968 bytes
#define FTDI_16K 0x3E00		// 15872 bytes
#define FTDI_32K 0x7C00		// 31744 bytes
#define FTDI_60K 0xE880		// 59520 bytes
#define FTDI_64K 0xF800		// 63488 bytes

#define CMD_OK			0xAA
#define CMD_FAIL		0xFF
#define CMD_RETRY		0xBB


static unsigned int bto32(unsigned char *data)
{
	unsigned long b32;
	
	b32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
	return b32;
}


void cmd_receive4k()
{
	unsigned char packet_buff[FTDI_4K];
	
	gecko_receive(packet_buff, FTDI_4K);
	printf("4k packet received from client.\n");
}


void cmd_send4k()
{
	unsigned char packet_buff[FTDI_4K];
	unsigned int i;
	
	// Fill the buffer with 4K test data
	for(i=0;i<FTDI_4K;)
	{
		packet_buff[i] = 0xAA;
		packet_buff[i+1] = 0x55;
		i += 2;
	}
	
	gecko_send(packet_buff, FTDI_4K);
	printf("4k packet sent back to client.\n");
}


void cmd_send64k()
{
	unsigned char packet_buff[FTDI_64K];
	unsigned int i;
	
	// Fill the buffer with 64k test data
	for(i=0;i<FTDI_64K;)
	{
		packet_buff[i] = 0xAA;
		packet_buff[i+1] = 0x55;
		i += 2;
	}
	
	gecko_send(packet_buff, FTDI_64K);
	printf("64k packet sent back to client.\n");
}


unsigned int cmd_dump8mb()
{
	unsigned char start_address[4];
	unsigned char end_address[4];
	unsigned int mem_start;
	unsigned int mem_end;
	unsigned int mem_range;
	unsigned int no_packets;
	unsigned int current_packet;
	unsigned int remainder_bytes;
	

	gecko_receive(start_address,4);		
	gecko_receive(end_address,4);		
	
	mem_start = bto32(start_address);
	mem_end = bto32(end_address);

	mem_range = mem_end - mem_start;

	no_packets = mem_range / FTDI_60K;						
	remainder_bytes = mem_range % FTDI_60K;			
	
	printf("Dumping 8MB Memory, please wait.\n");	
	
	for (current_packet = 0;current_packet < no_packets;)
	{	
		gecko_send((unsigned char*)mem_start, FTDI_60K);
		mem_start += FTDI_60K;
		current_packet++;
	}
	
	if(remainder_bytes > 0) {
		gecko_send((unsigned char*)mem_start, remainder_bytes);
	}
	
	printf("8MB Memory dump complete.\n");
	return 1;
}


unsigned int cmd_upload8mb()
{
	unsigned char start_address[4];
	unsigned char end_address[4];
	unsigned int mem_start;
	unsigned int mem_end;
	unsigned int mem_range;
	unsigned int no_packets;
	unsigned int current_packet;
	unsigned int remainder_bytes;
	
	
	gecko_receive(start_address,4);		
	gecko_receive(end_address,4);		
	
	mem_start = bto32(start_address);
	mem_end = bto32(end_address);

	mem_range = mem_end - mem_start;

	// Upload to the console has a max limit of a FTDI 4K packet, this seems a limit of the USB chip.
	no_packets = mem_range / FTDI_4K;						
	remainder_bytes = mem_range % FTDI_4K;
	
	printf("Receiving 8MB data, please wait.\n");	
	
	// Full Packet loop, sends full amount of data, divided into our optimized packet size
	for (current_packet = 0;current_packet < no_packets;)
	{	
		gecko_receive((unsigned char*)mem_start, FTDI_4K);
		DCFlushRange((unsigned char*)mem_start, FTDI_4K);
		
		mem_start += FTDI_4K;
		current_packet++;
	}

	if(remainder_bytes > 0) {
		gecko_receive((unsigned char*)mem_start, remainder_bytes);
	}
	
	printf("Memory upload complete.\n");
	return 1;
}


void cmd_ledon()
{
	gecko_ledon();
	printf("LED on\n");
}


void cmd_ledoff()
{
	gecko_ledoff();
	printf("LED off\n");
}
