#pragma once
#include <windows.h>
#include "transport_address.h"

struct s_transport_secure_address
{
	unsigned long  data1;
	unsigned short data2;
	unsigned short data3;
	unsigned char data4[8];
};

struct s_transport_secure_identifier
{
	unsigned long  data1;
	unsigned short data2;
	unsigned short data3;
	unsigned char data4[8];
};

struct s_transport_secure_key
{
	char data[16];
};

int transport_secure_address_get(GUID* transport_secure_address);