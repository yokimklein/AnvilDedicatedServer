#include "network_message_type_collection.h"

// TODO - test logic
const char* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type)
{
    const char* message_name = "<unknown>";
    if (message_type >= _network_message_type_ping && message_type < k_network_message_type_count && this->message_types[message_type].__unknown0)
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