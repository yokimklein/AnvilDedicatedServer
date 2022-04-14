#include "network_managed_session.h"
#include "../../dllmain.h"

// TEST THIS - function exists in ms29 but I couldn't get it to call correctly
bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform)
{
	c_managed_session* online_session_manager_globals__managed_sessions = (c_managed_session*)(module_base + 0x3EAB128);
	c_managed_session* managed_session = &online_session_manager_globals__managed_sessions[managed_session_index];

	if (managed_session_index == -1 || (managed_session->__flags8 & 0x10) == 0)
		return false;
	if (out_secure_host_description)
		*out_secure_host_description = managed_session->live_session.description;
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