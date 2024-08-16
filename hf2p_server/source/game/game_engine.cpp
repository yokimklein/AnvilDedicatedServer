#include "game_engine.h"
#include <simulation\simulation.h>
#include <simulation\game_interface\simulation_game_action.h>
#include <memory\tls.h>
#include <game\game_engine_util.h>
#include <game\game_engine_event_definitions.h>
#include <game\game.h>
#include <game\game_engine_team.h>
#include <game\game_results.h>
#include <tag_files\string_ids.h>
#include "assert.h"
#include <game\game_time.h>
#include <game\players.h>
#include <game\game_engine_spawning.h>
#include <networking\session\network_session_state.h>
#include <networking\network_utilities.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_life_cycle.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <simulation\game_interface\simulation_game_engine_globals.h>
#include <scenario\scenario.h>
#include <game\multiplayer_definitions.h>

REFERENCE_DECLARE_ARRAY(0xF01EC0, c_game_engine*, game_engines, k_game_engine_type_count);

void game_engine_attach_to_simulation()
{
	simulation_action_game_engine_globals_create();
	simulation_action_game_statborg_create();
	simulation_action_game_ai_create();
	if (game_engine_is_sandbox())
		simulation_action_game_map_variant_create_all();
	for (short i = 0; i < k_maximum_multiplayer_players; i++)
		simulation_action_game_engine_player_create(i);
	simulation_action_breakable_surfaces_create();
}

void game_engine_detach_from_simulation_gracefully()
{
	c_game_engine* game_engine = current_game_engine();
	game_engine->game_ending();
	simulation_action_game_engine_globals_delete();
	simulation_action_game_statborg_delete();
	simulation_action_game_ai_delete();
	if (game_engine_is_sandbox())
		simulation_action_game_map_variant_delete();
	for (short i = 0; i < k_maximum_multiplayer_players; i++)
		simulation_action_game_engine_player_delete(i);
}

void __fastcall game_engine_player_added(datum_index player_index)
{
	if (current_game_engine())
	{
		TLS_DATA_GET_VALUE_REFERENCE(players);
		if (game_is_authoritative() && game_engine_has_teams())
		{
			player_datum* player_data = (player_datum*)datum_get(*players, player_index);
			game_engine_recompute_active_teams();
			game_engine_adjust_team_score_for_composition(player_data->configuration.host.team_index);
		}

		game_engine_assemble_player_traits(player_index);
		current_game_engine()->player_added(player_index);
		game_engine_get_statborg()->player_added(player_index);

		if (game_ticks_to_seconds(game_time_get()) > 1.0)
		{
			s_game_engine_event_data event_data;
			game_engine_initialize_event(_multiplayer_event_type_general, STRING_ID(game_engine, general_event_player_joined), &event_data);
			game_engine_send_event(&event_data);
		}

		if (game_is_authoritative())
		{
			player_datum* player_data = (player_datum*)datum_get(*players, player_index);
			player_data->lives = current_game_variant()->get_active_variant()->get_respawn_options()->get_lives_per_round();
			simulation_action_game_engine_player_update(player_index, _simulation_player_update_lives_remaining);
		}

		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		for (long i = -1; ; game_engine_globals->fade_to_black_active_user_mask |= 1 << i)
		{
			if (player_index != -1)
				i = player_mapping_get_next_output_user(DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index), i);
			if (i == -1)
				break;
			game_engine_globals->fade_to_black_amount[i] = 1.0;
		}

		game_results_notify_player_indices_changed();
		game_results_statistic_set(DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index), _game_team_none, 0, 1);
	}
}

long game_engine_round_time_get()
{
	assert(game_is_authoritative());
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	
	long game_time = game_time_get();
	return MAX(0, game_time - game_engine_globals->round_timer);
}

