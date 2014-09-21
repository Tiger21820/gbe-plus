// GB Enhanced Copyright Daniel Baxter 2013
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : lcd.cpp
// Date : August 16, 2014
// Description : Game Boy Advance LCD emulation
//
// Draws background, window, and sprites to screen
// Responsible for blitting pixel data and limiting frame rate

#include "lcd.h"

/****** LCD Constructor ******/
LCD::LCD()
{
	reset();
}

/****** LCD Destructor ******/
LCD::~LCD()
{
	screen_buffer.clear();
	scanline_buffer.clear();
	std::cout<<"LCD::Shutdown\n";
}

/****** Reset LCD ******/
void LCD::reset()
{
	final_screen = internal_screen = NULL;

	scanline_buffer.clear();
	screen_buffer.clear();

	lcd_clock = 0;
	lcd_mode = 0;

	frame_start_time = 0;
	frame_current_time = 0;
	fps_count = 0;
	fps_time = 0;

	current_scanline = 0;
	scanline_pixel_counter = 0;

	screen_buffer.resize(0x9600, 0);
	scanline_buffer.resize(0x100, 0);
}

/****** Initialize LCD with SDL ******/
bool LCD::init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout<<"LCD::Error - Could not initialize SDL\n";
		return false;
	}

	final_screen = SDL_SetVideoMode(240, 160, 32, SDL_SWSURFACE);
	internal_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 240, 160, 32, 0, 0, 0, 0);

	if(final_screen == NULL) { return false; }

	std::cout<<"LCD::Initialized\n";

	return true;
}

/****** Render pixels for a given scanline (per-pixel) ******/
void LCD::render_scanline()
{
	//Get current map entry for rendered pixel
	u16 tile_number = (32 * (current_scanline/8)) + (scanline_pixel_counter/8);

	//Look at the Tile Map #(tile_number), see what Tile # it points to
	u16 map_entry = mem->read_u16(0x6000800 + (tile_number * 2));

	//Get address of Tile #(map_entry)
	u32 tile_addr = 0x6004000 + (map_entry * 32);

	//Get current line of tile being rendered
	u8 current_tile_line = (current_scanline % 8);

	//Get current pixel of tile being rendered
	u8 current_tile_pixel = (scanline_pixel_counter % 8) + (current_tile_line * 8);

	//Grab the byte corresponding to (current_tile_pixel), render it as ARGB
	tile_addr += (current_tile_pixel >> 1);
	u8 raw_color = mem->read_u8(tile_addr);

	if((current_tile_pixel % 2) == 0) { raw_color &= 0xF; }
	else { raw_color >>= 4; }

	u16 color_bytes = mem->read_u16(0x5000000 + (raw_color * 2));

	u8 red = ((color_bytes & 0x1F) * 8);
	color_bytes >>= 5;

	u8 green = ((color_bytes & 0x1F) * 8);
	color_bytes >>= 5;

	u8 blue = ((color_bytes & 0x1F) * 8);

	u32 final_color =  0xFF000000 | (red << 16) | (green << 8) | (blue);

	scanline_buffer[scanline_pixel_counter] = final_color;
}

	

/****** Run LCD for one cycle ******/
void LCD::step()
{
	lcd_clock++;

	//Only draw if Forced Blank is disabled
	if((mem->memory_map[DISPCNT] & 0x80) == 0)
	{
		//Mode 0 - Scanline rendering
		if(((lcd_clock % 1232) <= 960) && (lcd_clock < 197120)) 
		{
			//Change mode
			if(lcd_mode != 0) { lcd_mode = 0; }

			//Render scanline data (per-pixel every 4 cycles)
			if((lcd_clock % 4) == 0) 
			{
				render_scanline();
				scanline_pixel_counter++;
			}
		}

		//Mode 1 - H-Blank
		else if(((lcd_clock % 1232) > 960) && (lcd_clock < 197120))
		{
			//Change mode
			if(lcd_mode != 1) 
			{ 
				lcd_mode = 1;
				scanline_pixel_counter = 0;

				//Raise HBlank interrupt
				mem->memory_map[REG_IF] |= 0x2;

				//Push scanline data to final buffer
				for(int x = 0, y = (240 * current_scanline); x < 240; x++, y++)
				{
					screen_buffer[y] = scanline_buffer[x];
				}

				//Increment scanline count
				current_scanline++;
			}
		}

		//Mode 2 - VBlank
		else
		{
			//Change mode
			if(lcd_mode != 2) 
			{ 
				lcd_mode = 2;

				//Raise VBlank interrupt
				mem->memory_map[REG_IF] |= 0x1;

				//Render final buffer
				//Lock source surface
				if(SDL_MUSTLOCK(internal_screen)){ SDL_LockSurface(internal_screen); }
				u32* out_pixel_data = (u32*)internal_screen->pixels;

				for(int a = 0; a < 0x9600; a++) { out_pixel_data[a] = screen_buffer[a]; }

				//Unlock source surface
				if(SDL_MUSTLOCK(internal_screen)){ SDL_UnlockSurface(internal_screen); }
		
				//Blit
				SDL_BlitSurface(internal_screen, 0, final_screen, 0);
				if(SDL_Flip(final_screen) == -1) { std::cout<<"LCD::Error - Could not blit\n"; }

				frame_current_time = SDL_GetTicks();
				if((frame_current_time - frame_start_time) < (1000/60)) { SDL_Delay((1000/60) - (frame_current_time - frame_start_time));}
				
				fps_count++;
				if((SDL_GetTicks() - fps_time) >= 1000) { fps_time = SDL_GetTicks(); std::cout<<"FPS : " <<  std::dec << (int)fps_count << "\n"; fps_count = 0; }
			}

			//Raise HBlank interrupt
			if((lcd_clock % 1232) == 960) 
			{ 
				current_scanline++; 
				mem->memory_map[REG_IF] |= 0x2; 
			}

			//Reset LCD clock
			if(lcd_clock == 280896) { lcd_clock = 0; current_scanline = 0; scanline_pixel_counter = 0; frame_start_time = SDL_GetTicks();}
		}
	}
}


