#pragma once
#include "..\transport\transport_address.h"

struct xsession_unknown_struct_sizeof_0x10
{
	byte flags;
	uint64_t xuid;
};

struct xsession
{
	unsigned int controller;
	unsigned __int16 slots_flags;
	__int16 : 16;
	int public_slot_count;
	int private_slot_count;
	s_transport_session_description description;
	char __data40[16];
	xsession_unknown_struct_sizeof_0x10 unknown_struct[16];
};

struct c_managed_session
{
	uint32_t session_class;
	e_transport_platform platform;
	uint32_t __flags8;
	uint16_t __flagsC;
	uint16_t __flagsE;
	char __data10[256];
	uint16_t __unknown110;
	char __data114[4];
	xsession offline_session;
	xsession local_session;
	xsession live_session;
	uint64_t xuids[16];
	byte xuid_flags[16];
	byte xuid_next_flags[16];
	char __data5A8[4];
	char creation_contexts[32];
	uint32_t creation_context_count;
};
static_assert(sizeof(c_managed_session) == 0x5D0);

bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform);
const char* managed_session_get_id_string(long managed_session_index);