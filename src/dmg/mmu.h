// GB Enhanced+ Copyright Daniel Baxter 2015
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : mmu.h
// Date : May 09, 2015
// Description : Game Boy (Color) memory manager unit
//
// Handles reading and writing bytes to memory locations

#ifndef GB_MMU
#define GB_MMU

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "common.h"
#include "common/config.h"
#include "gamepad.h"
#include "lcd_data.h"
#include "apu_data.h"
#include "sio_data.h"

class DMG_MMU
{
	public:

	//Memory bank controller-types enumeration
	enum mbc_types
	{ 
		ROM_ONLY, 
		MBC1, 
		MBC2, 
		MBC3, 
		MBC5,
		MBC6,
		MBC7,
		HUC1,
		HUC3,
		MMM01,
		M161,
		GB_CAMERA,
		TAMA5,
	};

	std::vector <u8> memory_map;
	std::vector <u8> bios;

	//Memory Banks
	std::vector< std::vector<u8> > read_only_bank;
	std::vector< std::vector<u8> > random_access_bank;

	//Working RAM Banks - GBC only
	std::vector< std::vector<u8> > working_ram_bank;
	std::vector< std::vector<u8> > video_ram;

	//Flash memory - MBC6 only
	std::vector< std::vector<u8> > flash;

	u8 original_sys_type;

	//Bank controls
	u16 rom_bank;
	u8 ram_bank;
	u8 wram_bank;
	u8 vram_bank;
	u8 bank_bits;
	u8 bank_mode;
	bool ram_banking_enabled;

	//BIOS controls
	bool in_bios;
	u8 bios_type;
	u32 bios_size;

	//Cartridge data structure
	struct cart_data
	{
		//General MBC attributes
		u32 rom_size;
		u32 ram_size;
		mbc_types mbc_type;
		bool battery;
		bool ram;
		bool multicart;
		bool sonar;
		bool rumble;

		//MBC3 RTC
		bool rtc;
		bool rtc_enabled;
		bool rtc_latched;
		u8 rtc_latch_1, rtc_latch_2, rtc_reg[5];
		u8 latch_reg[5];
		u64 rtc_timestamp;

		//MBC6
		u8 flash_cnt;
		u8 flash_cmd;
		u8 flash_stat;
		u8 flash_io_bank;
		bool flash_get_id;

		//MBC7
		bool idle;
		u8 internal_value;
		u8 internal_state;
		u8 cs;
		u8 sk;
		u8 buffer_length;
		u8 command_code;
		u16 addr;
		u16 buffer;

		//Camera
		u8 cam_reg[54];
		std::vector <u8> cam_buffer;
		bool cam_lock;

		//MBC1S
		u8 sonar_byte;
		u8 depth;
		std::vector <u8> frame_data;
		u32 pulse_count;
		u32 frame_count;

		//TAMA5
		u8 tama_reg[16];
		u8 tama_ram[256];
		u8 tama_cmd;
		u8 tama_out;

		//GB Memory Cartridge
		u8 gb_mem_map[128];

		//HuC-1 + HuC-3
		u8 huc_ir_input;
		u8 huc_reg_map;
		u8 huc_semaphore;
		u8 huc_addr;
		u8 huc_ram[256];

		u32 huc_rtc_seconds;
		u32 huc_rtc_days;
		u8 huc_rtc_cmd;
		u8 huc_rtc_out;
		u8 huc_tone_generator_flag;
		
	} cart;

	struct ir_data
	{
		u8 signal;
		bool send;
		bool try_connection;
		u8 trigger;
		s32 fade_counter;
		u8 network_id;
	} ir_stat;

	struct gb_kiss_link_data
	{
		u32 cycles;
		std::vector<u32> input_signals;
		std::vector<u32> output_signals;
		std::vector<u8> gbf_data;
		std::vector<u8> input_data;
		std::vector<u8> output_data;
		std::vector<u8> file_search_data;
		gb_kiss_link_state state;
		gb_kiss_link_stage stage;
		bool is_locked;
		bool is_ping_delayed;
		bool is_upload_done;
		bool is_sender;
		bool is_running;

		u8 cmd;
		u8 checksum;
		u8 param;
		u8 slot;
		u8 cart_code;
		u8 mode;
		u16 len;
		u16 data_len;
		u16 remote_addr;
		u16 local_addr;

