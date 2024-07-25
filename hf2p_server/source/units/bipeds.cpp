#include "bipeds.h"
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_units.h>

bool __fastcall biped_calculate_melee_aiming(datum_index biped_index, real_vector3d* melee_aiming_vector)
{
	return INVOKE(0x4409F0, biped_calculate_melee_aiming, biped_index, melee_aiming_vector);
}

bool __fastcall biped_update_melee_turning(datum_index biped_index)
{
	s_biped_data* biped = (s_biped_data*)object_get(biped_index);

	if (!biped_calculate_melee_aiming(biped_index, &biped->melee_aiming_vector))
		return false;

	biped->facing_vector = biped->melee_aiming_vector;
	biped->aiming_vector = biped->melee_aiming_vector;
	biped->looking_vector = biped->melee_aiming_vector;
	biped->previous_looking_vector = biped->melee_aiming_vector;
	simulation_action_object_update(biped_index, _simulation_unit_update_desired_aiming_vector);
	biped->biped_flags.set(_biped_flag0, true);
	return true;
}