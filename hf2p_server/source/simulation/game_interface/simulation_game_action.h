#pragma once
#include <memory\data.h>

enum e_simulation_object_update_flag;
enum e_simulation_weapon_update_flag;
enum e_simulation_generic_update_flag;
enum e_simulation_item_update_flag;
enum e_simulation_projectile_update_flag;
enum e_simulation_vehicle_update_flag;
enum e_simulation_device_update_flag;
enum e_simulation_unit_update_flag;
enum e_simulation_entity_type;
enum e_simulation_player_update_flag;
enum e_simulation_statborg_update_flag;

// CAREFUL USING THIS IN HOOKS - constructor is called by default and will immediately use a register!
class c_simulation_object_update_flags : public c_flags<long, ulong64, 64>
{
public:
	c_simulation_object_update_flags();
	c_simulation_object_update_flags(datum_index object_index, e_simulation_object_update_flag flag);
	c_simulation_object_update_flags(datum_index weapon_index, e_simulation_weapon_update_flag flag);
	c_simulation_object_update_flags(datum_index generic_index, e_simulation_generic_update_flag flag);
	c_simulation_object_update_flags(datum_index item_index, e_simulation_item_update_flag flag);
	c_simulation_object_update_flags(datum_index projectile_index, e_simulation_projectile_update_flag flag);
	c_simulation_object_update_flags(datum_index vehicle_index, e_simulation_vehicle_update_flag flag);
	c_simulation_object_update_flags(datum_index device_index, e_simulation_device_update_flag flag);
	c_simulation_object_update_flags(datum_index unit_index, e_simulation_unit_update_flag flag);
	c_simulation_object_update_flags(datum_index player_index, e_simulation_player_update_flag flag);
	c_simulation_object_update_flags(e_simulation_statborg_update_flag flag);

	void set_flag(datum_index object_index, e_simulation_object_update_flag flag);
	void set_flag(datum_index weapon_index, e_simulation_weapon_update_flag flag);
	void set_flag(datum_index generic_index, e_simulation_generic_update_flag flag);
	void set_flag(datum_index item_index, e_simulation_item_update_flag flag);
	void set_flag(datum_index projectile_index, e_simulation_projectile_update_flag flag);
	void set_flag(datum_index vehicle_index, e_simulation_vehicle_update_flag flag);
	void set_flag(datum_index device_index, e_simulation_device_update_flag flag);
	void set_flag(datum_index unit_index, e_simulation_unit_update_flag flag);
	void set_flag(datum_index player_index, e_simulation_player_update_flag flag);
	void set_flag(e_simulation_statborg_update_flag flag);

	//e_simulation_entity_type get_simulation_entity_type(datum_index object_index);
};

template<typename t_flag_enum>
__declspec(noinline) void __fastcall simulation_action_object_update(datum_index object_index, t_flag_enum flag);

void simulation_action_game_engine_globals_create();
void simulation_action_game_engine_globals_delete();
void simulation_action_game_statborg_create();
void simulation_action_game_statborg_delete();
void simulation_action_game_ai_create();
void simulation_action_game_ai_delete();
void simulation_action_game_map_variant_create_all();
void simulation_action_game_map_variant_create_for_chunk(long chunk_index);
void simulation_action_game_map_variant_delete();
void simulation_action_breakable_surfaces_create();
//void simulation_action_actor_create(datum_index unit_index);