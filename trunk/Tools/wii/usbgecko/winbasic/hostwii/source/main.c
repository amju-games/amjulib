#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include "gecko.h"
#include "cmdshost.h"


static u32 *xfb = NULL;
static GXRModeObj *rmode = NULL;

static void Initialise() {

	u32 *framebuffer;

	VIDEO_Init();
	PAD_Init();
	
	rmode = VIDEO_GetPreferredMode(NULL);

	framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(framebuffer,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(framebuffer);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	xfb = framebuffer;
}



int main(int argc, char **argv)
{

	char gecko_cmd;
	
	Initialise();
	
	printf("\n\nUSB Gecko SE Demo Application (Host)\n");
	
	gecko_init(&gecko_state);
	
	if(gecko_state.detect == 1 && gecko_state.slot == 1)
	{
		printf("USB Gecko found in Slot A\n");
	}
	else if(gecko_state.detect == 1 && gecko_state.slot == 2)
	{
		printf("USB Gecko found in Slot B\n");
	}
	else
	{
		printf("No Device found.\n");
		while(1);
	}
	
	gecko_ledon();
	
	printf("Waiting for Command from client.\n");

	while(1) {
		
		gecko_receive(&gecko_cmd, 1);
		
		switch(gecko_cmd)
		{
			case 0x01: 
				cmd_receive4k();
				break;
				
			case 0x02: 
				cmd_send4k();
				break;	
				
			case 0x03: 
				cmd_send64k();
				break;
			
			case 0x04:
				cmd_dump8mb();
				break;
			
			case 0x06:	
				cmd_upload8mb();
				break;
		
			case 0x09:	
				cmd_ledoff();
				break;
				
			case 0x0A:	
				cmd_ledon();
				break;
		}
		

		VIDEO_WaitVSync();
		
	}

	return 0;
}


