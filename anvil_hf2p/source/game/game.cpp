#include "game.h"
#include <game\game_globals.h>
#include <game\game_engine_util.h>
#include <game\game_engine_team.h>
#include <networking\network_configuration.h>
#include <memory\tls.h>

REFERENCE_DECLARE(0x104DE54, bool, g_disable_video);
REFERENCE_DECLARE(0x104DD9A, bool, g_disable_audio);

game_options* game_options_get()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	ASSERT(game_globals && (game_globals->initializing || game_globals->map_active));

	return &game_globals->options;
}

e_game_simulation_type game_simulation_get()
{
	return game_options_get()->game_simulation_type;
}

bool game_is_server()
{
	e_game_simulation_type game_simulation = game_simulation_get();
	if (game_simulation == _game_simulation_synchronous_server || game_simulation == _game_simulation_distributed_server)
	{
		return true;
	}

	return false;
}

bool game_is_distributed()
{
	e_game_simulation_type game_simulation = game_simulation_get();
	if (game_simulation >= _game_simulation_distributed_client && game_simulation <= _game_simulation_distributed_server)
	{
		return true;
	}

	return false;
}

bool game_is_playback()
{
	return game_playback_get();
}

e_game_playback_type game_playback_get()
{
	return game_options_get()->game_playback;
}

bool game_is_available()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	if (!game_globals)
	{
		return false;
	}
	if (game_globals->map_active)
	{
		return false;
	}
	if (game_globals->active_structure_bsp_mask == 0)
	{
		return false;
	}

	return true;
}

bool game_is_predicted()
{
	return game_simulation_get() == _game_simulation_distributed_client;
}

bool game_is_authoritative()
{
	return !game_is_predicted();
}

bool game_engine_has_teams()
{
	if (game_engine_running())
	{
		c_game_variant* game_variant = current_game_variant();
		if (game_engine_variant_has_teams(game_variant))
		{
			return true;
		}
	}
	return false;
}

bool game_in_progress()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	if (game_globals && game_globals->game_in_progress && !game_globals->initializing)
	{
		return game_globals->map_active;
	}
	else
	{
		return false;
	}
}

bool game_is_client()
{
#if defined(GAME_CLIENT_ENABLED)
	return true;
#else
	return false;
#endif
}

bool game_is_dedicated_server()
{
#if defined(DEDICATED_SERVER_ENABLED)
	return true;
#else
	return false;
#endif
}

bool game_is_bot_client()
{
#if defined(BOT_CLIENT_ENABLED)
	return true;
#else
	return false;
#endif
}

bool game_is_sapien()
{
	return false;
}

bool game_is_guerilla()
{
	return false;
}

bool game_is_tool()
{
	return false;
}

bool game_is_halo3()
{
	return false;
}

bool game_is_campaign()
{
	return game_options_get()->game_mode == _game_mode_campaign;
}

bool game_is_multiplayer()
{
	return game_options_get()->game_mode == _game_mode_multiplayer;
}

bool game_is_survival()
{
	if (!game_options_valid())
	{
		return false;
	}

	return game_options_get()->game_mode == _game_mode_campaign && game_options_get()->survival_enabled;
}

bool game_is_finished()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	ASSERT(game_globals && game_globals->map_active);
	return game_globals->game_finished;
}

bool game_is_multithreaded()
{
	return true;
}

bool game_has_nonlocal_players()
{
	if (!VALID_INDEX(game_simulation_get(), k_game_simulation_count))
	{
		return false;
	}

	long local_machine_index = players_get_local_machine_index();
	ulong machine_valid_mask;
	s_machine_identifier machine_identifiers[k_maximum_machines];
	players_get_machines(&machine_valid_mask, machine_identifiers);
	return local_machine_index != NONE && TEST_MASK(~FLAG(local_machine_index), machine_valid_mask);
}

bool game_is_authoritative_playback()
{
	if (!game_options_valid())
	{
		return false;
	}

	if (!game_options_get()->game_playback)
	{
		return false;
	}

	if (game_options_get()->game_playback == _game_playback_network_client)
	{
		return false;
	}

	return true;
}

e_campaign_difficulty_level game_difficulty_level_get()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	if (game_options_get()->game_mode == _game_mode_campaign)
	{
		return game_options_get()->campaign_difficulty;
	}
	else
	{
		return _campaign_difficulty_level_normal;
	}
}

void game_get_determinism_versions(long* determinism_version, long* determinism_compatible_version)
{
	if (determinism_version)
	{
		*determinism_version = get_network_configuration()->determinism_configuration.determinism_version;
	}
	if (determinism_compatible_version)
	{
		*determinism_compatible_version = get_network_configuration()->determinism_configuration.determinism_compatible_version;
	}
}

bool __fastcall game_engine_teams_use_one_shared_life(e_game_team team)
{
	return INVOKE(0xDC7F0, game_engine_teams_use_one_shared_life, team);
}

bool game_options_valid()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	return game_globals && (game_globals->initializing || game_globals->map_active);
}

bool game_is_ui_shell()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	return game_globals->options.game_mode == _game_mode_mainmenu;
}

bool game_disable_rendering()
{
	return false;
}

bool game_disable_sound()
{
	return false;
}
