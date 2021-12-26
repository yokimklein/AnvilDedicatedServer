#include "network_session_manager.h"
#include "../messages/network_message_type_collection.h"
#include "../../dllmain.h"

c_network_session* c_network_session_manager::get_session(s_transport_secure_identifier const* secure_identifier)
{
	typedef c_network_session*(__thiscall* get_session_ptr)(c_network_session_manager* thisptr, s_transport_secure_identifier const* secure_identifier);
	auto get_session = reinterpret_cast<get_session_ptr>(module_base + 0x27C80);
	return get_session(this, secure_identifier);
}