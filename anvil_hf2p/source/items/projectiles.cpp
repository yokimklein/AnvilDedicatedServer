#include "projectiles.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_projectiles.h>
#include <simulation\simulation.h>

void __fastcall projectile_detonate_effects_and_damage(datum_index projectile_index, real_point3d const* position, real_vector3d const* forward, s_effect_vector const* effect_vector, float damage_scale, datum_index object_index, c_flags<e_simulation_projectile_effect_flag, uchar, k_simulation_projectile_effect_flag_count> effect_flags, c_global_material_type material_type, real_vector3d const* hit_normal, s_location const* location, bool valid_material_type, bool collided_with_invalid_material)
{
    projectile_datum* projectile = (projectile_datum*)object_get_and_verify_type(projectile_index, _object_mask_projectile);
    bool invalid_parent_index = projectile->object.parent_object_index == NONE;
    s_damage_owner damage_owner;
    object_get_damage_owner(projectile_index, &damage_owner);
    if (game_is_server() && !effect_flags.is_empty())
    {
        datum_index parent_object_index = projectile->object.parent_object_index;
        bool gamestate_index_invalid = projectile->object.gamestate_index != NONE;
        long player_index = NONE;
        if (parent_object_index != NONE && TEST_BIT(_object_mask_unit, object_header_get(parent_object_index)->type.get()))
        {
            long unit_player_index = unit_get_current_or_last_weak_player_index(parent_object_index);
            if (unit_player_index != NONE)
            {
                TLS_DATA_GET_VALUE_REFERENCE(players);
                if (datum_try_and_get(players, unit_player_index))
                {
                    player_index = unit_player_index;
                }
            }
        }
        bool collided_with_invalid_material = projectile->projectile.flags.test(_projectile_flags21);
        simulation_action_projectile_detonate(projectile->definition_index, position, forward, hit_normal, damage_scale, effect_flags, material_type, location, player_index, damage_owner.player_index, gamestate_index_invalid, valid_material_type, collided_with_invalid_material);
    }
    projectile_detonate_effects_and_damage_shared(projectile->definition_index, position, forward, hit_normal, effect_vector, damage_scale, &damage_owner, object_index, location, projectile->projectile.shot_id, effect_flags, material_type, valid_material_type, collided_with_invalid_material, !invalid_parent_index, NONE, _predictability3);
}

#pragma runtime_checks("", off)
void __fastcall projectile_detonate_effects_and_damage_shared(long definition_index, real_point3d const* position, real_vector3d const* forward, real_vector3d const* hit_vector, s_effect_vector const* effect_vector, float damage_scale, s_damage_owner const* damage_owner, datum_index object_index, s_location const* location, short shot_id, c_flags<e_simulation_projectile_effect_flag, uchar, k_simulation_projectile_effect_flag_count> effect_flags, c_global_material_type material_type, bool valid_material_type, bool collided_with_invalid_material, bool valid_parent_index, datum_index object_index2, e_predictability predictability)
{
    INVOKE(0x466620, projectile_detonate_effects_and_damage_shared, definition_index, position, forward, hit_vector, effect_vector, damage_scale, damage_owner, object_index, location, shot_id, effect_flags, material_type, valid_material_type, collided_with_invalid_material, valid_parent_index, object_index2, predictability);
    __asm add esp, 0x3C; // TODO: check if esp is as it should be after call
}
#pragma runtime_checks("", restore)