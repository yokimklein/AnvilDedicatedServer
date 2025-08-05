#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_address.h>

struct s_transport_unique_identifier
{
	byte data[0x10];
};
static_assert(sizeof(s_transport_unique_identifier) == 0x10);

struct s_transport_secure_address : s_transport_unique_identifier
{
	bool operator==(s_transport_secure_address other) { return csmemcmp(this, &other, sizeof(*this)) == 0; };
	bool operator!=(s_transport_secure_address other) { return csmemcmp(this, &other, sizeof(*this)) != 0; };
};
static_assert(sizeof(s_transport_secure_address) == 0x10);

struct s_transport_secure_identifier : s_transport_unique_identifier
{
};
static_assert(sizeof(s_transport_secure_identifier) == 0x10);

struct s_transport_secure_key : s_transport_unique_identifier
{
};
static_assert(sizeof(s_transport_secure_key) == 0x10);

struct s_transport_session_description
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_session_description) == 0x30);

#pragma pack(push, 4)
struct s_transport_security_key
{
	bool active;
	bool generated_locally;
	long key_platform;
	s_transport_secure_identifier key_id;
	s_transport_secure_key key;
};
static_assert(sizeof(s_transport_security_key) == 0x28);
#pragma pack(pop)

struct s_transport_security_globals
{
	bool initialized;
	bool local_address_valid;
	s_transport_secure_address local_secure_address;
	transport_address address;
	s_transport_secure_identifier identifier;
	s_transport_unique_identifier local_unique_identifier;
	byte pad[0x8];
	s_transport_security_key keys[10];
};
static_assert(sizeof(s_transport_security_globals) == 0x1E0);

bool transport_secure_address_get(s_transport_secure_address* secure_address);
char const* transport_secure_nonce_get_string(qword secure_nonce);
char const* transport_secure_address_get_string(s_transport_secure_address const* secure_address);
char const* transport_secure_identifier_get_string(s_transport_secure_identifier const* secure_identifier);
char const* transport_session_description_get_string(s_transport_session_description const* session_description);
char* transport_secure_address_to_string(s_transport_secure_address const* secure_address, char* string, long maximum_string_length, bool include_online, bool include_mac);
const char* transport_secure_address_get_mac_string(s_transport_secure_address const* secure_address);
char const* transport_unique_identifier_get_string(s_transport_unique_identifier const* unique_identifier);
const s_transport_unique_identifier* transport_unique_identifier_get();
void transport_secure_address_build_from_identifier(s_transport_unique_identifier const* unique_identifier, s_transport_secure_address* secure_address);
bool transport_secure_identifier_retrieve(transport_address const* usable_address, e_transport_platform transport_platform, s_transport_secure_identifier* secure_identifier, s_transport_secure_address* secure_address);
void transport_secure_address_extract_identifier(s_transport_secure_address const* secure_address, s_transport_unique_identifier* unique_identifier);
void transport_secure_address_generate(s_transport_secure_address* secure_address);
bool __cdecl transport_secure_address_resolve();
s_transport_secure_address* get_session_secure_address(s_transport_secure_address* out_address);
bool __fastcall transport_secure_key_create(s_transport_session_description* session_description);

extern s_transport_security_globals& transport_security_globals;
extern s_transport_secure_address const& g_session_secure_address;