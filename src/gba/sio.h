// GB Enhanced Copyright Daniel Baxter 2018
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : sio.cpp
// Date : August 14, 2018
// Description : Game Boy Advance Serial Input-Output emulation
//
// Sets up SDL networking
// Emulates GBA-to-GBA data transfers

#ifndef GBA_SIO
#define GBA_SIO

#ifdef GBE_NETPLAY
#include <SDL2/SDL_net.h>
#endif

#include "mmu.h"
#include "sio_data.h" 

class AGB_SIO
{
	public:

	//Link to memory map
	AGB_MMU* mem;

	agb_sio_data sio_stat;

	bool network_init;
	bool is_master;
	u8 master_id;
	u8 max_clients;

	#ifdef GBE_NETPLAY

	//Receiving server
	struct tcp_server
	{
		TCPsocket host_socket, remote_socket;
		IPaddress host_ip;
		bool connected;
		bool host_init;
		bool remote_init;
		u16 port;
	} server;

	//Sending client
	struct tcp_sender
	{
		TCPsocket host_socket;
		IPaddress host_ip;
		bool connected;
		bool host_init;
		u16 port;
	} sender;

	SDLNet_SocketSet tcp_sockets;
	SDLNet_SocketSet four_player_tcp_sockets;

	#endif

	AGB_SIO();
	~AGB_SIO();

	bool init();
	void reset();

	bool send_data();
	bool receive_byte();
	bool request_sync();
	void process_network_communication();
};

#endif // GBA_SIO
