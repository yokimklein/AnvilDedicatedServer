#include "hf2p_session.h"
#include <cseries\cseries.h>

bool __cdecl hf2p_setup_session()
{
	return INVOKE(ADDRESS_HF2P_SETUP_SESSION, hf2p_setup_session);
}

void __cdecl hf2p_handle_disconnection()
{
	INVOKE(ADDRESS_HF2P_HANDLE_DISCONNECTION, hf2p_handle_disconnection);
}