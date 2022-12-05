#pragma once
#include "..\..\dllmain.h"
#include "..\session\network_session_parameter_session.h"

FUNCTION_DEF(0x2AC20, void, __fastcall, network_life_cycle_end);
FUNCTION_DEF(0x2AD00, bool, __fastcall, network_life_cycle_create_local_squad, e_network_session_class session_class);