#pragma once
#include "..\session\network_session_parameters_session.h"
#include "..\session\network_observer.h"

static bool* life_cycle_initialized = (bool*)(module_base + 0x3EADFA8);

FUNCTION_DEF(0x2AC20, void, __fastcall, network_life_cycle_end);
FUNCTION_DEF(0x2AD00, bool, __fastcall, network_life_cycle_create_local_squad, e_network_session_class session_class);

bool network_life_cycle_get_observer(c_network_observer** out_observer);