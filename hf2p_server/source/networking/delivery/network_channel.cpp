#include "network_channel.h"
#include "..\messages\network_message_type_collection.h"
#include "..\transport\transport_address.h"

// TODO: THESE FOUR FUNCS
bool c_network_channel::connected()
{
	return true;
}

char* c_network_channel::get_name()
{
	return 0;
}

char* c_network_channel::get_message_type_name(e_network_message_type message_type)
{
	return 0;
}

bool c_network_channel::get_remote_address(s_transport_address* remote_address)
{
	return true;
}