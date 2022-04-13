#include "network_managed_session.h"
#include "../../dllmain.h"

bool managed_session_get_security_information(long managed_session_index, bool unknown, s_transport_session_description* session_description, e_transport_platform* transport_platform)
{
	typedef bool(__fastcall* managed_session_get_security_information_ptr)(long managed_session_index, bool unknown, s_transport_session_description* session_description, e_transport_platform* transport_platform);
	auto managed_session_get_security_information_call = reinterpret_cast<managed_session_get_security_information_ptr>(module_base + 0x28970);

	return managed_session_get_security_information_call(managed_session_index, unknown, session_description, transport_platform);
}

// FUNC TODO
const char* managed_session_get_id_string(long managed_session_index)
{
	const char* managed_session_id = "00:00:00:00:00:00:00:00";
	return managed_session_id;
}