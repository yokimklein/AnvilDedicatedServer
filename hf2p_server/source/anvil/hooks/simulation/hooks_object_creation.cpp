#include "hooks_object_creation.h"
#include <anvil\hooks\hooks.h>
#include <memory\data.h>
#include <memory\tls.h>
#include <nmd_assembly.h>
#include <objects\crates.h>
#include <Patch.hpp>
#include <simulation\game_interface\simulation_game_units.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall event_generate_part_hook()
{
	datum_index object_index;
	__asm mov object_index, esi;
	TLS_DATA_GET_VALUE_REFERENCE(object_headers);

	s_object_header* object_header = (s_object_header*)datum_get(*object_headers, object_index);
	if (!TEST_BIT(_object_mask_crate, object_header->type.get()) || !crate_will_self_destruct(object_index))
	{
		object_set_garbage(object_index, true, 0);
	}
	simulation_action_object_create(object_index);
}

// preserve no_barrel_prediction variable
__declspec(naked) void weapon_barrel_create_projectiles_hook0()
{
	// esp + original sp - variable offset + new stack space
	__asm mov eax, [esp + 0x2254 - 0x2244 + 0x0C];
	__asm mov[ebp + 4], eax;
	__asm retn;
}

// crate projectiles
__declspec(safebuffers) void __fastcall weapon_barrel_create_projectiles_hook1()
{
	datum_index object_index;
	s_object_placement_data* placement_data;
	DEFINE_ORIGINAL_EBP_ESP(0x2254, sizeof(object_index) + sizeof(placement_data));

	__asm mov eax, original_esp;
	__asm lea eax, [eax + 0x2250 - 0x2048];
	__asm mov placement_data, eax;

	object_index = object_new(placement_data);
	if (object_index != -1)
		simulation_action_object_create(object_index);
}

// non-predicted standard projectiles
__declspec(safebuffers) void __fastcall weapon_barrel_create_projectiles_hook2()
{
	datum_index object_index;
	bool no_barrel_prediction;
	DEFINE_ORIGINAL_EBP_ESP(0x2254, sizeof(object_index) + (sizeof(no_barrel_prediction) + 3));

	__asm mov eax, original_esp;
	__asm mov eax, [eax + 0x2250 - 0x2230];
	__asm mov object_index, eax;

	__asm mov eax, original_ebp;
	__asm mov eax, [eax + 4];
	__asm mov no_barrel_prediction, al;

	if (no_barrel_prediction)
		simulation_action_object_create(object_index);
}

// preserve object_force_inside_bsp return
__declspec(naked) void throw_release_hook0()
{
	__asm mov[ebp + 4], eax;
	__asm retn;
}

__declspec(safebuffers) void __fastcall throw_release_hook1()
{
	s_new_unit_action_grenade* action_state_storage;
	datum_index object_index;
	bool force_inside_bsp;
	bool stick;
	DEFINE_ORIGINAL_EBP_ESP(0x58, sizeof(action_state_storage) + sizeof(object_index) + (sizeof(force_inside_bsp) + sizeof(stick) + 2));

	__asm mov action_state_storage, ebx;

	__asm mov object_index, esi;

	__asm mov eax, original_ebp;
	__asm mov eax, [eax + 4];
	__asm mov force_inside_bsp, al;

	__asm mov eax, original_ebp;
	__asm mov eax, [eax + 24];
	__asm mov stick, al;

	// Added these checks which the update would be nested within had there been enough space to fit it
	if (object_index != NONE && !action_state_storage->unk2_4) // TODO: figure out what this field is
	{
		assert(force_inside_bsp == 0x0 || force_inside_bsp == 0x1);
		if (stick || force_inside_bsp) // needs to stay in this order - force inside may not have a valid value if object_force_inside_bsp hasn't run?
		{
			simulation_action_object_create(object_index);
		}
	}
}

__declspec(safebuffers) void __fastcall equipment_activate_hook()
{
	datum_index object_index;
	__asm mov object_index, eax;

	simulation_action_object_create(object_index);
}

__declspec(safebuffers) void __fastcall item_in_unit_inventory_hook()
{
	datum_index object_index;
	__asm mov object_index, edi;

	simulation_action_object_create(object_index);
}
#pragma runtime_checks("", restore)

void __fastcall player_set_facing_player_spawn_hook(datum_index player_index, real_vector3d* forward)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	s_player_datum* player_data = (s_player_datum*)datum_get(*players, player_index);
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

void anvil_hooks_object_creation_apply()
{
	// create & update player biped on spawn (player_spawn)
	Hook(0xBB084, player_set_facing_player_spawn_hook, HookFlags::IsCall).Apply();

	// map variant object spawning
	Hook(0xAEA03, game_engine_register_object_hook, HookFlags::IsCall).Apply(); // c_map_variant::create_object
	Hook(0x172D86, game_engine_register_object_hook, HookFlags::IsCall).Apply(); // c_candy_spawner:spawn_object
	Hook(0x4095BB, game_engine_register_object_hook, HookFlags::IsCall).Apply(); // object_new_from_scenario_internal

	// effect object spawning
	insert_hook(0x114A2F, 0x114A6C, event_generate_part_hook, _hook_replace);
	
	// weapon_barrel_create_projectiles
	add_variable_space_to_stack_frame(0x4371F0, 0x439A51, 4); // Add 4 bytes of variable space to the stack frame
	insert_hook(0x4373B4, 0x4373BB, weapon_barrel_create_projectiles_hook0, _hook_execute_replaced_last); // preserve no_barrel_prediction in our new variable
	insert_hook(0x4391CD, 0x4391D9, weapon_barrel_create_projectiles_hook1, _hook_replace); // crate projectiles
	insert_hook(0x4394A2, 0x4394A8, weapon_barrel_create_projectiles_hook2, _hook_execute_replaced_last); // non-predicted standard projectiles
	insert_hook(0x439A4B, 0x439A50, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

	// grenade & equipment throw spawning
	add_variable_space_to_stack_frame(0x47CE00, 0x47D21C, 4); // Add 4 bytes of variable space to the stack frame
	insert_hook(0x47D174, 0x47D179, throw_release_hook0, _hook_execute_replaced_last); // preserve object_force_inside_bsp's return value
	insert_hook(0x47D185, 0x47D18D, throw_release_hook1, _hook_replace); // create thrown projectiles - we're replacing the inlined function call at the return address in another hook so we can replace this
	insert_hook(0x47D213, 0x47D21B, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
	
	// hologram spawning
	insert_hook(0x45113A, 0x451144, equipment_activate_hook, _hook_execute_replaced_last);

	// item inventory
	insert_hook(0x484337, 0x48433D, item_in_unit_inventory_hook, _hook_execute_replaced_last);
}