#include "hooks_damage_events.h"
#include <anvil\hooks\hooks.h>
#include <memory\data.h>
#include <simulation\game_interface\simulation_game_damage.h>

// runtime checks need to be disabled non-naked hooks, make sure to write them within the pragmas
// ALSO __declspec(safebuffers) is required - the compiler overwrites a lot of the registers from the hooked function otherwise making those variables inaccessible
#pragma runtime_checks("", off)
__declspec(safebuffers) void __fastcall damage_section_deplete_hook() // destroy hologram?
{
    datum_index object_index;
    long damage_section_index;
    long response_index;
    DEFINE_ORIGINAL_EBP_ESP(0x44, sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index));

    __asm mov ecx, original_ebp;
    __asm mov eax, [ecx - 0x0C];
    __asm mov object_index, eax;
    __asm mov eax, [ecx + 0x10];
    __asm mov damage_section_index, eax;
    __asm mov eax, [ecx - 0x08];
    __asm mov response_index, eax;

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

__declspec(safebuffers) void __fastcall damage_section_respond_to_damage_hook() // destroy hologram with powerdrain?
{
    datum_index object_index;
    long damage_section_index;
    long response_index;
    DEFINE_ORIGINAL_EBP_ESP(0x40, sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index));

    __asm mov ecx, original_ebp;
    __asm mov eax, [ecx - 0x14];
    __asm mov object_index, eax;
    __asm mov eax, [ecx + 0x10];
    __asm mov damage_section_index, eax;
    __asm mov eax, [ecx - 0x18];
    __asm mov response_index, eax;
    
    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}

__declspec(safebuffers) void __fastcall object_damage_new_hook() // throwing deployable cover?
{
    datum_index object_index;
    long damage_section_index;
    long response_index;
    DEFINE_ORIGINAL_EBP_ESP(0x94, sizeof(object_index) + sizeof(damage_section_index) + sizeof(response_index));

    __asm mov object_index, edi;
    __asm mov eax, original_esp;
    __asm mov eax, [eax + 0x94 - 0x6C];
    __asm mov damage_section_index, eax;
    __asm mov response_index, ebx;

    simulation_action_damage_section_response(object_index, damage_section_index, response_index, _damage_section_receives_area_effect_damage);
}
#pragma runtime_checks("", restore)

void anvil_hooks_damage_events_apply()
{
    // simulation_action_damage_section_response
    insert_hook(0x414EC5, 0x414ECA, damage_section_deplete_hook, _hook_execute_replaced_first);
    insert_hook(0x414B96, 0x414B9E, damage_section_respond_to_damage_hook, _hook_execute_replaced_first);
    insert_hook(0x40C9C4, 0x40C9C9, object_damage_new_hook, _hook_execute_replaced_first);   
}