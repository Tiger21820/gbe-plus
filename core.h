// GB Enhanced+ Copyright Daniel Baxter 2014
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : core.h
// Date : August 16, 2014
// Description : Emulator core
//
// The "Core" is an abstraction for all of emulated components
// It controls the large-scale behavior of the CPU, LCD/GPU, MMU, and APU/DSP
// Can start, stop, and reset emulator
// Also contains a debugging unit


#ifndef EMU_CORE
#define EMU_CORE

#include "mmu.h"
#include "arm7.h"

class Core
{
	public:
		Core();
		void start();
		void stop();

		void debug_step();
		void debug_display();
		void debug_process_command();

		bool running;
	
		struct debugging
		{
			bool debug_mode;
			std::vector <u32> breakpoints;
			std::string last_command;
		} db_unit;

		MMU core_mmu;
		ARM7 core_cpu;
};
		
#endif // EMU_CORE