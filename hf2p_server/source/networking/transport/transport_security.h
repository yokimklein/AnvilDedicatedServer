#pragma once
#include "..\..\cseries\cseries.h"
#include "transport_address.h"
#include "..\..\dllmain.h"

struct s_transport_unique_identifier
{
	dword part0;
	word part4[2];
	byte part8[8];
};
static_assert(sizeof(s_transport_unique_identifier) == 0x10);

struct s_transport_secure_address
{
	dword part0;
	word part4[2];
	byte part8[8];
};
static_assert(sizeof(s_transport_secure_address) == 0x10);

struct s_transport_secure_identifier
{
	dword part0;
	word part4[2];
	byte part8[8];
};
static_assert(sizeof(s_transport_secure_identifier) == 0x10);

struct s_transport_secure_key
{
	byte data[16];
};
static_assert(sizeof(s_transport_secure_key) == 0x10);

struct s_transport_session_description
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_session_description) == 0x30);

#pragma pack(push, 1)
struct s_transport_security_globals
{
	bool initialized;
	bool address_resolved;
	s_transport_secure_address secure_address;
	byte unknown1;
	byte unknown2;
	s_transport_address address;
	s_transport_secure_identifier identifier;
	s_transport_unique_identifier local_unique_identifier;
	byte unknown_data[400];
};
static_assert(sizeof(s_transport_security_globals) == 0x1D8);
#pragma pack(pop)

bool transport_secure_address_get(s_transport_secure_address* secure_address);
char const* transport_secure_nonce_get_string(qword secure_nonce);
char const* transport_secure_address_get_string(s_transport_secure_address const* secure_address);
char const* transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier);
char const* transport_session_description_get_string(s_transport_session_description const* session_description);
char* transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* string, long maximum_string_length, bool include_online, bool include_mac);
const s_transport_unique_identifier* transport_unique_identifier_get();
void transport_secure_address_build_from_identifier(s_transport_unique_identifier const* unique_identifier, s_transport_secure_address* secure_address);
bool transport_secure_identifier_retrieve(s_transport_address const* usable_address, e_transport_platform transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address);

static s_transport_security_globals* transport_security_globals = (s_transport_security_globals*)(module_base + 0x4EBE9D0);