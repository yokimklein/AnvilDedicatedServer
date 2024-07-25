#include "simulation_game_damage.h"
#include <game\game.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <stdio.h>

void simulation_action_damage_section_response(datum_index object_index, long damage_section_index, long response_index, e_damage_section_response_type damage_section_response_type)
{
	if (game_is_distributed() && game_is_server() && !game_is_playback())
	{
		if (simulation_object_get_authoritative_entity(object_index) == -1)
		{
			const char* object_description;
			if (object_index == -1)
				object_description = "NONE";
			else
				object_description = object_describe(object_index);
			printf("MP/NET/SIMULATION,EVENT: simulation_action_damage_section_response: damage section on [%s] fired without an entity index\n", object_description);
		}
		else
		{
			s_simulation_damage_section_response_data response_data = {};
			response_data.damage_section_index = damage_section_index;
			response_data.response_index = response_index;
			response_data.damage_section_response_type = damage_section_response_type;
			long object_references = object_index;
			simulation_event_generate_for_clients(_simulation_event_type_damage_section_response, 1, &object_references, -1, sizeof(s_simulation_damage_section_response_data), &response_data);
		}
	}
}