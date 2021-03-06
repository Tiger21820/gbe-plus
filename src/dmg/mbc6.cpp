// GB Enhanced+ Copyright Daniel Baxter 2018
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : mbc6.cpp
// Date : July 06, 2018
// Description : Game Boy Memory Bank Controller 6 I/O handling
//
// Handles reading and writing bytes to memory locations for MBC6
// Used to switch ROM and RAM banks in MBC6
// Also handles Flash reading and writing

#include "mmu.h" 

/****** Performs write operations specific to the MBC6 ******/
void DMG_MMU::mbc6_write(u16 address, u8 value)
{
	//Write to External RAM Bank 0
	if((address >= 0xA000) && (address <= 0xAFFF))
	{
		u8 bank_0 = (bank_bits & 0x7);
		if(ram_banking_enabled) { random_access_bank[bank_0][address - 0xA000] = value; }
	}

	//Write to External RAM Bank 1
	else if((address >= 0xB000) && (address <= 0xBFFF))
	{
		u8 bank_1 = ((bank_bits >> 4) & 0x7);
		if(ram_banking_enabled) { random_access_bank[bank_1][address - 0xB000] = value; }
	}

	//MBC register - Enable or Disable RAM Banking
	else if(address <= 0x3FF)
	{
		if((value & 0xF) == 0xA) 
		{ 
			if(cart.ram) { ram_banking_enabled = true; } 
		}

		else { ram_banking_enabled = false; }
	}

	//MBC register - RAM Bank 0 ID
	else if((address >= 0x400) && (address <= 0x7FF))
	{
		bank_bits &= ~0x7;
		bank_bits |= (value & 0x7);
	}

	//MBC register - RAM Bank 1 ID
	else if((address >= 0x800) && (address <= 0xBFF))
	{
		bank_bits &= ~0x70;
		bank_bits |= ((value << 4) & 0x70);
	}

	//MBC register - FLASH Control
	else if((address >= 0xC00) && (address <= 0xFFF))
	{
		if(value & 0x1) { cart.flash_cnt |= 0x1; }
		else { cart.flash_cnt &= ~0x1; }
	}

	//MBC register - FLASH enable
	else if((address >= 0x1000) && (address <= 0x1FFF))
	{
		if(value & 0x1) { cart.flash_cnt |= 0x2; }
		else { cart.flash_cnt &= ~0x2; }
	}

	//MBC register - Select ROM Bank 0
	else if((address >= 0x2000) && (address <= 0x27FF)) 
	{
		rom_bank &= ~0x7F;
		rom_bank |= (value & 0x7F);
	}

	//MBC register - Bank 0 Type
	else if((address >= 0x2800) && (address <= 0x2FFF))
	{
		if(value == 0x8) { cart.flash_cnt |= 0x4; }
		else { cart.flash_cnt &= ~0x4; }
	}

	//MBC register - Select ROM Bank 1
	else if((address >= 0x3000) && (address <= 0x37FF))
	{
		rom_bank &= ~0x7F00;
		rom_bank |= ((value << 8) & 0x7F00);
	}

	//MBC register - Bank 1 Type
	else if((address >= 0x3800) && (address <= 0x3FFF))
	{
		if(value == 0x8) { cart.flash_cnt |= 0x8; }
		else { cart.flash_cnt &= ~0x8; }
	}
}

/****** Performs read operations specific to the MBC6 ******/
u8 DMG_MMU::mbc6_read(u16 address)
{
	//Read using ROM Banking - Bank 0
	if((address >= 0x4000) && (address <= 0x5FFF))
	{
		//Read from FLASH - TODO
		if((cart.flash_cnt & 0x1) && (cart.flash_cnt & 0x4)) { return 0x0; }

		u8 bank_0 = (rom_bank & 0x7F);
		u8 real_bank = (bank_0 >> 1);

		if(bank_0 >= 4)
		{
			if(bank_0 & 0x1) { return read_only_bank[real_bank - 2][address - 0x2000]; }
			else { return read_only_bank[real_bank - 2][address - 0x4000]; }
		}

		//When reading from Bank 0-3, just use the memory map
		else
		{
			switch(bank_0)
			{
				case 0: return memory_map[address - 0x4000];
				case 1: return memory_map[address - 0x2000];
				case 2: return memory_map[address];
				case 3: return memory_map[address + 0x2000];
			}
		}
	}

	//Read using ROM Banking - Bank 1
	else if((address >= 0x6000) && (address <= 0x7FFF))
	{
		//Read from FLASH - TODO
		if((cart.flash_cnt & 0x2) && (cart.flash_cnt & 0x8)) { return 0x0; }

		u8 bank_1 = ((rom_bank >> 8) & 0x7F);
		u8 real_bank = (bank_1 >> 1);

		if(bank_1 >= 4)
		{
			if(bank_1 & 0x1) { return read_only_bank[real_bank - 2][address - 0x4000]; }
			else { return read_only_bank[real_bank - 2][address - 0x6000]; }
		}

		//When reading from Bank 0-3, just use the memory map
		else
		{
			switch(bank_1)
			{
				case 0: return memory_map[address - 0x6000];
				case 1: return memory_map[address - 0x4000];
				case 2: return memory_map[address - 0x2000];
				case 3: return memory_map[address];
			}
		}
	}

	//Read using RAM Banking - Bank 0
	else if((address >= 0xA000) && (address <= 0xAFFF))
	{
		u8 bank_0 = (bank_bits & 0x7);
		if(ram_banking_enabled) { return random_access_bank[bank_0][address - 0xA000]; }
		else { return 0x00; }
	}

	//Read using RAM Banking - Bank 1
	else if((address >= 0xB000) && (address <= 0xBFFF))
	{
		u8 bank_1 = ((bank_bits >> 4) & 0x7);
		if(ram_banking_enabled) { return random_access_bank[bank_1][address - 0xB000]; }
		else { return 0x00; }
	}
}