void game_engine_update_round_conditions()
{
	if (game_is_authoritative())
	{
		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		long round_time = game_engine_round_time_get();
		long game_over_timer = game_engine_globals->game_over_timer;
		c_flags<e_game_engine_round_condition, byte, k_game_engine_round_condition_count> round_condition_flags;
		round_condition_flags.set(_game_engine_round_condition_unknown0, round_time < 5); // 5
		round_condition_flags.set(_game_engine_round_condition_unknown1, round_time < game_seconds_integer_to_ticks(1)); // 1
		round_condition_flags.set(_game_engine_round_condition_welcome, round_time < game_seconds_integer_to_ticks(k_pre_round_seconds + 3)); // 3
		round_condition_flags.set(_game_engine_round_condition_waiting_for_players, round_time < game_seconds_integer_to_ticks(MAX(k_pre_round_seconds - 14, 0))); // 4
		round_condition_flags.set(_game_engine_round_condition_match_introduction, round_time < game_seconds_integer_to_ticks(MAX(k_pre_round_seconds - 7, 0))); // 11
		round_condition_flags.set(_game_engine_round_condition_unknown6, round_time < game_seconds_integer_to_ticks(5)); // 5
		round_condition_flags.set(_game_engine_round_condition_spawn_players, round_time <= game_seconds_integer_to_ticks(k_pre_round_seconds)); // 18
		round_condition_flags.set(_game_engine_round_condition_unknown7, game_over_timer >= game_seconds_integer_to_ticks(4)); // 4
		if (game_engine_globals->round_condition_flags != round_condition_flags)
		{
			if (game_engine_globals->round_condition_flags.test(_game_engine_round_condition_waiting_for_players) &&
				!round_condition_flags.test(_game_engine_round_condition_waiting_for_players))
			{
				c_player_in_game_iterator player_iterator;
				player_iterator.begin();
				while (player_iterator.next())
				{
					datum_index player_index = player_iterator.get_index();
					current_game_engine()->emit_game_start_event(player_index);
				}
			}
			simulation_action_game_engine_globals_update(_simulation_game_engine_globals_update_round_condition);
			game_engine_globals->round_condition_flags = round_condition_flags;
		}
	}
}

bool game_engine_round_condition_test(e_game_engine_round_condition condition)
{
	assert(VALID_INDEX(condition, k_game_engine_round_condition_count));
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);

	return game_engine_globals->round_condition_flags.test(condition);
}

bool game_engine_in_round()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	return (current_game_engine() && game_engine_globals->current_state == _game_engine_state_round_progress && (game_is_predicted() || game_engine_globals->desired_state == _game_engine_state_round_progress));
}

bool __fastcall game_engine_player_is_playing(datum_index player_index)
{
	return INVOKE(0xC77A0, game_engine_player_is_playing, player_index);
}

void __fastcall game_engine_player_set_spawn_timer(datum_index player_index, long timer_ticks)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	
	player->respawn_timer_countdown_ticks = timer_ticks;
	player->respawn_timer_countdown_seconds = CLAMP(game_ticks_to_seconds_ceil(timer_ticks), 0, 1023);
	simulation_action_game_engine_player_update(player_index, _simulation_player_update_spawn_timer);
}

void __fastcall game_engine_get_multiplayer_string(string_id id, c_static_wchar_string<1024>* out_multiplayer_string)
{
	INVOKE(0xCD1F0, game_engine_get_multiplayer_string, id, out_multiplayer_string);
}

void game_engine_player_activated(datum_index player_index)
{
	if (current_game_engine())
	{
		current_game_engine()->player_activated(player_index);
	}
}

void __fastcall game_engine_boot_player_safe(datum_index player_index, datum_index spectating_player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	player->spectating_player_index = datum_absolute_index_to_index(*players, spectating_player_index);
	simulation_action_game_engine_player_update(player_index, _simulation_player_update_spectating_player);
}

void __fastcall game_engine_boot_player(datum_index booted_player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, booted_player_index);
	if (player != nullptr)
	{
		s_game_engine_event_data event_data;
		game_engine_initialize_event(_multiplayer_event_type_general, STRING_ID(game_engine, general_event_player_booted_player), &event_data);
		game_engine_set_event_effect_player_and_team(booted_player_index, &event_data);
		game_engine_send_event(&event_data);
		simulation_boot_machine(&player->machine_identifier, _network_session_boot_reason_player_booted_player);
	}
}

void __fastcall game_engine_set_player_navpoint_action(datum_index player_index, e_navpoint_action action)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	s_player_waypoint_data* waypoint = &game_engine_globals->player_waypoints[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)];
	if (!game_is_predicted())
	{
		e_navpoint_action old_action = waypoint->action1;
		player_navpoint_data_set_action(waypoint, action);
		if (old_action != waypoint->action1.get())
		{
			simulation_action_game_engine_player_update(player_index, _simulation_player_update_waypoint_action);
		}
	}
}

void __fastcall update_player_navpoint_data(datum_index player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	s_player_waypoint_data* waypoint = &game_engine_globals->player_waypoints[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)];
	if (!game_is_predicted())
	{
		e_navpoint_action old_action = waypoint->action1.get();
		player_navpoint_data_update(waypoint);
		if (old_action != waypoint->action1.get())
		{
			simulation_action_game_engine_player_update(player_index, _simulation_player_update_waypoint_action);
		}
	}
	if (waypoint->respawn_timer1 != 0)
	{
		waypoint->respawn_timer2 = MAX(waypoint->respawn_timer2 - 1, 0);
		if (waypoint->respawn_timer2 == 0)
		{
			waypoint->dead = false;
			waypoint->respawn_timer1 = 0;
			waypoint->respawn_timer2 = 0;
		}
	}
}

