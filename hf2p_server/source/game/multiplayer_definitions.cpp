#include "multiplayer_definitions.h"
#include <tag_files\string_ids.h>
#include <scenario\scenario.h>

s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(multiplayer_globals_definition* multiplayer_globals)
{
	if (multiplayer_globals != nullptr && multiplayer_globals->universal.count() > 0)
	{
		return &multiplayer_globals->universal[0];
	}
	return nullptr;
}

string_id multiplayer_universal_data_get_weapon_selection_name_from_absolute_index(short absolute_index)
{
	switch (absolute_index)
	{
		case -1:
		case 0:
			return _string_id_invalid;
	}

	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr)
	{
		if (absolute_index >= 0 && absolute_index < universal->weapon_selections.count())
		{
			s_multiplayer_weapon_selection* weapon_selection = &universal->weapon_selections[absolute_index];
			return weapon_selection->name.get_value();
		}
	}
	return _string_id_invalid;
}

const s_multiplayer_weapon_set* multiplayer_universal_data_weapon_set_try_and_get(short absolute_index)
{
	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr && absolute_index < 0x8000 && absolute_index < universal->weapon_sets.count())
	{
		return &universal->weapon_sets[absolute_index];
	}
	return nullptr;
}

const s_multiplayer_vehicle_set* multiplayer_universal_data_vehicle_set_try_and_get(short absolute_index)
{
	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr && absolute_index < 0x8000 && absolute_index < universal->vehicle_sets.count())
	{
		return &universal->vehicle_sets[absolute_index];
	}
	return nullptr;
}

string_id multiplayer_universal_data_get_weapon_set_name_from_absolute_index(short absolute_index)
{
	if (absolute_index == -2)
		return STRING_ID(game_engine, random);

	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr && absolute_index < 0x8000 && absolute_index < universal->weapon_sets.count())
	{
		s_multiplayer_weapon_set* weapon_set = &universal->weapon_sets[absolute_index];
		return weapon_set->name.get_value();
	}

	return STRING_ID(global, default);
}

short multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(string_id name)
{
	switch (name)
	{
		case STRING_ID(game_engine, random):
			return -2;
		case _string_id_invalid:
		case _string_id_empty_string:
			return -1;
	}

	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr)
	{
		for (short i = 0; i < universal->weapon_sets.count(); i++)
		{
			if (universal->weapon_sets[i].name.get_value() == name)
			{
				return i;
			}
		}
	}
	return -1;
}

string_id multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(short absolute_index)
{
	if (absolute_index != NONE)
	{
		s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
		if (universal != nullptr && absolute_index < 0x8000 && absolute_index < universal->vehicle_sets.count())
		{
			s_multiplayer_vehicle_set* vehicle_set = &universal->vehicle_sets[absolute_index];
			return vehicle_set->name.get_value();
		}
	}
	return STRING_ID(global, default);
}

short multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(string_id name)
{
	switch (name)
	{
		case _string_id_invalid:
		case _string_id_empty_string:
			return NONE;
	}

	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (universal != nullptr)
	{
		for (short i = 0; i < universal->vehicle_sets.count(); i++)
		{
			if (universal->vehicle_sets[i].name.get_value() == name)
			{
				return i;
			}
		}
	}
	return NONE;
}
