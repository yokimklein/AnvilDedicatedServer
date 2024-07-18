#include "hooks_object_updates.h"
#include <anvil\hooks\hooks.h>
#include <Patch.hpp>
#include <memory\data.h>
#include <game\players.h>
#include <game\game.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <units\units.h>
#include <units\bipeds.h>
#include <memory\tls.h>
#include <objects\objects.h>

// TODO: DELETE THIS
void __stdcall simulation_action_object_update_with_bitmask2(datum_index object_index, ulong64 raw_bits)
{
    c_simulation_object_update_flags update_flags{};
    update_flags.set_raw(raw_bits);
    simulation_action_object_update_internal(object_index, update_flags);
}

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
// TO RECALCULATE EBP VARIABLE OFFSET: sp + 0x10 + offset, (eg original was [ebp - 0x10], sp was 0x20, (0x20 + 0x10, -0x10) is [ebp + 0x20])
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall object_update_hook()
{
    datum_index object_index;
    __asm mov object_index, edi;
    c_smart_data_array<s_object_header> object_headers;
    s_object_header* object_header;
    object_headers = get_tls()->object_headers;
    object_header = (s_object_header*)datum_get(*object_headers, object_index);
    
    // we've left the scope of this if check where we've hooked, so we need to check it again
    if (!object_header->flags.test(_object_header_do_not_update_bit))
    {
        if (object_needs_rigid_body_update(object_index))
        {
            simulation_action_object_update(object_index, _simulation_generic_update_rigid_body);
        }
    }
}

__declspec(safebuffers) void __fastcall c_map_variant__remove_object_hook()
{
    datum_index object_index;

    __asm mov eax, [edi + 0x4]; // map_variant_placement->object_index
    __asm mov object_index, eax;

    if (game_is_authoritative())
    {
        simulation_action_object_update(object_index, _simulation_object_update_map_variant_index);
    }
}

__declspec(safebuffers) void __fastcall c_map_variant__unknown4_hook1()
{
    datum_index object_index;
    c_simulation_object_update_flags update_flags;

    __asm mov eax, [ebx + esi + 0x134]; // map_variant->objects[object_placement_index].object_index
    __asm mov object_index, eax;

    update_flags.m_flags.clear();
    update_flags.set_flag(object_index, _simulation_object_update_position);
    update_flags.set_flag(object_index, _simulation_object_update_forward_and_up);
    simulation_action_object_update_internal(object_index, update_flags);
}

__declspec(safebuffers) void __fastcall c_map_variant__unknown4_hook2()
{
    datum_index object_index;

    __asm mov eax, [ebx + esi + 0x134]; // map_variant->objects[object_placement_index].object_index
    __asm mov object_index, eax;

    simulation_action_object_update(object_index, _simulation_object_update_parent_state);
}

__declspec(safebuffers) void __fastcall player_set_unit_index_hook2()
{
    datum_index unit_index;
    __asm mov eax, [edi + 0x30];
    __asm mov unit_index, eax;

    simulation_action_object_update(unit_index, _simulation_unit_update_assassination_data);
}

__declspec(safebuffers) void __fastcall unit_died_hook()
{
    __asm mov ecx, eax; // preserve eax value

    c_simulation_object_update_flags update_flags;
    s_unit_data* unit;
    datum_index unit_index;
    DEFINE_ORIGINAL_EBP_ESP(0x74, sizeof(update_flags) + sizeof(unit) + sizeof(unit_index) + 4); // where's this extra 4 bytes of space coming from?

    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x70 - 0x60];
    __asm mov unit_index, eax;
    __asm mov unit, ecx;

    update_flags.m_flags.clear();
    if (unit->object_identifier.type == _object_type_vehicle)
        update_flags.set_flag(unit_index, _simulation_vehicle_update_active_camo);
    else
        update_flags.set_flag(unit_index, _simulation_unit_update_active_camo);
    simulation_action_object_update_internal(unit_index, update_flags);
}

// preserve player_object_index variable
__declspec(naked) void unit_add_initial_loadout_hook0()
{
    __asm mov[ebp + 4], ecx;
    __asm retn;
}

