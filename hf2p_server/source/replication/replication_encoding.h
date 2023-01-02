#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/replication/replication_encoding.hpp

#include "..\cseries\cseries.h"

struct s_player_prediction
{
	long predicted_entity_index;
	long predicted_gamestate_index;
	byte control_context_identifier;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d translational_velocity;
	real_vector3d angular_velocity;
};
static_assert(sizeof(s_player_prediction) == 0x48);