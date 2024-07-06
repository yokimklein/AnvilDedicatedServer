#include "hooks_object_creation.h"
#include <anvil\hooks\hooks.h>
#include <anvil\server_tools.h>
#include <Patch.hpp>
#include <memory\data.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <objects\crates.h>
#include <nmd_assembly.h>

// runtime checks need to be disabled for these, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
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

// preserve no_barrel_prediction
thread_local static bool no_barrel_prediction;
__declspec(safebuffers) void __fastcall weapon_barrel_create_projectiles_hook0()
{
	__asm mov eax, [esp + 0x2264 - 0x2244]; // TODO: thread local crashes, also debug disassembly to ensure we're getting the right variable here
	__asm mov no_barrel_prediction, al;
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

datum_index __fastcall weapon_barrel_create_projectiles_hook1(s_object_placement_data* placement_data)
{
	datum_index object_index = object_new(placement_data);
	if (object_index != -1)
		simulation_action_object_create(object_index);
	return object_index;
}

__declspec(naked) void weapon_barrel_create_projectiles_hook2()
{
	__asm
	{
		// call our inserted function
		// if (v220) 
		cmp[esp + 0x2250 - 0x2244], 0 // v220
		jz if_failed

		push[esp + 0x2250 - 0x2228] // object_index 0x2228
		call simulation_action_object_create
		add esp, 4

		if_failed:
		// execute the instructions we replaced
		mov eax, [esp + 0x2250 - 0x21F8]
		xor dl, dl

		// return back to the original code
		mov ecx, module_base
		add ecx, 0x4394A8
		jmp ecx
	}
}

__declspec(naked) void throw_release_hook1()
{
	__asm
	{
		cmp[ebp + 0x20], 0
		jnz sim_update
		mov eax, module_base
		add eax, 0x47CFC7
		jmp eax

		sim_update :
		push esi // object_index
		call simulation_action_object_create
		add esp, 4

		mov eax, module_base
		add eax, 0x47D185
		jmp eax
	}
}

__declspec(naked) void throw_release_hook2()
{
	__asm
	{
		add esp, 4
		test al, al
		jnz sim_update
		mov eax, module_base
		add eax, 0x47D17B
		jmp eax

		sim_update :
		push esi // object_index
		call simulation_action_object_create
		add esp, 4

		mov eax, module_base
		add eax, 0x47D185
		jmp eax
	}
}

__declspec(naked) void equipment_activate_hook()
{
	__asm
	{
		// preserve register
		push eax

		push eax // object_index
		call simulation_action_object_create
		add esp, 4

		// restore register
		pop eax

		// original replaced instructions
		mov ecx, [esp + 0x358 - 0x348]
		mov[ecx + 0x1A0], eax

		// return
		mov eax, module_base
		add eax, 0x451498
		jmp eax
	}
}

__declspec(naked) void item_in_unit_inventory_hook1()
{
	__asm
	{
		push edi // object_index
		call simulation_action_object_create
		add esp, 4

		// original replaced instructions
		mov eax, [ebx + 0x0C]
		mov ecx, [ebp - 0x08]

		// return
		mov edx, module_base
		add edx, 0x48433D
		jmp edx
	}
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
	//insert_hook(0x4373B4, 0x4373BB, weapon_barrel_create_projectiles_hook0, _hook_execute_replaced_last); // preserve variable used in hook2
	Hook(0x4391D4, weapon_barrel_create_projectiles_hook1, HookFlags::IsCall).Apply(); // crate projectiles // hooks nearby object_new
	// TODO: Preserve v220 variable here for this to work
	//Pointer::Base(0x4394A2).WriteJump(weapon_barrel_create_projectiles_hook2, HookFlags::None); // standard projectiles
	
	//insert_hook(0x114A2F, 0x114A6C, event_generate_part_hook, _hook_execute_replaced_last);
	increase_positive_ebp_offsets(0x4371F0, 0x439A50, 4);

	// grenade & equipment throw spawning
	//insert_hook(0x47CFBD, );
	Pointer::Base(0x47CFBD).WriteJump(throw_release_hook1, HookFlags::None);
	Pointer::Base(0x47D174).WriteJump(throw_release_hook2, HookFlags::None);
	// hologram spawning
	Pointer::Base(0x45113A).WriteJump(equipment_activate_hook, HookFlags::None);
	// item inventory
	Pointer::Base(0x484337).WriteJump(item_in_unit_inventory_hook1, HookFlags::None);
}