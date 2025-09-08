#pragma once
#include "cseries\cseries.h"

#define REMOTE_COMMAND_HEADER_TAG "RCN"
#define REMOTE_COMMAND_PACKET_VERSION 1

#define MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE 0x400
#define MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE 0x10000

enum e_remote_command
{
	_remote_command_map_reset = 0,
	_remote_command_map_synch,
	_remote_command_camera,
	_remote_command_add_object,
	_remote_command_modify_object,
	_remote_command_delete_object,
	_remote_command_sound_command,
	_remote_command_hs_expression,
	_remote_command_tag_placement,
	_remote_command_flag_placement,

	NUMBER_OF_REMOTE_COMMANDS
};

struct transport_endpoint;
//struct s_remote_command_globals
//{
//	transport_endpoint* send_endpoint;
//	transport_endpoint* receive_endpoint;
//	transport_endpoint* listen_endpoint;
//
//	long reception_header_size;
//	char reception_header_buffer[32];
//
//	long reception_packet_total_size;
//	long reception_packet_payload_size;
//	long reception_packet_received;
//	char reception_packet_buffer[MAXIMUM_ENCODED_REMOTE_COMMAND_PACKET_SIZE + MAXIMUM_REMOTE_COMMAND_PAYLOAD_SIZE];
//
//	long last_camera_sync_milliseconds;
//	s_observer_result last_camera_synch_state;
//
//	//long light_volume_index;
//	//dword light_volume_send_time;
//	//scenario_light_block light_volume; // scenario->light_volumes[light_volume_index];
//
//	// long connections?
//	bool connected;
//};
//static_assert(sizeof(s_remote_command_globals) == 0x104B4);

//-----------------------------------------------------------------------------

#define k_maximum_number_of_tokens 100
#define k_token_length 256

#define COMMAND_CALLBACK_DECLARE(_command) callback_result_t _command##_callback(const void* userdata, long token_count, tokens_t const tokens)
#define COMMAND_CALLBACK_REGISTER(_command, _parameter_count, _parameters, ...) { #_command, _command##_callback, _parameter_count, _parameters, __VA_ARGS__ }

#define COMMAND_CALLBACK_PARAMETER_CHECK                                      \
ASSERT(userdata != nullptr);                                                  \
ASSERT((token_count - 1) >= 0);                                               \
                                                                              \
const s_command& command = *static_cast<const s_command*>(userdata);          \
callback_result_t result = command.name;                                      \
result.append_line(": succeeded");                                            \
if ((token_count - 1) != command.parameter_count)                             \
{                                                                             \
    result = "Invalid usage. ";                                               \
    result.append_print_line("%s %s", command.name, command.parameter_types); \
    result.append(command.extra_info);                                        \
    return result;                                                            \
}

//-----------------------------------------------------------------------------


using _token_t = c_static_string<k_token_length>;
using token_t = _token_t*;
using tokens_t = c_static_array<token_t, k_maximum_number_of_tokens>;

using callback_result_t = c_static_string<4096>;
using callback_t = callback_result_t(const void*, long, tokens_t const);

struct s_command
{
	const char* name;
	callback_t* callback;
	long parameter_count;
	const char* parameter_types;
	const char* extra_info;
};

//-----------------------------------------------------------------------------

COMMAND_CALLBACK_DECLARE(help);
COMMAND_CALLBACK_DECLARE(anvil_launch_scenario);

//-----------------------------------------------------------------------------

s_command const k_registered_commands[] =
{
	COMMAND_CALLBACK_REGISTER(help, 0, "", "prints this output.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
	COMMAND_CALLBACK_REGISTER(anvil_launch_scenario, 2, "<string> <string>", "launches into a scenario.\r\nNETWORK SAFE: Unknown, assumed unsafe"),
};

void command_tokenize(const char* input, tokens_t& tokens, long* token_count);