		u32 gbf_index;
		u16 gbf_file_size;
		u16 gbf_raw_size;
		u8 gbf_title_icon_size;
		u8 gbf_flags;
	} kiss_link;

	bool div_reset;

	dmg_core_pad* g_pad;

	std::vector<u32> sub_screen_buffer;
	u32 sub_screen_update;
	bool sub_screen_lock;

	//Advanced debugging
	#ifdef GBE_DEBUG
	bool debug_write;
	bool debug_read;
	u16 debug_addr;
	#endif

	DMG_MMU();
	~DMG_MMU();

	void reset();
	void grab_mbc3_time();
	void grab_huc3_time();

	u8 read_u8(u16 address);
	u16 read_u16(u16 address);
	s8 read_s8(u16 address);

	void write_u8(u16 address, u8 value);
	void write_u16(u16 address, u16 value);

	//GBC DMAs
	void hdma();
	void gdma();

	bool read_file(std::string filename);
	bool read_bios(std::string filename);
	bool save_backup(std::string filename);
	bool load_backup(std::string filename);

	bool patch_ips(std::string filename);
	bool patch_ups(std::string filename);

	//Memory Bank Controller dedicated read/write operations
	void mbc_write(u16 address, u8 value);
	u8 mbc_read(u16 address);

	void mbc1_write(u16 address, u8 value);
	u8 mbc1_read(u16 address);

	void mbc1_multicart_write(u16 address, u8 value);
	u8 mbc1_multicart_read(u16 address);

	void mbc1s_write(u16 address, u8 value);
	u8 mbc1s_read(u16 address);
	bool mbc1s_load_sonar_data(std::string filename);

	void mbc2_write(u16 address, u8 value);
	u8 mbc2_read(u16 address);

	void mbc3_write(u16 address, u8 value);
	u8 mbc3_read(u16 address);

	void mbc5_write(u16 address, u8 value);
	u8 mbc5_read(u16 address);

	void mbc6_write(u16 address, u8 value);
	u8 mbc6_read(u16 address);

	void mbc7_write(u16 address, u8 value);
	void mbc7_write_ram(u8 value);
	u8 mbc7_read(u16 address);

	void huc1_write(u16 address, u8 value);
	u8 huc1_read(u16 address);

	void huc3_write(u16 address, u8 value);
	u8 huc3_read(u16 address);
	void huc3_process_command();

	void mmm01_write(u16 address, u8 value);
	u8 mmm01_read(u16 address);

	void m161_write(u16 address, u8 value);
	u8 m161_read(u16 address);

	void cam_write(u16 address, u8 value);
	u8 cam_read(u16 address);
	bool cam_load_snapshot(std::string filename);

	void tama5_write(u16 address, u8 value);
	u8 tama5_read(u16 address);
	void grab_tama5_time(u8 index);

	void gb_mem_remap();
	bool gb_mem_read_map(std::string filename);
	void gb_mem_format_save(std::string filename);

	void gb_kiss_link_get_bytes();
	void gb_kiss_link_set_signal(u8 input);
	void gb_kiss_link_handshake(u8 input);
	void gb_kiss_link_send_command();
	void gb_kiss_link_recv_command();
	void gb_kiss_link_send_ping(u32 on_pulse, u32 off_pulse);
	void gb_kiss_link_process();
	void gb_kiss_link_process_command();
	void gb_kiss_link_process_ping();
	void gb_kiss_link_finish_command();
	bool gb_kiss_link_load_file(std::string filename);
	bool gb_kiss_link_save_file();
	void gb_kiss_link_reset(bool reset_gbf);

	void set_gs_cheats();
	void set_gg_cheats();

	void set_lcd_data(dmg_lcd_data* ex_lcd_stat);
	void set_apu_data(dmg_apu_data* ex_apu_stat);
	void set_sio_data(dmg_sio_data* ex_sio_stat);

	//Serialize data for save state loading/saving
	bool mmu_read(u32 offset, std::string filename);
	bool mmu_write(std::string filename);
	u32 size();

	private:

	u8 previous_value;

	//Only the MMU and LCD should communicate through this structure
	dmg_lcd_data* lcd_stat;

	//Only the MMU and APU should communicate through this structure
	dmg_apu_data* apu_stat;

	//Only the MMU and SIO should communicate through this structure
	dmg_sio_data* sio_stat;
};

#endif // GB_MMU
