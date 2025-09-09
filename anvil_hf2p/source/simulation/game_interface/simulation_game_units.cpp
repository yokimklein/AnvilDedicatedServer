#include "simulation_game_units.h"
#include <game\game.h>
#include <stdio.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <cseries\cseries_events.h>

void simulation_action_unit_board_vehicle(datum_index unit_index)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
	long seat_index = unit->unit.seat_index;
	datum_index parent_object_index = unit->object.parent_object_index;
	if (parent_object_index == NONE || seat_index == NONE)
	{
		event(_event_warning, "networking:simulation:event: attempting to generate a board-vehicle event for unit not in a vehicle or bad seat index");
		return;
	}

	s_simulation_unit_board_vehicle_event_data event_data;
	csmemset(&event_data, 0, sizeof(s_simulation_unit_board_vehicle_event_data));
	event_data.boarding_seat_index = seat_index;
	datum_index object_references[] = { unit_index, parent_object_index };
	simulation_event_generate_for_clients(_simulation_event_type_unit_board_vehicle, NUMBEROF(object_references), object_references, NONE, sizeof(event_data), &event_data);
}

void simulation_action_unit_exit_vehicle(datum_index unit_index)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
	long seat_index = unit->unit.seat_index;
	datum_index parent_object_index = unit->object.parent_object_index;
	if (parent_object_index == NONE || seat_index == NONE)
	{
		event(_event_warning, "networking:simulation:event: attempting to generate an exit-vehicle event for unit not in a vehicle or bad seat index");
		return;
	}

	s_simulation_unit_exit_vehicle_event_data event_data;
	csmemset(&event_data, 0, sizeof(s_simulation_unit_exit_vehicle_event_data));
	event_data.parent_seat_index = seat_index;
	event_data.detach_on_exit = false;
	datum_index object_references[] = { unit_index, parent_object_index };
	simulation_event_generate_for_clients(_simulation_event_type_unit_exit_vehicle, NUMBEROF(object_references), object_references, NONE, sizeof(event_data), &event_data);
}

void simulation_action_unit_melee_clang(long clang_effect_index, real_point3d const* position, real_vector3d const* forward)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	s_simulation_unit_melee_clang_event_data event_data;
	csmemset(&event_data, 0, sizeof(s_simulation_unit_melee_clang_event_data));
	event_data.clang_effect_index = clang_effect_index;
	event_data.position = *position;
	event_data.forward = *forward;
	simulation_event_generate_for_clients(_simulation_event_type_unit_melee_clang, 0, nullptr, NONE, sizeof(event_data), &event_data);
}