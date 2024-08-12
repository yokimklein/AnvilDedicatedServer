#include "simulation_game_projectiles.h"
#include <game\game.h>
#include <items\projectiles.h>
#include <math\matrix_math.h>
#include <simulation\game_interface\simulation_game_events.h>

void simulation_action_projectile_attached(datum_index projectile_index, datum_index object_index, short node_index, real_point3d const* position, s_location const* location)
{
	if (game_is_distributed() && game_is_server() && !game_is_playback())
	{
		projectile_datum* projectile = (projectile_datum*)object_get_and_verify_type(projectile_index, _object_mask_projectile);
		if (projectile->object.gamestate_index != NONE)
		{
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
			assert(position);
			if (object_index == NONE)
			{
				event_data.node_position = *position;
			}
			else
			{
				long parent_object_node_matrices_count = 0;
				real_matrix4x3* node_matrices = object_get_node_matrices(object_index, &parent_object_node_matrices_count);
				assert(VALID_INDEX(event_data.parent_node_index, parent_object_node_matrices_count));
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
	}
}