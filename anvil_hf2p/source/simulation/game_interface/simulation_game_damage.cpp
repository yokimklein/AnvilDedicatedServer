#include "simulation_game_damage.h"
#include <game\game.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <stdio.h>
#include <cache\cache_files.h>
#include <tag_files\tag_groups.h>
#include <objects\object_definitions.h>
#include <math.h>

void simulation_action_damage_section_response(datum_index object_index, long damage_section_index, long response_index, e_damage_section_response_type damage_section_response_type)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	if (simulation_object_get_authoritative_entity(object_index) == NONE)
	{
		const char* object_description;
		if (object_index == NONE)
		{
			object_description = "NONE";
		}
		else
		{
			object_description = object_describe(object_index);
		}
		printf("MP/NET/SIMULATION,EVENT: simulation_action_damage_section_response: damage section on [%s] fired without an entity index\n", object_description);
	}
	else
	{
		s_simulation_damage_section_response_data response_data;
		response_data.damage_section_index = damage_section_index;
		response_data.response_index = response_index;
		response_data.damage_section_response_type = damage_section_response_type;
		datum_index object_references[] = { object_index };
		simulation_event_generate_for_clients(_simulation_event_type_damage_section_response, NUMBEROF(object_references), object_references, NONE, sizeof(s_simulation_damage_section_response_data), &response_data);
	}
}

void simulation_action_damage_aftermath(datum_index object_index, s_damage_aftermath_result_data const* result_data)
{
	if (!should_send_damage_aftermath_event(object_index, result_data))
	{
		return;
	}

	datum_index object_references[2];
	s_simulation_damage_aftermath_event_data aftermath_event_data;
	build_damage_aftermath_event_data(object_index, result_data, &aftermath_event_data, object_references);
	simulation_event_generate_for_clients(_simulation_event_type_damage_aftermath, NUMBEROF(object_references), object_references, NONE, sizeof(s_simulation_damage_aftermath_event_data), &aftermath_event_data);
}

bool should_send_damage_aftermath_event(datum_index object_index, s_damage_aftermath_result_data const* result_data)
{
	if (game_is_distributed()
		&& game_is_server()
		&& !game_is_playback()
		&& simulation_object_get_authoritative_entity(object_index) != NONE
		&& damage_aftermath_is_important(object_index, result_data))
	{
		return true;
	}
	return false;
}

bool damage_aftermath_is_important(datum_index object_index, s_damage_aftermath_result_data const* result_data)
{
	object_datum* object = object_get(object_index);
	struct object_definition* object_definition = (struct object_definition*)tag_get(OBJECT_TAG, object->definition_index);
	struct damage_effect_definition* damage_effect_definition = (struct damage_effect_definition*)tag_get(DAMAGE_EFFECT_TAG, result_data->damage_definition_index);
	datum_index player_index = NONE;
	if (TEST_BIT(_object_mask_unit, object->object.object_identifier.m_type.get()))
	{
		unit_datum* unit = (unit_datum*)object;
		player_index = unit->unit.player_index;
	}
	if (!TEST_BIT(_object_mask_unit | _object_mask_crate, object->object.object_identifier.m_type.get()))
	{
		return false;
	}
	if (damage_effect_definition->damage.intantaneous_acceleration * object_definition->object.acceleration_scale > k_real_epsilon
		&& (!result_data->flags.test(_damage_aftermath_outside_aoe_dmg_range_bit))
		|| TEST_BIT(_object_mask_unit, object->object.object_identifier.m_type.get())
		&& (!object->object.damage_flags.test(_object_damage_flag_bit2) || result_data->flags.test(_damage_aftermath_body_depleted_bit))
		&& (result_data->body_damage > 0.0f || result_data->shield_damage > 0.0f))
	{
		return true;
	}
	if (player_index == NONE)
	{
		return false;
	}
	return result_data->damage_owner_player_index != NONE
		&& result_data->damage_owner_player_index != player_index
		&& result_data->scale > 0.0f;
}

