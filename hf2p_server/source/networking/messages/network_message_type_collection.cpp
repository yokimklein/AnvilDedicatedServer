#include "network_message_type_collection.h"

const char* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type)
{
    const char* message_name = "<unknown>";
    if (message_type >= _network_message_type_ping && message_type < k_network_message_type_count && this->message_types[message_type].initialized)
        message_name = this->message_types[message_type].message_type_name;
    return message_name;
}

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason refuse_reason)
{
    const char* refuse_string = "<unknown>";
    if (refuse_reason >= _network_join_refuse_reason_none && refuse_reason < k_network_join_refuse_reason_count)
        refuse_string = k_join_refuse_reason_strings[refuse_reason];
    return refuse_string;
}

s_network_message_membership_update::s_network_message_membership_update()
{
	for (size_t j = 0; j < k_network_maximum_machines_per_session; j++)
		peers[j].peer_properties_update.flags = 0;
	for (size_t j = 0; j < k_network_maximum_players_per_session; j++)
	{
		players[j].player_data.client = *new s_player_configuration_from_client();
		players[j].player_data.host.player_appearance.unknown = *new s_player_appearance_model_customization();
		players[j].player_data.host = *new s_player_configuration_from_host();
		//memset(&players[j].player_data.client, 0, sizeof(s_player_configuration_from_client));
		//memset(&players[j].player_data.host.player_appearance.unknown, 0, sizeof(s_player_appearance_model_customization));
		//memset(&players[j].player_data.host, 0, sizeof(s_player_configuration_from_host));
		players[j].player_data.host.player_assigned_team = -1;
		players[j].player_data.host.player_team = -1;
	}
};