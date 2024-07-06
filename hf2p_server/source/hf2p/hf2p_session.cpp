#include "hf2p_session.h"
#include <cseries\cseries.h>

bool __cdecl hf2p_setup_session()
{
	return INVOKE(0x3AAF10, hf2p_setup_session);
}

void __cdecl hf2p_handle_disconnection()
{
	INVOKE(0x2F29C0, hf2p_handle_disconnection);
}