/*
		USB Gecko Special Edition Based Example Application (Client)
		2010 www.usbgecko.com (code by ian@unrom.com)

		This code is Public Domain.
		
		cmdfuncs.c
*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "gecko.h"

#pragma warning(disable : 4996)

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN


#define CMD_SEND4K 		0x01
#define CMD_REC4K		0x02
#define CMD_REC64K		0x03
#define CMD_DUMP8MB		0x04
#define CMD_UPLOAD8MB	0x06
#define CMD_LEDOFF		0x09	
#define CMD_LEDON		0x0A

#define CMD_OK			0xAA
#define CMD_FAIL		0xFF
#define CMD_RETRY		0xBB

// Below values are the optimized packet sizes for the FTDI chip. See AN232B-03	Optimising D2XX Data Throughput.

#define FTDI_4K  		0xF80 		// 3968 bytes
#define FTDI_16K 		0x3E00		// 15872 bytes
#define FTDI_32K 		0x7C00		// 31744 bytes
#define FTDI_60K 		0xE880		// 59520 bytes
#define FTDI_64K 		0xF800		// 63488 bytes

#define MEMORY_START	0x81000000
#define MEMORY_END		0x81800000

static void longtob(unsigned char* data, unsigned int val)
{
	data[0] = (unsigned char)(val >> 24);
	data[1] = (unsigned char)(val >> 16);
	data[2] = (unsigned char)(val >> 8);
	data[3] = (unsigned char)(val);
}


unsigned int send4k()
{
	static unsigned char cmd_send4k = CMD_SEND4K;
	unsigned char packet_buff[FTDI_4K];
	int ret;

	memset(packet_buff,0,FTDI_4K);
	
	ret = gecko_write(&cmd_send4k, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit send 4k packet command.\n");
	}
	
	ret = gecko_write(packet_buff, FTDI_4K);
	if(ret != 1) {
		printf("Error: Failed to send 4k packet.\n");
	}
	
	printf("4k packet sent.\n");
	return 1;	
}

unsigned int receive4k()
{
	static unsigned char cmd_receive4k = CMD_REC4K;
	unsigned char packet_buff[FTDI_4K];
	unsigned int ret;

	FILE *fp;

	fp = fopen("4kpacket.bin","wb");
	if(fp == 0) {
		printf("Error: Failed to create file.\n");
		return 0;
	}
	
	ret = gecko_write(&cmd_receive4k, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit receive 4k packet command.\n");
		fclose(fp);
		return 0;
	}
	
	ret = gecko_read(packet_buff, FTDI_4K);
	if(ret != 1) {
		printf("Error: Failed to receive 4k packet.\n");
		fclose(fp);
		return 0;
	}
	
	fwrite(packet_buff, 1, FTDI_4K, fp);
	fclose(fp);
	printf("4k packet received. 4kpacket.bin saved.\n");
	return 1;
}


unsigned int receive64k()
{
	static unsigned char cmd_receive64k = CMD_REC64K;
	unsigned char packet_buff[FTDI_64K];
	unsigned int ret;

	FILE *fp;

	fp = fopen("64kpacket.bin","wb");
	if(fp == 0) {
		printf("Error: Failed to create file.\n");
		fclose(fp);
		return 0;
	}
	
	ret = gecko_write(&cmd_receive64k, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit receive 64k packet command.\n");
		fclose(fp);
		return 0;
	}
	
	ret = gecko_read(packet_buff, FTDI_64K);
	if(ret != 1) {
		printf("Error: Failed to receive 64k packet.\n");
		return 0;
	}

	fwrite(packet_buff, 1, FTDI_64K, fp);
	fclose(fp);
	printf("64k packet received. 64kpacket.bin saved.\n");
	return 1;
}




unsigned int dump8mb()
{
	FILE *fp;
	
	static unsigned char cmd_send8mb = CMD_DUMP8MB;
	unsigned char mem_start[4];
	unsigned char mem_end[4];
	unsigned char packet_buff[FTDI_60K];
	unsigned int mem_range;
	unsigned int start_address = MEMORY_START;
	unsigned int end_address = MEMORY_END;
	unsigned int no_packets;
	unsigned int current_packet;
	unsigned int no_remainder_bytes;
	unsigned int ret = 0;

	longtob(mem_start,start_address);
	longtob(mem_end,end_address);
	
	fp = fopen("mem8mb.bin","wb");
	if(fp == 0) {
		printf("Error: Failed to create file.\n");
		return 0;
	}
	
	ret = gecko_write(&cmd_send8mb, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit dump 8MB command.\n");
		fclose(fp);	
		return 0;
	}
		
	ret = gecko_write(mem_start, 4);
	if(ret != 1) {
		printf("Error: Failed to send start address.\n");
		fclose(fp);	
		return 0;
	}
	
	ret = gecko_write(mem_end, 4);
	if(ret != 1) {
		printf("Error: Failed to send end address.\n");
		fclose(fp);	
		return 0;
	}
	
	mem_range = end_address - start_address;
	no_packets = mem_range / FTDI_60K;						
	no_remainder_bytes = mem_range % FTDI_60K;
	
	printf("Dumping memory to file 'mem8mb.bin' - Please wait.\n");

	for (current_packet = 0;current_packet < no_packets;)
	{
		ret = gecko_read(packet_buff, FTDI_60K);
		
		switch(ret)
		{
			case 1:
				fwrite(packet_buff, 1, FTDI_60K, fp);
				fflush(fp);
				current_packet++;
				//printf(".");
				break;

			case 0:
				printf("Error: failed to receive packet.\n");		
				fclose(fp);			
				return 0 ;
				break;
		}
	}

	if (no_remainder_bytes > 0)
	{
		ret = gecko_read(packet_buff, no_remainder_bytes);

		switch(ret)
		{
			case 1:
				// Write any remainder bytes to the file
				fwrite(packet_buff, 1, no_remainder_bytes, fp);
				break;

			case 0:
				printf("Error: failed to receive remainder packet.\n");		
				fclose(fp);			
				return 0 ;
				break;
		}
	}

	printf("\nDone. mem8mb.bin saved.\n");
	fclose(fp);
	return 1;
}


unsigned int upload8mb()
{
	static unsigned char cmd_upload8mb = CMD_UPLOAD8MB;
	unsigned char packet_buff[FTDI_4K];
	unsigned char mem_start[4];
	unsigned char mem_end[4];
	unsigned int mem_range;
	unsigned int start_address = MEMORY_START;
	unsigned int end_address = MEMORY_END;
	unsigned int no_packets;
	unsigned int current_packet;
	unsigned int no_remainder_bytes;
	unsigned int ret;
	unsigned int i;

	// fill the packet buffer with some data for testing
	for(i=0;i<FTDI_4K;)
	{
		packet_buff[i] = 0xCC;
		packet_buff[i+1] = 0x66;
		i += 2;
	}

	longtob(mem_start,start_address);
	longtob(mem_end,end_address);

	ret = gecko_write(&cmd_upload8mb, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit upload 4MB command.\n");
		return 0;
	}

	ret = gecko_write(mem_start, 4);
	if(ret != 1) {
		printf("Error: Failed to send start address.\n");
		return 0;
	}
	
	ret = gecko_write(mem_end, 4);
	if(ret != 1) {
		printf("Error: Failed to send end address.\n");
		return 0;
	}

	// 4K FTDI packet size, seems to be the limitation of the USB chip
	mem_range = end_address - start_address;
	no_packets = mem_range / FTDI_4K;						
	no_remainder_bytes = mem_range % FTDI_4K;
	
	printf("Uploading 8MB of data to host' - Please wait.\n");

	// upload full amount of packets to the host
	for (current_packet = 0;current_packet < no_packets;)
	{
		ret = gecko_write(packet_buff, FTDI_4K);
		if(ret != 1) {
			printf("Error: failed to send packet.\n");		
			return 0 ;
		}

		current_packet++;
		//printf(".");
	}

	// Remainder
	if (no_remainder_bytes > 0)
	{
		gecko_write(packet_buff, no_remainder_bytes);
	}

	printf("\nMemory upload complete.\n");
	return 1;
}


unsigned int led_on()
{
	static unsigned char cmd_ledon = CMD_LEDON;
	int ret;
	
	ret = gecko_write(&cmd_ledon, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit LED on command.\n");
		return 0;
	}

	printf("LED on.\n");
	return 1;	
}


unsigned int led_off()
{
	static unsigned char cmd_ledoff = CMD_LEDOFF;
	int ret;
	
	ret = gecko_write(&cmd_ledoff, 1);
	if(ret != 1) {
		printf("Error: Failed to transmit LED off command.\n");
		return 0;
	}

	printf("LED off.\n");
	return 1;	
}