#include "simulation_game_units.h"
#include <game\game.h>
#include <stdio.h>
#include <simulation\game_interface\simulation_game_events.h>

void simulation_action_unit_board_vehicle(datum_index unit_index)
{
	if (game_is_distributed() && game_is_server() && !game_is_playback())
	{
		unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
		long seat_index = unit->unit.seat_index;
		datum_index parent_object_index = unit->object.parent_object_index;
		if (parent_object_index == NONE || seat_index == NONE)
		{
			printf("MP/NET/SIMULATION,EVENT: simulation_action_unit_board_vehicle: attempting to generate a board-vehicle event for unit not in a vehicle or bad seat index\n");
		}
		else
		{
			s_simulation_unit_board_vehicle_event_data event_data;
			event_data.boarding_seat_index = seat_index;
			datum_index object_references[] = { unit_index, parent_object_index };
			simulation_event_generate_for_clients(_simulation_event_type_unit_board_vehicle, NUMBEROF(object_references), object_references, NONE, sizeof(event_data), &event_data);
		}
	}
}