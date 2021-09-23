#pragma once
#include "..\delivery\network_channel.h"
#include "..\transport\transport_address.h"

class c_network_link
{
public:
	c_network_channel* get_associated_channel(s_transport_address address);

private:

};

