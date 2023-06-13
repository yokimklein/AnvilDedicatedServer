#pragma once
#include <networking\messages\network_message_type_collection.h>
#include <game\game_results.h>

struct s_network_message_distributed_game_results : s_network_message
{
	long establishment_identifier;
	long update_number;
	s_game_results_incremental_update game_results;
};
static_assert(sizeof(s_network_message_distributed_game_results) == 0x10CA8);