void __fastcall player_navpoint_data_update(s_player_waypoint_data* waypoint)
{
	if (waypoint->ticks > 0)
	{
		waypoint->ticks--;
		if (waypoint->ticks == 0)
		{
			if (waypoint->action2.get() != _navpoint_action_none)
			{
				waypoint->action1 = waypoint->action2;
				waypoint->ticks = (byte)game_seconds_to_ticks_round(0.5f); // This seems bad, wouldn't this cause issues if you have more than 255 ticks per second?
				waypoint->action2 = _navpoint_action_none;
			}
			else
			{
				waypoint->action1 = _navpoint_action_none;
				waypoint->ticks = 0;
			}
		}
	}
}

void __fastcall game_engine_player_indices_swapped(long player_1_absolute_index, long player_2_absolute_index)
{
	if (current_game_engine())
	{
		game_engine_get_statborg()->player_indices_swapped(player_1_absolute_index, player_2_absolute_index);
	}
}

void __fastcall game_engine_apply_appearance_traits(datum_index player_index, c_player_trait_appearance* trait)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	bool player_update = false;
	bool update_camo = false;
	if (player->traits.get_appearance_traits()->get_active_camo_setting() != trait->get_active_camo_setting())
	{
		player->traits.get_appearance_traits_writeable()->set_active_camo_setting(trait->get_active_camo_setting(), false);
		player_update = true;
		update_camo = game_is_authoritative();
	}
	if (player->traits.get_appearance_traits()->get_waypoint_setting() != trait->get_waypoint_setting())
	{
		player->traits.get_appearance_traits_writeable()->set_waypoint_setting(trait->get_waypoint_setting(), false);
		player_update = true;
	}
	if (player->traits.get_appearance_traits()->get_aura_setting() != trait->get_aura_setting())
	{
		player->traits.get_appearance_traits_writeable()->set_aura_setting(trait->get_aura_setting(), false);
		player_update = true;
	}
	if (player->traits.get_appearance_traits()->get_forced_change_color_setting() != trait->get_forced_change_color_setting())
	{
		player->traits.get_appearance_traits_writeable()->set_forced_change_color_setting(trait->get_forced_change_color_setting(), false);
		player_update = true;
	}
	if (player_update)
	{
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_appearance_traits);
	}
	if (update_camo)
	{
		if (player->unit_index != NONE)
		{
			float active_camo_value;
			const long minimum_value = _active_camo_setting_off;
			switch (trait->get_active_camo_setting())
			{
				case _active_camo_setting_poor:
				case _active_camo_setting_good:
				case _active_camo_setting_invisible:
					active_camo_value = k_active_camo_values[trait->get_active_camo_setting() - minimum_value];
					unit_active_camouflage_set_maximum(player->unit_index, active_camo_value);
					unit_active_camouflage_set_level(player->unit_index, 4.0f, NONE);
					break;
				default:
					unit_active_camouflage_set_maximum(player->unit_index, 1.0f);
					unit_active_camouflage_disable(player->unit_index, 4.0f);
					break;
			}
		}
	}
}

void __fastcall game_engine_apply_movement_traits(datum_index player_index, c_player_trait_movement* trait)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	bool player_update = false;
	if (player->traits.get_movement_traits()->get_speed() != trait->get_speed())
	{
		player->traits.get_movement_traits_writeable()->set_speed_setting(trait->get_speed_setting(), false);
		player_update = true;
	}
	if (player->traits.get_movement_traits()->get_gravity() != trait->get_gravity())
	{
		player->traits.get_movement_traits_writeable()->set_gravity_setting(trait->get_gravity_setting(), false);
		player_update = true;
	}
	if (player->traits.get_movement_traits()->get_vehicle_usage_setting() != trait->get_vehicle_usage_setting())
	{
		player->traits.get_movement_traits_writeable()->set_vehicle_usage_setting(trait->get_vehicle_usage_setting(), false);
		player_update = true;
	}
	if (player_update)
	{
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_movement_traits);
	}
}

void __fastcall game_engine_apply_sensors_traits(datum_index player_index, c_player_trait_sensors* trait)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	bool player_update = false;
	if (player->traits.get_sensor_traits()->get_motion_tracker_setting() != trait->get_motion_tracker_setting())
	{
		player->traits.get_sensor_traits_writeable()->set_motion_tracker_setting(trait->get_motion_tracker_setting(), false);
		player_update = true;
	}
	if (player->traits.get_sensor_traits()->get_motion_tracker_range_setting() != trait->get_motion_tracker_range_setting())
	{
		player->traits.get_sensor_traits_writeable()->set_motion_tracker_range_setting(trait->get_motion_tracker_range_setting(), false);
		player_update = true;
	}
	if (player_update)
	{
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_sensor_traits);
	}
}

