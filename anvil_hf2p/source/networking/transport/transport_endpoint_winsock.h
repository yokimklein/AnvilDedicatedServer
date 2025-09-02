#pragma once

enum e_transport_type
{
	_transport_type_udp = 0x0,
	_transport_type_vdp = 0x1,
	_transport_type_tcp = 0x2,
	k_transport_type_count = 0x3,
};

struct transport_endpoint
{
	void* socket;
	e_transport_type type;
	long flags;
};

//long netadapter_test();