// syncs grenade counts on spawn
__declspec(safebuffers) void __fastcall unit_add_initial_loadout_hook1()
{
    datum_index player_object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C8, sizeof(player_object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov player_object_index, eax;

    simulation_action_object_update(player_object_index, _simulation_unit_update_grenade_counts);
}

// syncs revenge shield bonus on spawn
__declspec(safebuffers) void __fastcall unit_add_initial_loadout_hook2()
{
    datum_index player_object_index;
    DEFINE_ORIGINAL_EBP_ESP(0x1C8, sizeof(player_object_index));

    __asm mov eax, original_ebp;
    __asm mov eax, [eax + 4];
    __asm mov player_object_index, eax;

    simulation_action_object_update(player_object_index, _simulation_object_update_shield_vitality);
}
#pragma runtime_checks("", restore)

void __fastcall player_set_unit_index_hook1(datum_index unit_index, bool unknown)
{
    simulation_action_object_update(unit_index, _simulation_unit_update_control);
    unit_set_actively_controlled(unit_index, unknown);
}

__declspec(naked) void grenade_throw_move_to_hand_hook()
{
    __asm
    {
        // original replaced instructions
        mov[ecx + 0x324], al

        // preserve registers across call
        push ecx
        push edx

        push 0
        push 67108864 // _simulation_unit_update_grenade_counts (1 << 26)
        push[ebp - 0x08] // unit_index
        call simulation_action_object_update_with_bitmask2

        // restore registers
        pop edx
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x47D435
        jmp eax
    }
}

__declspec(naked) void unit_add_grenade_to_inventory_hook()
{
    __asm
    {
        // original replaced instructions
        inc byte ptr[esi + ebx + 0x324]

        // preserve registers across call
        push edx

        push 0
        push 67108864 // _simulation_unit_update_grenade_counts (1 << 26)
        push[ebp - 0x08] // unit_index
        call simulation_action_object_update_with_bitmask2

        // restore registers
        pop edx

        // return
        mov ecx, module_base
        add ecx, 0x4243DF
        jmp ecx
    }
}

__declspec(naked) void unit_add_equipment_to_inventory_hook()
{
    __asm
    {
        // original replaced instructions
        mov[ecx + 0x314], eax

        // preserve registers across call
        push ecx

        push 0
        push 805306368 // _simulation_unit_update_equipment & _simulation_unit_update_equipment_charges ((1 << 28) | (1 << 29))
        push[esp + 0x2C - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask2

        // restore registers
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x42458C
        jmp eax
    }
}

__declspec(naked) void unit_update_control_hook1()
{
    __asm
    {
        // original replaced instructions
        mov[edi + 0x1E4], eax

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask2

        // return
        mov eax, module_base
        add eax, 0x418550
        jmp eax
    }
}

__declspec(naked) void unit_update_control_hook2()
{
    __asm
    {
        // original replaced instructions
        mov[edi + 0x1B4], eax

        // preserve register across call
        push ecx

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask2

        // restore register
        pop ecx

        // return
        mov eax, module_base
        add eax, 0x418693
        jmp eax
    }
}

__declspec(naked) void unit_update_control_hook3()
{
    __asm
    {
        // original replaced instructions
        mov eax, [edx + 0x1A4]

        // preserve register across call
        push edx

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask2

        // restore register
        pop edx

        // return
        mov eax, module_base
        add eax, 0x418A79
        jmp eax
    }
}

__declspec(naked) void projectile_attach_hook()
{
    __asm
    {
        // original replaced instructions
        movss dword ptr[esi + 0x18C], xmm1

        // preserve registers across call
        push eax
        push ecx
        push edx

        push 0
        push 2048 // _simulation_object_update_parent_state (1 << 11)
        push[esp + 0x6C - 0x48] // projectile_index
        call simulation_action_object_update_with_bitmask2

        // restore registers
        pop edx
        pop ecx
        pop eax

        // replaced instructions
        mov eax, [eax + 0x0C]
        mov eax, [eax + 0x44]
        mov cl, [eax + ecx * 8 + 3]
        // return
        mov eax, module_base
        add eax, 0x467F23
        jmp eax
    }
}

__declspec(naked) void unit_set_aiming_vectors_hook1() // c_simulation_unit_entity_definition::apply_object_update
{
    __asm
    {
        push[edi + 0x0B4] // looking_vector
        mov edx, [edi + 0xB4] // aiming_vector
        mov ecx, ebx // unit_index
        call unit_set_aiming_vectors

        // return
        mov eax, module_base
        add eax, 0x59F48
        jmp eax
    }
}

__declspec(naked) void unit_set_aiming_vectors_hook3() // c_vehicle_auto_turret::control
{
    __asm
    {
        lea eax, [esi + 0x8BC]
        push eax // looking_vector
        mov edx, eax // aiming_vector
        mov ecx, ebx // unit_index
        call unit_set_aiming_vectors

        // return
        mov eax, module_base
        add eax, 0x4A1010
        jmp eax
    }
}

__declspec(naked) void c_simulation_weapon_fire_event_definition__apply_object_update_hook1()
{
    __asm
    {
        // original replaced instruction
        mov dword ptr[esp + 0xE0 - 0xC0], 0xFFFFFFFF

        // set new variable
        mov dword ptr[esp + 0xE0 - 0xD4], 0xFFFFFFFF

        // return
        mov edx, module_base
        add edx, 0x60BE5
        jmp edx
    }
}

__declspec(naked) void c_simulation_weapon_fire_event_definition__apply_object_update_hook2()
{
    __asm
    {
        // original replaced instruction
        mov eax, [esp + 0xE0 - 0xC4]
        mov[esp + 0xE0 - 0xC0], edi

        // set new variable
        mov[esp + 0xE0 - 0xD4], edi

        // return
        mov ecx, module_base
        add ecx, 0x60C74
        jmp ecx
    }
}

__declspec(naked) void unit_set_aiming_vectors_hook2() // c_simulation_weapon_fire_event_definition::apply_object_update
{
    __asm
    {
        // replaced instructions
        mov[ecx + 0x1E4], eax

        push 0
        push 65536 // _simulation_unit_update_desired_aiming_vector (1 << 16)
        push[esp + 0xE8 - 0xD4] // unit_index
        call simulation_action_object_update_with_bitmask2

        // return
        mov ecx, module_base
        add ecx, 0x610CF
        jmp ecx
    }
}

__declspec(naked) void equipment_activate_hook2()
{
    __asm
    {
        // replaced instructions
        mov[edi + 0x198], eax

        push 0
        push 65536 // _simulation_item_update_unknown16 (1 << 16)
        push[esp + 0x360 - 0x328] // equipment_index
        call simulation_action_object_update_with_bitmask2

        // return
        mov eax, module_base
        add eax, 0x4514E2
        jmp eax
    }
}

__declspec(naked) void unit_update_energy_hook()
{
    __asm
    {
        // replaced instructions
        mov[esi + 0x31C], eax

        push 0
        push 1073741824 // _simulation_unit_update_consumable_energy (1 << 30)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask2

        // return
        mov eax, module_base
        add eax, 0x41B606
        jmp eax
    }
}

__declspec(naked) void equipment_handle_energy_cost_hook0()
{
    __asm
    {
        // add new variable to preserve unit_index in
        sub esp, 0x10 // increased by 4 bytes from 0x0C
        push ebx
        push esi
        push edi
        mov edi, [eax]
        mov esi, ecx

        // preserve unit_index in new variable
        mov dword ptr[esp + 0x18 - 0x0C], esi

        // return
        mov eax, module_base
        add eax, 0x42D2A6
        jmp eax
    }
}

__declspec(naked) void equipment_handle_energy_cost_hook1()
{
    // Make sure this is static else the stack will break
    static void* sub_2E7BE0 = (void*)BASE_ADDRESS(0x2E7BE0);
    __asm
    {
        // replaced instructions
        call sub_2E7BE0

        push 0
        push 1073741824 // _simulation_unit_update_consumable_energy (1 << 30)
        push[esp + 0x20 - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask2

        // return
        mov eax, module_base
        add eax, 0x42D39D
        jmp eax
    }
}
/*
__declspec(naked) void equipment_handle_energy_cost_hook2()
{
    // Make sure this is static else the stack will break
    static void* hf2p_set_player_cooldown = (void*)BASE_ADDRESS(0xC19E0);
    __asm
    {
        // replaced instructions
        call hf2p_set_player_cooldown

        push 0
        push 536870912 // _simulation_unit_update_equipment_charges (1 << 29)
        push[esp + 0x20 - 0x0C] // unit_index
        call simulation_action_object_update_with_bitmask2

        push 0
        push 262144 // flag _simulation_player_update_equipment_charges (1 << 18)
        push[ebx + 0x198] // unit->player_index
        call simulation_action_game_engine_player_update_with_bitmask
        add esp, 12

        // return
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        retn
    }
}
*/
__declspec(naked) void unit_set_hologram_hook()
{
    __asm
    {
        // replaced instructions
        mov dword ptr[eax + 0x400], 0x3F800000

        // save register across call
        push ecx
        push edx

        // biped/vehicle update
        cmp byte ptr[esi + 0x9A], 1
        jnz biped_update

        // vehicle update
        push 0
        push 131072 // _simulation_vehicle_update_active_camo (1 << 17)
        push ebx // unit_index
        call simulation_action_object_update_with_bitmask2
        jmp return_label

        // biped update
        biped_update :
        push 0
            push 134217728 // _simulation_unit_update_active_camo (1 << 27)
            push ebx // unit_index
            call simulation_action_object_update_with_bitmask2

            // restore register
            return_label :
        pop edx
            pop ecx

            // return
            mov eax, module_base
            add eax, 0x42C578
            jmp eax
    }
}

void anvil_hooks_object_updates_apply()
{
    // add simulation_action_object_update back to object_update
    insert_hook(0x404907, 0x40490E, object_update_hook, _hook_execute_replaced_last);

    // add simulation_action_object_update back to player_set_facing
    Hook(0xB6300, player_set_facing).Apply();

    // c_map_variant::remove_object - should fix map variant object respawn times
    insert_hook(0xADB2B, 0xADB45, c_map_variant__remove_object_hook, _hook_execute_replaced_first, true);

    // c_map_variant::unknown4 - called when objects spawn/respawn on sandtrap's elephants
    insert_hook(0xABA7E, 0xABA86, c_map_variant__unknown4_hook1, _hook_execute_replaced_last); // UNTESTED!!
    insert_hook(0xABAAA, 0xABAB1, c_map_variant__unknown4_hook2, _hook_execute_replaced_first); // UNTESTED!!

    // player_set_unit_index
    Hook(0xB5F8E, player_set_unit_index_hook1, HookFlags::IsCall).Apply(); // hooks nearby unit_set_actively_controlled call
    Hook(0xB60E4, player_set_unit_index_hook1, HookFlags::IsCall).Apply(); // hooks nearby unit_set_actively_controlled call
    insert_hook(0xB6289, 0xB628E, player_set_unit_index_hook2, _hook_execute_replaced_first);

    // player_increment_control_context - fixes player control after respawning
    Hook(0xB9A90, player_increment_control_context).Apply();

    // unit_died - update camo state on death for units & vehicles
    insert_hook(0x421469, 0x421471, unit_died_hook, _hook_execute_replaced_first);

    // sync grenade count after throw
    Pointer::Base(0x47D42F).WriteJump(grenade_throw_move_to_hand_hook, HookFlags::None);
    // sync grenade pickups
    Pointer::Base(0x4243D8).WriteJump(unit_add_grenade_to_inventory_hook, HookFlags::None);
    // sync equipment pickup
    Pointer::Base(0x424586).WriteJump(unit_add_equipment_to_inventory_hook, HookFlags::None);
    // unit_update_control
    Pointer::Base(0x41854A).WriteJump(unit_update_control_hook1, HookFlags::None); // UNTESTED!!
    Pointer::Base(0x41868D).WriteJump(unit_update_control_hook2, HookFlags::None); // called for units with flag bit 2 set
    Pointer::Base(0x418A73).WriteJump(unit_update_control_hook3, HookFlags::None); // sets aim & look vectors for controlled units - ie driving vehicles
    
    // unit_add_initial_loadout - sync spawn loadouts
    add_variable_space_to_stack_frame(0xFB6E0, 0xFBAFD, 4); // Add 4 bytes of variable space to the stack frame
    insert_hook(0xFB6F1, 0xFB6F6, unit_add_initial_loadout_hook0, _hook_execute_replaced_last); // preserve player_object_index in a new variable
    insert_hook(0xFBA34, 0xFBA3A, unit_add_initial_loadout_hook1, _hook_execute_replaced_last); // syncs grenade counts
    insert_hook(0xFBAD9, 0xFBAE0, unit_add_initial_loadout_hook2, _hook_execute_replaced_last); // used to sync the revenge_shield_boost modifier shield bonus
    insert_hook(0xFBAE1, 0xFBAE6, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning
    insert_hook(0xFBAF2, 0xFBAFC, (void*)4, _hook_stack_frame_cleanup); // clean up our new variable before returning

    // projectile_attach - prevents plasma nades from appearing like they can be picked up when stuck to a player
    Pointer::Base(0x467F11).WriteJump(projectile_attach_hook, HookFlags::None);
    // rewrite biped_update_melee_turning w/ updates
    Hook(0x440E30, biped_update_melee_turning).Apply();
    // unit_control
    Hook(0x2BB82, unit_control, HookFlags::IsCall).Apply();
    Hook(0xBD4E7, unit_control, HookFlags::IsCall).Apply();
    Hook(0xBD573, unit_control, HookFlags::IsCall).Apply();
    Hook(0x181410, unit_control, HookFlags::IsCall).Apply();
    Hook(0x69BB96, unit_control, HookFlags::IsCall).Apply();
    Hook(0x69DA76, unit_control, HookFlags::IsCall).Apply();
    // unit_set_aiming_vectors
    Hook(0x42A490, unit_set_aiming_vectors).Apply(); // UNTESTED!! called by c_game_engine::apply_player_update & player_teleport_on_bsp_switch
    Patch::NopFill(Pointer::Base(0x1C9AF5), 3);// remove push 4 after original call to convert usercall to fastcall
    Pointer::Base(0xB911B).Write<byte>(0x20); // 0x24 to 0x20 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    Pointer::Base(0xB911E).Write<byte>(0x10); // 0x14 to 0x10 // stack correction // UNTESTED!! called by player_teleport_on_bsp_switch
    Pointer::Base(0x59EFF).WriteJump(unit_set_aiming_vectors_hook1, HookFlags::None); // UNTESTED!! c_simulation_unit_entity_definition::apply_object_update
    Pointer::Base(0x4A0FCB).WriteJump(unit_set_aiming_vectors_hook3, HookFlags::None); // auto turret aiming direction, but not facing? - c_vehicle_auto_turret::control
    // TODO: This is bad and breaks the stack pointer offsets for other variables
    //Pointer::Base(0x60B68).Write<byte>(0xD8); // expand variable space by 4 bytes to create a new variable (0xD4 to 0xD8)
    //Pointer::Base(0x60BDD).WriteJump(c_simulation_weapon_fire_event_definition__apply_object_update_hook1, HookFlags::None); // set new variable to -1
    //Pointer::Base(0x60C6C).WriteJump(c_simulation_weapon_fire_event_definition__apply_object_update_hook2, HookFlags::None); // preserve unit_index in our new variable for unit_set_aiming_vectors_hook2 to use
    //Pointer::Base(0x610C9).WriteJump(unit_set_aiming_vectors_hook2, HookFlags::None); // handles recoil - c_simulation_weapon_fire_event_definition::apply_object_update
    // equipment_activate
    Pointer::Base(0x4514DC).WriteJump(equipment_activate_hook2, HookFlags::None); // TODO: this doesn't seem to work the way I expect it to - deployable covers still don't activate for clients
    // sync unit energy levels
    Pointer::Base(0x41B600).WriteJump(unit_update_energy_hook, HookFlags::None);
    // TODO: creates new variable and breaks SP in doing so, fix this
    // sync energy costs / energy levels decreasing when throwing an equipment
    //Pointer::Base(0x42D29C).WriteJump(equipment_handle_energy_cost_hook0, HookFlags::None);
    //Pointer::Base(0x42D398).WriteJump(equipment_handle_energy_cost_hook1, HookFlags::None);
    //Pointer::Base(0x42D3ED).WriteJump(equipment_handle_energy_cost_hook2, HookFlags::None); // includes player update
    // unit_set_hologram
    Pointer::Base(0x42C56E).WriteJump(unit_set_hologram_hook, HookFlags::None);
}