void build_damage_aftermath_event_data(datum_index object_index, s_damage_aftermath_result_data const* result_data, s_simulation_damage_aftermath_event_data* out_event_data, datum_index out_object_references[2])
{
	if (simulation_object_get_authoritative_entity(object_index) == NONE)
	{
		return;
	}
	out_object_references[0] = object_index;
	if (result_data->object_index == NONE || !object_try_and_get_and_verify_type(result_data->object_index, NONE))
	{
		out_object_references[1] = NONE;
	}
	else
	{
		out_object_references[1] = result_data->object_index;
	}
	csmemset(out_event_data, 0, sizeof(s_simulation_damage_aftermath_event_data));
	out_event_data->damage_definition_index = result_data->damage_definition_index;
	out_event_data->damage_owner_player_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(result_data->damage_owner_player_index);
	out_event_data->direction = result_data->direction;
	if (normalize3d(&out_event_data->direction) > k_real_epsilon)
	{
		// Match parity with H3/ODST and set direction to be valid when above the epsilon
		// HO uses reach's build_damage_aftermath_event_data which does not set this bool, causing damage physics to not sync
		out_event_data->direction_valid = true;
	}
	else
	{
		out_event_data->direction = *global_up3d;
		printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: event damage_aftermath has bad direction vector - who generated this nonsense?\n");
	}
	if (result_data->epicenter_valid)
	{
		out_event_data->epicenter_valid = true;
		real_point3d object_origin;
		object_get_origin_interpolated(object_index, &object_origin);
		vector_from_points3d(&object_origin, (real_point3d*)&result_data->epicenter_direction_vector, &out_event_data->epicenter_direction_vector);
		out_event_data->epicenter_direction = normalize3d(&out_event_data->epicenter_direction_vector);
		if (fabsf(out_event_data->epicenter_direction) < k_real_epsilon)
		{
			printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: Epicenter represents invalid vector despite a physics impulse occurring on the authority.  What's up?\n");
			out_event_data->epicenter_valid = false;
		}
		else
		{
			if (!IN_RANGE_INCLUSIVE(out_event_data->epicenter_direction, 0.0f, 25.0f))
			{
				printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: epicenter magnitude (%f) out of expected range (%f to %f)\n", out_event_data->epicenter_direction, 0.0f, 25.0f);
			}
			out_event_data->epicenter_direction = CLAMP(out_event_data->epicenter_direction, 0.0f, 25.0f);
		}
	}
	if (!IN_RANGE_INCLUSIVE(result_data->scale, 0.0f, 2.0f))
	{
		printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: scale (%f) out of expected range (%f to %f)\n", result_data->scale, 0.0f, 2.0f);
	}
	out_event_data->scale = CLAMP(result_data->scale, 0.0f, 2.0f);

	if (!IN_RANGE_INCLUSIVE(result_data->shake_scale, 0.0f, 2.0f))
	{
		printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: shake scale (%f) out of expected range (%f to %f)\n", result_data->shake_scale, 0.0f, 2.0f);
	}
	out_event_data->shake_scale = CLAMP(result_data->shake_scale, 0.0f, 2.0f);
	out_event_data->flags = result_data->flags;
	out_event_data->ping_type = result_data->ping_type;

	if (!IN_RANGE_INCLUSIVE(result_data->body_damage, 0.0f, 16.0f))
	{
		printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: body damage (%f) out of expected range (%f to %f)\n", result_data->body_damage, 0.0f, 16.0f);
	}
	out_event_data->body_damage = CLAMP(result_data->body_damage, 0.0f, 16.0f);

	if (!IN_RANGE_INCLUSIVE(result_data->shield_damage, 0.0f, 3.0f))
	{
		printf("MP/NET/SIMULATION: build_damage_aftermath_event_data: shield damage (%f) out of expected range (%f to %f)\n", result_data->shield_damage, 0.0f, 3.0f);
	}
	out_event_data->shield_damage = CLAMP(result_data->shield_damage, 0.0f, 3.0f);
	out_event_data->body_part = static_cast<short>(result_data->body_part);
	out_event_data->node_index = static_cast<short>(result_data->node_index);
	out_event_data->damage_reporting_info = result_data->damage_reporting_info;
	out_event_data->special_death_type = result_data->special_death_type;
}

void simulation_action_damage_aftermath_exclusive_list(datum_index object_index, s_damage_aftermath_result_data const* result_data, datum_index const* player_indices, long player_count)
{
	if (!should_send_damage_aftermath_event(object_index, result_data))
	{
		return;
	}

	datum_index object_references[2];
	s_simulation_damage_aftermath_event_data aftermath_event_data;
	build_damage_aftermath_event_data(object_index, result_data, &aftermath_event_data, object_references);
	simulation_event_generate_for_client_player_list(_simulation_event_type_damage_aftermath, NUMBEROF(object_references), object_references, player_indices, player_count, sizeof(s_simulation_damage_aftermath_event_data), &aftermath_event_data);
}