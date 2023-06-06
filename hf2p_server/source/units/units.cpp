#include "units.h"
#include "..\game\game.h"
#include "..\memory\tls.h"
#include "..\simulation\game_interface\simulation_game_objects.h"

void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index)
{
    if (!game_is_predicted())
    {
        s_object_header* object_header_data = (s_object_header*)(get_tls()->object_headers->data);
        s_unit_data* unit = (s_unit_data*)object_get(unit_index);
        word cycled_identifier = (unit->current_weapon_set.set_identifier + 1) & 0xF;
        unit->current_weapon_set.set_identifier = cycled_identifier;
        unit->desired_weapon_set.set_identifier = cycled_identifier;

        c_flags<long, ulong64, 64> update_flags = {};
        update_flags.set(17, true);
        simulation_action_object_update(unit_index, &update_flags);
    }
}

void __fastcall unit_delete_all_weapons_internal(datum_index unit_index)
{
    s_object_header* object_header_data = (s_object_header*)(get_tls()->object_headers->data);
    s_unit_data* unit = (s_unit_data*)object_get(unit_index);
    long counter = 0;
    do
    {
        if (unit->weapon_object_indices[counter] != -1)
            unit_inventory_set_weapon_index(unit_index, counter, -1, _unit_drop_type_delete);
        counter++;
    }
    while (counter < 4);
}

void __fastcall unit_inventory_set_weapon_index(datum_index unit_index, datum_index inventory_index, datum_index item_index, e_unit_drop_type drop_type)
{
    // convert fastcall to usercall
    const auto unit_inventory_set_weapon_index_call = (void*)(module_base + 0x426D10);
    __asm
    {
        push drop_type
        push item_index
        mov edx, inventory_index
        mov ecx, unit_index
        call unit_inventory_set_weapon_index_call
        add esp, 8 // this is the only real difference as far as i can tell
    }
}