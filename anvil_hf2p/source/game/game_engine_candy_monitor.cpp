#include "game_engine_candy_monitor.h"

void __fastcall game_engine_register_object(datum_index object_index)
{
	INVOKE(0x172600, game_engine_register_object, object_index);
}