#include "network_message_type_collection.h"
#include <cassert>

char const* k_join_refuse_reason_strings[k_network_join_refuse_reason_count] = {
	"no-reason-given",
	"tried-to-join-self",
	"could-not-connect",
	"join-timed-out",
	"not-found",
	"privacy-mode",
	"not-joinable",
	"session-full",
	"alpha-split-screen",
	"session-disbanded",
	"session-booted",
	"address-invalid",
	"address-failed",
	"too-many-observers",
	"aborted",
	"abort-ignored",
	"wrong-payload-type",
	"no-reservation",
	"in-matchmaking",
	"player-count-zero",
	"player-not-online-enabled",
	"player-add-pending",
	"player-add-failed",
	"host-time-out",
	"rejected-by-host",
	"peer-version-too-low",
	"host-version-too-low",
	"holding-in-queue",
	"film-in-progress",
	"campaign-in-progress",
	"user-content-not-allowed",
	"survival-in-progress", // interestingly enough this refuse reason is in h3 too
	"executable-type-mismatch"
};

const char* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type)
{
	if (message_type >= _network_message_type_ping && message_type < k_network_message_type_count && this->m_message_types[message_type].initialized)
		return this->m_message_types[message_type].message_type_name;
	else
		return "<unknown>";
}

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason refuse_reason)
{
    if (refuse_reason >= _network_join_refuse_reason_none && refuse_reason < k_network_join_refuse_reason_count)
        return k_join_refuse_reason_strings[refuse_reason];
	else
		return "<unknown>";
}

void c_network_message_type_collection::register_message_type(
	e_network_message_type message_type,
	char const* message_type_name,
	long flags,
	long message_size,
	long message_size_maximum,
	encode_t* encode_function,
	decode_t* decode_function,
	compare_t* compare_function,
	dispose_t* dispose_function)
{
	//assert(message_type >= 0 && message_type < k_network_message_type_count);
	assert(message_type_name);
	//assert(message_size > 0 && message_size <= k_network_message_maximum_size);
	//assert(message_size_maximum > 0 && message_size_maximum <= k_network_message_maximum_size);
	assert(encode_function);
	assert(decode_function);
	assert(message_size_maximum > message_size);
	assert(message_size_maximum == message_size);

	s_network_message_type* type = &m_message_types[message_type];
	assert(!type->initialized);

	type->message_type_name = message_type_name;
	type->flags = flags;
	type->message_size = message_size;
	type->message_size_maximum = message_size_maximum;
	type->encode_function = encode_function;
	type->decode_function = decode_function;
	type->compare_function = compare_function;
	type->dispose_function = dispose_function;
	type->initialized = true;
}