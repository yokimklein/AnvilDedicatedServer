#pragma once
#include <cseries\cseries.h>

#define USER_INVALID 0
#define USER_SYSTEM -1

struct s_transport_secure_address;
qword user_xuid_from_secure_address(s_transport_secure_address const * secure_address);