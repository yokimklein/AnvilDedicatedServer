#include "simulation_game_projectiles.h"
#include <game\game.h>
#include <items\projectiles.h>
#include <math\matrix_math.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <simulation\simulation.h>

void simulation_action_projectile_attached(datum_index projectile_index, datum_index object_index, short node_index, real_point3d const* position, s_location const* location)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	projectile_datum* projectile = (projectile_datum*)object_get_and_verify_type(projectile_index, _object_mask_projectile);
	if (projectile->object.gamestate_index == NONE)
	{
		return;
	}

	s_simulation_projectile_attached_event_data event_data;
	csmemset(&event_data, 0, sizeof(s_simulation_projectile_attached_event_data));
	if (object_index == NONE ||
		projectile->object.gamestate_index != NONE ||
		projectile->object.parent_object_index != NONE ||
		object_get_type(projectile->object.parent_object_index) != _object_type_biped)
	{
		event_data.parent_node_index = node_index;
	}
	else
	{
		event_data.parent_node_index = projectile->object.parent_node_index;
	}
	datum_index object_references[] = { projectile_index, object_index };
	event_data.attached_to_object = object_index != NONE;
	ASSERT(position);
	if (object_index == NONE)
	{
		event_data.node_position = *position;
	}
	else
	{
		long parent_object_node_matrices_count = 0;
		real_matrix4x3* node_matrices = object_get_node_matrices(object_index, &parent_object_node_matrices_count);
		ASSERT(VALID_INDEX(event_data.parent_node_index, parent_object_node_matrices_count));
		real_matrix4x3 inverse_matrix;
		matrix4x3_inverse(&node_matrices[event_data.parent_node_index], &inverse_matrix);
		matrix4x3_transform_point(&inverse_matrix, position, &event_data.node_position);
		event_data.position_encoding_type = 0;
	}
	if (location)
	{
		event_data.location.cluster_reference = location->cluster_reference;
	}
	else
	{
		event_data.location.cluster_reference.bsp_index = NONE;
		event_data.location.cluster_reference.cluster_index = NONE;
	}
	simulation_event_generate_for_clients(_simulation_event_type_projectile_attached, NUMBEROF(object_references), object_references, NONE, sizeof(event_data), &event_data);
}

void simulation_action_projectile_detonate(long projectile_definition_index, real_point3d const* position, real_vector3d const* forward, real_vector3d const* hit_normal, float damage_scale, c_flags<e_simulation_projectile_effect_flag, uchar, k_simulation_projectile_effect_flag_count> projectile_effect_flag, c_global_material_type hit_material_type, s_location const* location, long player_index0, long player_index1, bool gamestate_index_invalid, bool valid_material_type, bool collided_with_invalid_material)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback() || (!gamestate_index_invalid && !valid_material_type))
	{
		return;
	}

	s_simulation_projectile_detonate_event_data event_data;
	csmemset(&event_data, 0, sizeof(s_simulation_projectile_detonate_event_data));

	ASSERT(hit_normal);

	event_data.projectile_definition_index = projectile_definition_index;
	event_data.position = *position;
	event_data.forward = *forward;
	event_data.damage_scale = damage_scale;
	event_data.projectile_effect_flag = projectile_effect_flag;
	event_data.collided_with_invalid_material = collided_with_invalid_material;
	event_data.material_index = hit_material_type.m_index;
	event_data.location_valid = location->cluster_reference.bsp_index != NONE;
	if (event_data.location_valid)
	{
		event_data.location = *location;
	}
	else
	{
		event_data.location.cluster_reference.bsp_index = NONE;
		event_data.location.cluster_reference.cluster_index = NONE;
	}
	event_data.hit_normal = *hit_normal;
	event_data.player_index0 = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index0);
	event_data.player_index1 = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index1);
	simulation_event_generate_for_clients(_simulation_event_type_projectile_detonate, 0, nullptr, NONE, sizeof(event_data), &event_data);
}

// syncs explosion sounds for non-predicted projectiles
void simulation_action_projectile_impact_raw(bool detonation_timer_started, datum_index projectile_index, float material_effect_scale, float material_effect_error, real_vector3d const* impact_direction_normal, collision_result const* collision, bool from_impact)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	projectile_datum* projectile = (projectile_datum*)object_get_and_verify_type(projectile_index, _object_mask_projectile);
	if (projectile->object.gamestate_index == NONE)
	{
		return;
	}

	if (collision->type.get() != _collision_result_object || collision->object_index == NONE || object_get(collision->object_index)->object.gamestate_index == NONE)
	{
		s_simulation_projectile_impact_effect_event_data event_data;
		csmemset(&event_data, 0, sizeof(s_simulation_projectile_impact_effect_event_data));
		event_data.projectile_definition_index = projectile->definition_index;
		event_data.material_effect_scale = material_effect_scale;
		event_data.material_effect_error = material_effect_error;
		event_data.impact_direction_normal = *impact_direction_normal;
		event_data.collision_plane_normal = collision->plane.n;
		event_data.position = collision->position;
		event_data.material_index = collision->material_type.m_index;
		event_data.from_impact = from_impact;
		simulation_event_generate_for_clients(_simulation_event_type_projectile_impact_effect, 0, nullptr, NONE, sizeof(event_data), &event_data);
	}
	else
	{
		s_simulation_projectile_object_impact_effect_event_data event_data;
		csmemset(&event_data, 0, sizeof(s_simulation_projectile_object_impact_effect_event_data));
		event_data.projectile_definition_index = projectile->definition_index;
		event_data.material_effect_scale = material_effect_scale;
		event_data.material_effect_error = material_effect_error;
		event_data.impact_direction_normal = *impact_direction_normal;
		event_data.collision_plane_normal = collision->plane.n;
		event_data.material_index = collision->material_type.m_index;
		event_data.from_impact = from_impact;
		event_data.collision_node_index = collision->node_index;
		datum_index object_references[] = { collision->object_index };
		event_data.detonation_timer_started = detonation_timer_started;
		event_data.position_encoding_type = simulation_get_node_space_encoding_type(collision->object_index);
		long object_node_matrices_count = 0;
		real_matrix4x3* node_matrices = object_get_node_matrices(collision->object_index, &object_node_matrices_count);
		VALID_INDEX(collision->node_index, object_node_matrices_count);
		real_matrix4x3 inverse_matrix;
		matrix4x3_inverse(&node_matrices[collision->node_index], &inverse_matrix);
		matrix4x3_transform_point(&inverse_matrix, &collision->position, &event_data.position);
		simulation_event_generate_for_clients(_simulation_event_type_projectile_object_impact_effect, NUMBEROF(object_references), object_references, NONE, sizeof(event_data), &event_data);
	}
}