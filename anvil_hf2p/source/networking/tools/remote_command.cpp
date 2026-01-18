#include "remote_command.h"
#include "anvil\server_tools.h"
#include "text\unicode.h"
#include "anvil\session_control.h"
#include "anvil\session_voting.h"
#include <string>

void command_tokenize(const char* input, tokens_t& tokens, long* token_count)
{
	bool in_quotes = false;
	long num_chars = strlen(input);
	char current_token[k_token_length] = { 0 };

	for (int i = 0; i < num_chars; i++)
	{
		char c = input[i];

		if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == 0)
		{
			if (in_quotes)
			{
				current_token[strlen(current_token)] = c;
			}
			else if (strlen(current_token) > 0)
			{
				token_t& token = tokens[*token_count] = new _token_t();
				token->set(current_token);
				(*token_count)++;
				csmemset(current_token, 0, k_token_length);
			}
		}
		else if (c == '"')
		{
			if (in_quotes)
			{
				in_quotes = false;
			}
			else
			{
				in_quotes = true;
			}
		}
		else
		{
			current_token[strlen(current_token)] = c;
		}
	}

	if (strlen(current_token) > 0)
	{
		token_t& token = tokens[*token_count] = new _token_t();
		token->set(current_token);
		(*token_count)++;
	}
}

callback_result_t help_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	ASSERT(token_count >= 1);

	static callback_result_t result;
	if (result.is_empty())
	{
		for (long i = 0; i < NUMBEROF(k_registered_commands); i++)
		{
			const s_command& command = k_registered_commands[i];

			result.append_print("%s ", command.name);
			result.append_line(command.parameter_types && *command.parameter_types != 0 ? command.parameter_types : "");
			result.append_line(command.extra_info && *command.extra_info != 0 ? command.extra_info : "");
			result.append_line();
		}
	}

	return result;
}

callback_result_t anvil_launch_scenario_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	const char* scenario_path = tokens[1]->get_string();
	const char* map_name = tokens[2]->get_string();
	c_static_wchar_string<16> map_name_wide;
	map_name_wide.print(L"%hs", map_name);

	anvil_launch_scenario(scenario_path, map_name_wide.get_string());

	return result;
}

callback_result_t anvil_session_set_map_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	e_map_id map_id = (e_map_id)atol(tokens[1]->get_string());

	anvil_session_set_map(map_id);

	return result;
}

callback_result_t anvil_session_set_gamemode_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	e_game_engine_type engine_index = (e_game_engine_type)atol(tokens[1]->get_string());
	long variant_index = (long)atol(tokens[1]->get_string());
	ulong time_limit = (ulong)atol(tokens[1]->get_string());

	anvil_session_set_gamemode(engine_index, variant_index, time_limit);

	return result;
}

callback_result_t anvil_session_start_countdown_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	anvil_session_start_countdown();

	return result;
}

callback_result_t anvil_session_launch_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	anvil_session_launch();

	return result;
}

callback_result_t anvil_boot_peer_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	long peer_index = (long)atol(tokens[1]->get_string());

	anvil_boot_peer(peer_index);

	return result;
}

callback_result_t anvil_session_begin_vote_callback(const void* userdata, long token_count, tokens_t const tokens)
{
	COMMAND_CALLBACK_PARAMETER_CHECK;

	anvil_session_begin_vote();

	return result;
}