// GB Enhanced+ Copyright Daniel Baxter 2014
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : dma.cpp
// Date : May 29, 2016
// Description : ARM946E-S/ARM7TDMI emulator
//
// Emulates the NDS9 and NDS7's 4 DMA channels
// Transfers memory to different locations

#include "arm9.h" 

/****** Performs DMA0 transfers ******/
void ARM9::dma0()
{
	//Wait 2 cycles after DMA is triggered before actual transfer
	if(mem->dma[0].delay != 0) { mem->dma[0].delay--; }

	//See if DMA Start Timing conditions dictate a transfer
	else
	{
		u32 temp_value = 0;
		u32 original_dest_addr = mem->dma[0].destination_address;

		if((mem->dma[0].control & 0x80000000) == 0) { mem->dma[0].enable = false; return; }

		//Check DMA Start Timings
		switch(((mem->dma[0].control >> 28) & 0x7))
		{
			case 0x0: std::cout<<"NDS9 DMA0 - Immediate\n"; break;
			case 0x1: std::cout<<"NDS9 DMA0 - VBlank\n"; break;
			case 0x2: std::cout<<"NDS9 DMA0 - HBlank\n"; break;
			case 0x3: std::cout<<"NDS9 DMA0 - Display Sync\n"; break;
			case 0x4: std::cout<<"NDS9 DMA0 - Main Mem Display\n"; break;
			case 0x5: std::cout<<"NDS9 DMA0 - DS Cart\n"; break;
			case 0x6: std::cout<<"NDS9 DMA0 - GBA Cart\n"; break;
			case 0x7: std::cout<<"NDS9 DMA0 - Geometry Command FIFO\n"; break;
		}

		mem->dma[0].enable = false;
	}
}

/****** Performs DMA1 transfers ******/
void ARM9::dma1()
{
	//Wait 2 cycles after DMA is triggered before actual transfer
	if(mem->dma[1].delay != 0) { mem->dma[1].delay--; }

	//See if DMA Start Timing conditions dictate a transfer
	else
	{
		u32 temp_value = 0;
		u32 original_dest_addr = mem->dma[1].destination_address;

		if((mem->dma[1].control & 0x80000000) == 0) { mem->dma[1].enable = false; return; }

		//Check DMA Start Timings
		switch(((mem->dma[1].control >> 28) & 0x3))
		{
			case 0x0: std::cout<<"NDS9 DMA1 - Immediate\n"; break;
			case 0x1: std::cout<<"NDS9 DMA1 - VBlank\n"; break;
			case 0x2: std::cout<<"NDS9 DMA1 - HBlank\n"; break;
			case 0x3: std::cout<<"NDS9 DMA1 - Display Sync\n"; break;
			case 0x4: std::cout<<"NDS9 DMA1 - Main Mem Display\n"; break;
			case 0x5: std::cout<<"NDS9 DMA1 - DS Cart\n"; break;
			case 0x6: std::cout<<"NDS9 DMA1 - GBA Cart\n"; break;
			case 0x7: std::cout<<"NDS9 DMA1 - Geometry Command FIFO\n"; break;
		}

		mem->dma[1].enable = false;
	}
}

/****** Performs DMA2 transfers ******/
void ARM9::dma2()
{
	//Wait 2 cycles after DMA is triggered before actual transfer
	if(mem->dma[2].delay != 0) { mem->dma[2].delay--; }

	//See if DMA Start Timing conditions dictate a transfer
	else
	{
		u32 temp_value = 0;
		u32 original_dest_addr = mem->dma[2].destination_address;

		if((mem->dma[2].control & 0x80000000) == 0) { mem->dma[2].enable = false; return; }

		//Check DMA Start Timings
		switch(((mem->dma[2].control >> 28) & 0x3))
		{
			case 0x0: std::cout<<"NDS9 DMA2 - Immediate\n"; break;
			case 0x1: std::cout<<"NDS9 DMA2 - VBlank\n"; break;
			case 0x2: std::cout<<"NDS9 DMA2 - HBlank\n"; break;
			case 0x3: std::cout<<"NDS9 DMA2 - Display Sync\n"; break;
			case 0x4: std::cout<<"NDS9 DMA2 - Main Mem Display\n"; break;
			case 0x5: std::cout<<"NDS9 DMA2 - DS Cart\n"; break;
			case 0x6: std::cout<<"NDS9 DMA2 - GBA Cart\n"; break;
			case 0x7: std::cout<<"NDS9 DMA2 - Geometry Command FIFO\n"; break;
		}

		mem->dma[2].enable = false;
	}
}

/****** Performs DMA3 transfers ******/
void ARM9::dma3()
{
	//Wait 2 cycles after DMA is triggered before actual transfer
	if(mem->dma[3].delay != 0) { mem->dma[3].delay--; }

	//See if DMA Start Timing conditions dictate a transfer
	else
	{
		u32 temp_value = 0;
		u32 original_dest_addr = mem->dma[3].destination_address;

		if((mem->dma[3].control & 0x80000000) == 0) { mem->dma[3].enable = false; return; }

		//Check DMA Start Timings
		switch(((mem->dma[3].control >> 28) & 0x3))
		{
			case 0x0: std::cout<<"NDS9 DMA3 - Immediate\n"; break;
			case 0x1: std::cout<<"NDS9 DMA3 - VBlank\n"; break;
			case 0x2: std::cout<<"NDS9 DMA3 - HBlank\n"; break;
			case 0x3: std::cout<<"NDS9 DMA3 - Display Sync\n"; break;
			case 0x4: std::cout<<"NDS9 DMA3 - Main Mem Display\n"; break;
			case 0x5: std::cout<<"NDS9 DMA3 - DS Cart\n"; break;
			case 0x6: std::cout<<"NDS9 DMA3 - GBA Cart\n"; break;
			case 0x7: std::cout<<"NDS9 DMA3 - Geometry Command FIFO\n"; break;
		}

		mem->dma[3].enable = false;
	}
}
