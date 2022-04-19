#include "network_managed_session.h"

// TEST THIS - function exists in ms29 but I couldn't get it to call correctly
bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform)
{
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];

	if (managed_session_index == -1 || (managed_session->flags & 0x10) == 0)
		return false;
	if (out_secure_host_description)
		*out_secure_host_description = managed_session->actual_online_session_state.description;
	if (out_transport_platform)
		*out_transport_platform = managed_session->platform;
	return true;
}

// FUNC TODO
const char* managed_session_get_id_string(long managed_session_index)
{
	const char* managed_session_id = "00:00:00:00:00:00:00:00";
	return managed_session_id;
}

// this exists in ms29, I rewrote it thinking there was an issue with it but there wasn't, oh well lol
bool managed_session_get_id(long index, s_transport_secure_identifier* secure_id)
{
	if (secure_id != nullptr)
		memset(secure_id, 0, sizeof(s_transport_secure_identifier));
	if (index == -1)
		return false;
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[index];
	if ((managed_session->flags & 0x10) == 0) // test _online_managed_session_created_bit
		return false;
	if (secure_id != nullptr)
		memcpy(secure_id->data, managed_session->actual_online_session_state.description.session_id.data, sizeof(s_transport_secure_identifier));
	return true;
}