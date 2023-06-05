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