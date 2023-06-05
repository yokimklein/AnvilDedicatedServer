#include "weapons.h"
#include "..\memory\tls.h"

datum_index weapon_get_owner_unit_index(datum_index weapon_index)
{
    s_object_header* object_header = (s_object_header*)datum_get(get_tls()->object_headers, weapon_index);
    s_weapon_data* weapon = (s_weapon_data*)object_header->data;
    if (weapon->unknown_state) // 0x17D
        return weapon->owner_unit_index; // 0x188
    else
        return -1;
}