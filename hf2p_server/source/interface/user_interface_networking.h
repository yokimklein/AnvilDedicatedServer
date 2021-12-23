#pragma once

enum e_desired_multiplayer_mode
{
	_desired_multiplayer_mode_none = -1,
	_desired_multiplayer_mode_campaign,
	_desired_multiplayer_mode_custom_games,
	_desired_multiplayer_mode_forge,
	_desired_multiplayer_mode_theater,
	_desired_multiplayer_mode_survival,

	k_number_of_desired_multiplayer_modes
};

// non-original names, may be incorrect
enum e_gui_network_session_advertisement_mode
{
	_network_advertise_xbox_live_anyone,
	_network_advertise_xbox_live_friends_only,
	_network_advertise_xbox_live_invite_only,
	_network_advertise_system_link,
	_network_advertise_offline,

	k_network_advertise_count
};