#include "hooks_object_creation.h"
#include <anvil\hooks\hooks.h>
#include <memory\data.h>
#include <memory\tls.h>
#include <nmd_assembly.h>
#include <objects\crates.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <game\game_engine_candy_monitor.h>
#include <motor\actions.h>

void __cdecl event_generate_part_hook(s_hook_registers& registers)
{
	datum_index object_index = registers.esi;

	TLS_DATA_GET_VALUE_REFERENCE(object_headers);

	object_header_datum* object_header = (object_header_datum*)datum_get(object_headers, object_index);
	if (!TEST_BIT(_object_mask_crate, object_header->type.get()) || !crate_will_self_destruct(object_index))
	{
		object_set_garbage(object_index, true, 0);
	}
	simulation_action_object_create(object_index);
}

// preserve no_barrel_prediction variable
void __cdecl weapon_barrel_create_projectiles_hook0(s_hook_registers& registers)
{
	// esp + original sp - variable offset + new stack space
	bool* no_barrel_prediction = (bool*)(registers.ebp + 0x04);
	*no_barrel_prediction = *(bool*)(registers.esp + 0x2250 - 0x2244);
}

// crate projectiles
void __cdecl weapon_barrel_create_projectiles_hook1(s_hook_registers& registers)
{
	s_object_placement_data* placement_data = (s_object_placement_data*)(registers.esp + 0x2250 - 0x2048);

	datum_index object_index = object_new(placement_data);
	if (object_index != NONE)
	{
		simulation_action_object_create(object_index);
	}
}

// non-predicted standard projectiles
void __cdecl weapon_barrel_create_projectiles_hook2(s_hook_registers& registers)
{
	datum_index object_index = *(datum_index*)(registers.esp + 0x2250 - 0x2230);
	bool no_barrel_prediction = *(bool*)(registers.ebp + 0x04);

	if (no_barrel_prediction)
	{
		simulation_action_object_create(object_index);
	}
}

// preserve object_force_inside_bsp return
void __cdecl throw_release_hook0(s_hook_registers& registers)
{
	bool* force_inside_bsp = (bool*)(registers.ebp + 0x04);
	*force_inside_bsp = (bool)registers.eax;
}

void __cdecl throw_release_hook1(s_hook_registers& registers)
{
	s_action_state* action_state_storage = (s_action_state*)registers.ebx;
	datum_index object_index = (datum_index)registers.esi;
	bool force_inside_bsp = *(bool*)(registers.ebp + 0x04);
	bool force_inside_bsp_valid = (force_inside_bsp == 0x0 || force_inside_bsp == 0x1);
	bool force_creation = *(bool*)(registers.ebp + 0x24); // $NOTE: originally + 0x20, but the stack frame has been increased by 4 bytes
	ASSERT(force_creation == 0x00 || force_creation == 0x01, "force_creation contains undefined data! this should never happen!");

	// Added these checks which the update would be nested within had there been enough space to fit it
	if (object_index != NONE && !action_state_storage->throw_predicted)
	{
		// needs to stay in this order - force_inside_bsp will contain an undefined value if object_force_inside_bsp hasn't run
		if (force_creation || (force_inside_bsp_valid && force_inside_bsp))
		{
			simulation_action_object_create(object_index);
		}
	}
}

// redirect code away from above hook so it isn't accidentally called from this path
void __cdecl throw_release_hook3(s_hook_registers& registers)
{
	datum_index object_index = registers.esi;
	object_delete(object_index);
	registers.esi = NONE;
}

void __cdecl equipment_activate_hook(s_hook_registers& registers)
{
	datum_index object_index = (datum_index)registers.eax;

	simulation_action_object_create(object_index);
}

void __cdecl item_in_unit_inventory_hook(s_hook_registers& registers)
{
	datum_index object_index = (datum_index)registers.edi;

	simulation_action_object_create(object_index);
}

//void __cdecl actor_place_hook(s_hook_registers& registers) // TODO: UNTESTED!!
//{
//	datum_index unit_index = *(datum_index*)(registers.ebp - 0x24);
//
//	if (unit_index != NONE)
//	{
//		simulation_action_actor_create(unit_index);
//	}
//}

void __cdecl unit_drop_plasma_on_death_hook(s_hook_registers& registers)
{
	datum_index projectile_index = (datum_index)registers.eax;

	simulation_action_object_create(projectile_index);
}

