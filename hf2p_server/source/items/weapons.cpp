#include "weapons.h"
#include <memory\tls.h>

datum_index weapon_get_owner_unit_index(datum_index weapon_index)
{
    TLS_DATA_GET_VALUE_REFERENCE(object_headers);
    s_object_header* object_header = (s_object_header*)datum_get(*object_headers, weapon_index);
    s_weapon_data* weapon = (s_weapon_data*)object_header->data;
    if (weapon->unknown_state) // 0x17D
        return weapon->owner_unit_index; // 0x188
    else
        return -1;
}

void __fastcall weapon_delay_predicted_state(datum_index weapon_index)
{
    INVOKE(0x432310, weapon_delay_predicted_state, weapon_index);
}

long __fastcall weapon_get_owner_unit_inventory_index(datum_index weapon_index)
{
    return INVOKE(0x433D90, weapon_get_owner_unit_inventory_index, weapon_index);
}