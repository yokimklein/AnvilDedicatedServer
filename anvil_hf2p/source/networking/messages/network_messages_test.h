#pragma once

struct s_network_message_test
{
	bool in_sequence;
	long id;
};
static_assert(sizeof(s_network_message_test) == 0x8);

class c_bitstream;
class c_network_message_test
{
public:

};