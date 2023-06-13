#include "bipeds.h"
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_objects.h>

bool __fastcall biped_update_melee_turning(datum_index biped_index)
{
	s_object_header* object_header_data = (s_object_header*)(get_tls()->object_headers->data);
	s_biped_data* biped = (s_biped_data*)object_get(biped_index);

	if (!biped_calculate_melee_aiming(biped_index, &biped->melee_aiming_vector))
		return false;

	biped->facing_vector = biped->melee_aiming_vector;
	biped->aiming_vector = biped->melee_aiming_vector;
	biped->looking_vector = biped->melee_aiming_vector;
	biped->previous_looking_vector = biped->melee_aiming_vector;
	simulation_action_object_update(biped_index, _simulation_biped_update_desired_aiming_vector);
	biped->biped_flags.set(_biped_flag0, true);
	return true;
}