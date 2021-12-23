#include "network_session_manager.h"
#include "../messages/network_message_type_collection.h"
#include "../../dllmain.h"

class c_network_channel;
c_network_session* c_network_session_manager::get_session(s_network_message* message)
{
	// 0xD7C80
	typedef c_network_session*(__thiscall* get_session_ptr)(c_network_session_manager* thisptr, s_network_message* message);
	auto get_session = reinterpret_cast<get_session_ptr>(module_base + 0xD7C80);

	return get_session(this, message);
}