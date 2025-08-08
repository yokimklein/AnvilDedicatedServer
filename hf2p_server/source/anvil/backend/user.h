#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_security.h>
#include <anvil\backend\private_service.h>
#include <simulation\simulation.h>
#include <atomic>

#define USER_INVALID 0
#define USER_SYSTEM -1

struct s_userid_sessionid_pair
{
	qword user_id;
	const wchar_t* session_id;
};

struct s_user_session
{
	bool valid;
	qword user_id;
	s_transport_secure_address session_id;
};

struct s_lobby_session_data
{
	s_lobby_session_data();
	void reset_user_data();

	std::atomic<e_request_status> status;
	s_user_session users[k_network_maximum_players_per_session];
};

extern s_lobby_session_data g_lobby_session_data;

qword user_xuid_from_secure_address(const s_transport_secure_address* secure_address);
bool user_sessions_request_for_lobby();