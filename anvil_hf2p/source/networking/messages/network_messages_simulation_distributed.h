#pragma once
#include <networking\messages\network_message_type_collection.h>
#include <game\game_results.h>

struct s_network_message_distributed_game_results
{
	long establishment_identifier;
	long update_number;
	s_game_results_incremental_update update;
};
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
static_assert(sizeof(s_network_message_distributed_game_results) == 0x10CA8);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
static_assert(sizeof(s_network_message_distributed_game_results) == 0x10EA8);
#endif