void __fastcall game_engine_apply_shield_vitality_traits(datum_index player_index, c_player_trait_shield_vitality* trait)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	bool player_update = false;
	if (player->traits.get_shield_vitality_traits()->get_damage_resistance_percentage_setting() != trait->get_damage_resistance_percentage_setting())
	{
		player->traits.get_shield_vitality_traits_writeable()->set_damage_resistance_percentage_setting(trait->get_damage_resistance_percentage_setting(), false);
		player_update = true;
	}
	if (player->traits.get_shield_vitality_traits()->get_shield_recharge_rate_percentage_setting() != trait->get_shield_recharge_rate_percentage_setting())
	{
		player->traits.get_shield_vitality_traits_writeable()->set_shield_recharge_rate_percentage_setting(trait->get_shield_recharge_rate_percentage_setting(), false);
		player_update = true;
	}
	if (player->traits.get_shield_vitality_traits()->get_vampirism_percentage_setting() != trait->get_vampirism_percentage_setting())
	{
		player->traits.get_shield_vitality_traits_writeable()->set_vampirism_percentage_setting(trait->get_vampirism_percentage_setting(), false);
		player_update = true;
	}
	if (player->traits.get_shield_vitality_traits()->get_headshot_immunity_setting() != trait->get_headshot_immunity_setting())
	{
		player->traits.get_shield_vitality_traits_writeable()->set_headshot_immunity_setting(trait->get_headshot_immunity_setting(), false);
		player_update = true;
	}
	if (player->traits.get_shield_vitality_traits()->get_shield_multiplier_setting() != trait->get_shield_multiplier_setting())
	{
		player->traits.get_shield_vitality_traits_writeable()->set_shield_multiplier_setting(trait->get_shield_multiplier_setting(), false);
		player_update = true;
	}
	if (player_update)
	{
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_shield_vitality_traits);
	}
}

void __fastcall game_engine_apply_weapons_traits(datum_index player_index, c_player_trait_weapons* trait)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	bool player_update = false;
	if (player->traits.get_weapons_traits()->get_damage_modifier_percentage_setting() != trait->get_damage_modifier_percentage_setting())
	{
		player->traits.get_weapons_traits_writeable()->set_damage_modifier_percentage_setting(trait->get_damage_modifier_percentage_setting(), false);
		player_update = true;
	}
	if (player->traits.get_weapons_traits()->get_initial_primary_weapon_absolute_index() != trait->get_initial_primary_weapon_absolute_index())
	{
		player->traits.get_weapons_traits_writeable()->set_initial_primary_weapon_absolute_index(trait->get_initial_primary_weapon_absolute_index(), false);
		player_update = true;
	}
	if (player->traits.get_weapons_traits()->get_initial_secondary_weapon_absolute_index() != trait->get_initial_secondary_weapon_absolute_index())
	{
		player->traits.get_weapons_traits_writeable()->set_initial_secondary_weapon_absolute_index(trait->get_initial_secondary_weapon_absolute_index(), false);
		player_update = true;
	}
	if (player->traits.get_weapons_traits()->get_initial_grenade_count_setting() != trait->get_initial_grenade_count_setting())
	{
		player->traits.get_weapons_traits_writeable()->set_initial_grenade_count_setting(trait->get_initial_grenade_count_setting(), false);
		player_update = true;
	}
	if (player->traits.get_weapons_traits()->get_recharging_grenades_setting() != trait->get_recharging_grenades_setting())
	{
		player->traits.get_weapons_traits_writeable()->set_recharging_grenades_setting(trait->get_recharging_grenades_setting(), false);
		player_update = true;
	}
	if (player->traits.get_weapons_traits()->get_infinite_ammo_setting() != trait->get_infinite_ammo_setting())
	{
		player->traits.get_weapons_traits_writeable()->set_infinite_ammo_setting(trait->get_infinite_ammo_setting(), false);
		player_update = true;
	}
	if (player->traits.get_weapons_traits()->get_weapon_pickup_allowed() != trait->get_weapon_pickup_allowed())
	{
		player->traits.get_weapons_traits_writeable()->set_weapon_pickup_allowed(trait->get_weapon_pickup_allowed(), false);
		player_update = true;
	}
	if (player_update)
	{
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_weapon_traits);
	}
}