void __fastcall player_set_facing_player_spawn_hook(datum_index player_index, real_vector3d* forward)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player_data = (player_datum*)datum_get(players, player_index);
	simulation_action_object_create(player_data->unit_index);
	simulation_action_object_update(player_data->unit_index, _simulation_unit_update_control);

	// this part was removed from HO
	//if ( game_is_multiplayer() )
	//{
	//  game_engine_add_starting_equipment(player_object_index);
	//  game_engine_spawn_influencer_record_player_spawn(player_index3);
	//}

	player_set_facing(player_index, forward);
}

void __fastcall game_engine_register_object_hook(datum_index object_index)
{
	game_engine_register_object(object_index);
	simulation_action_object_create(object_index);
}

void __cdecl create_flag_at_position_hook(s_hook_registers& registers)
{
	s_object_placement_data* placement_data = (s_object_placement_data*)registers.ecx;

	datum_index object_index = object_new(placement_data);

	if (object_index != NONE)
	{
		simulation_action_object_create(object_index);
	}

	registers.eax = object_index; // return object index
}

void anvil_hooks_object_creation_apply()
{
	// create & update player biped on spawn (player_spawn)
	hook::call(0xBB084, player_set_facing_player_spawn_hook);

	// map variant object spawning
	hook::call(0xAEA03, game_engine_register_object_hook); // c_map_variant::create_object
	hook::call(0x172D86, game_engine_register_object_hook); // c_candy_spawner:spawn_object
	hook::call(0x4095BB, game_engine_register_object_hook); // object_new_from_scenario_internal

	// effect object spawning
	hook::insert(0x114A2F, 0x114A6C, event_generate_part_hook, _hook_replace);
	
	// weapon_barrel_create_projectiles
	hook::add_variable_space_to_stack_frame(0x4371F0, 0x439A51, 4); // Add 4 bytes of variable space to the stack frame
	hook::insert(0x4373B4, 0x4373BB, weapon_barrel_create_projectiles_hook0, _hook_execute_replaced_last); // preserve no_barrel_prediction in our new variable
	hook::insert(0x4391CD, 0x4391D9, weapon_barrel_create_projectiles_hook1, _hook_replace); // crate projectiles
	hook::insert(0x4394A2, 0x4394A8, weapon_barrel_create_projectiles_hook2, _hook_execute_replaced_last); // non-predicted standard projectiles
	hook::insert(0x439A4B, 0x439A50, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

	// grenade & equipment throw spawning
	hook::add_variable_space_to_stack_frame(0x47CE00, 0x47D21C, 4); // Add 4 bytes of variable space to the stack frame
	hook::insert(0x47D174, 0x47D179, throw_release_hook0, _hook_execute_replaced_last); // preserve object_force_inside_bsp's return value
	hook::insert(0x47D185, 0x47D18D, throw_release_hook1, _hook_replace); // create thrown projectiles - we're replacing the inlined function call at the return address in another hook so we can replace this
	hook::insert(0x47D211, 0x47D21B, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
	hook::insert(0x47D17B, 0x47D18D, throw_release_hook3, _hook_replace, FLAG(_hook_no_nop)); // redirect code path to avoid accidentally calling above hook
	patch::nop_region(0x47D180, 5); // cleanup leftover instruction bytes

	// hologram spawning
	hook::insert(0x45113A, 0x451144, equipment_activate_hook, _hook_execute_replaced_last);

	// item inventory
	hook::insert(0x484337, 0x48433D, item_in_unit_inventory_hook, _hook_execute_replaced_last);

	// actor place
	//patch::bytes(0x6989CB, { 0x0F, 0x84, 0x6D, 0x00, 0x00, 0x00 }); // redirect jump to hook instead of return
	//patch::bytes(0x6989E0, { 0x0F, 0x86, 0x58, 0x00, 0x00, 0x00 }); // redirect jump to hook instead of return
	//patch::bytes(0x6989F5, { 0x74, 0x47 }); // redirect jump to hook instead of return
	//patch::bytes(0x698A35, { 0x74, 07 }); // redirect jump to hook instead of return
	//hook::insert(0x698A3E, 0x698A47, actor_place_hook, _hook_execute_replaced_last);

	// final gambit modifier plasma grenade
	hook::insert(0x426F19, 0x426F20, unit_drop_plasma_on_death_hook, _hook_execute_replaced_last);

	// ctf & derived gamemodes flag spawning
	hook::insert(0x22A596, 0x22A59B, create_flag_at_position_hook, _hook_replace);
}