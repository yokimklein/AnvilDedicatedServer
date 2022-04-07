#pragma once
#include <windows.h>
#include "transport_address.h"

struct s_transport_secure_address
{
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	byte data4[8];
};

struct s_transport_secure_identifier
{
	uint32_t  data1;
	uint16_t data2;
	uint16_t data3;
	byte data4[8];
};

struct s_transport_secure_key
{
	char data[16];
};

long transport_secure_address_get(GUID* transport_secure_address);