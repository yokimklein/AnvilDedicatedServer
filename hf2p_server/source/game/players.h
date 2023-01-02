#pragma once
#include "..\cseries\cseries.h"

struct s_machine_identifier
{
	long data1;
	long data2;
	long data3;
	long data4;
};

struct s_player_identifier
{
	qword data;
	//dword ip_addr;
	//word port;
	//word_flags flags;
};
static_assert(sizeof(s_player_identifier) == 0x8);

bool player_identifier_is_valid(s_player_identifier const* identifier);
const char* player_identifier_get_string(s_player_identifier const* identifier);