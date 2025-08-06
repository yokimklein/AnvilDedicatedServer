#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_security.h>

#define USER_INVALID 0
#define USER_SYSTEM -1

enum e_request_status
{
	_request_status_none,
	_request_status_waiting,
	_request_status_response_received,

	k_request_status_count
};

struct s_userid_sessionid_pair
{
	qword user_id;
	const wchar_t* session_id;
};

struct s_user_sessions
{
	qword user_id;
	s_transport_secure_address session_id;
};

extern ulong g_user_sessions_count;
extern s_user_sessions* g_user_sessions;
extern e_request_status g_userid_request_status;

qword user_xuid_from_secure_address(const s_transport_secure_address* secure_address);
bool user_sessions_for_lobby_request();
void user_sessions_for_lobby_response();