#include "weapons.h"
#include <memory\tls.h>

datum_index weapon_get_owner_unit_index(datum_index weapon_index)
{
    TLS_DATA_GET_VALUE_REFERENCE(object_headers);
    weapon_datum* weapon = (weapon_datum*)object_get_and_verify_type(weapon_index, _object_mask_weapon);
    if (weapon->item.inventory_state.get() != _item_inventory_state_free)
    {
        return weapon->item.inventory_unit_index;
    }
    else
    {
        return NONE;
    }
}

void __fastcall weapon_delay_predicted_state(datum_index weapon_index)
{
    INVOKE(0x432310, weapon_delay_predicted_state, weapon_index);
}

long __fastcall weapon_get_owner_unit_inventory_index(datum_index weapon_index)
{
    return INVOKE(0x433D90, weapon_get_owner_unit_inventory_index, weapon_index);
}