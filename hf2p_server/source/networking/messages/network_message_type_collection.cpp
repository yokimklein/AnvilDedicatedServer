#include "network_message_type_collection.h"

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

const char* c_network_message_type_collection::get_message_type_name(e_network_message_type message_type) const
{
	if (message_type < 0 || message_type >= k_network_message_type_count || !m_message_types[message_type].initialized)
		return "<unknown>";
	return m_message_types[message_type].message_type_name;
}

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason refuse_reason)
{
	if (refuse_reason < _network_join_refuse_reason_none || refuse_reason >= k_network_join_refuse_reason_count)
		return "<unknown>";
	return k_join_refuse_reason_strings[refuse_reason];
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
	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);
	ASSERT(message_type_name);
	if (!TEST_BIT(flags, 1))
	{
		ASSERT(message_size > 0 && message_size <= k_network_message_maximum_size);
		ASSERT(message_size_maximum > 0 && message_size_maximum <= k_network_message_maximum_size);
		ASSERT(encode_function);
		ASSERT(decode_function);
	}
	if (TEST_BIT(flags, 0))
	{
		ASSERT(message_size_maximum > message_size);
	}
	else
	{
		ASSERT(message_size_maximum == message_size);
	}

	s_network_message_type* type = &m_message_types[message_type];
	ASSERT(!type->initialized);

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

void c_network_message_type_collection::encode_message_header(c_bitstream* stream, e_network_message_type message_type, long message_storage_size)
{
	DECLFUNC(0x387A0, void, __thiscall, c_network_message_type_collection*, c_bitstream*, e_network_message_type, long)(this, stream, message_type, message_storage_size);
}

bool c_network_message_type_collection::decode_message_header(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size) const
{
	ASSERT(packet);
	ASSERT(message_type);
	ASSERT(message_storage_size);

	return DECLFUNC(0x38830, bool, __thiscall, const c_network_message_type_collection*, c_bitstream*, e_network_message_type*, long*)(this, packet, message_type, message_storage_size);
}

bool __cdecl c_network_message_type_collection::decode_message(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size, void* message_storage) const
{
	ASSERT(packet);
	ASSERT(message_type);
	ASSERT(message_storage_size);
	ASSERT(message_storage);

	if (decode_message_header(packet, message_type, message_storage_size))
	{
		const s_network_message_type* type_definition = &m_message_types[*message_type];

		ASSERT(*message_type >= 0 && *message_type < k_network_message_type_count);
		ASSERT(type_definition->initialized);
		ASSERT(*message_storage_size >= 0 && *message_storage_size <= k_network_message_maximum_size);
		ASSERT(*message_storage_size >= type_definition->message_size);
		ASSERT(*message_storage_size <= type_definition->message_size_maximum);

		csmemset(message_storage, 0, *message_storage_size);

		if (TEST_BIT(type_definition->flags, 1))
		{
			return true;
		}

		ASSERT(type_definition->decode_function);
		return type_definition->decode_function(packet, *message_storage_size, message_storage);
	}

	return false;
}

void __cdecl c_network_message_type_collection::dispose_message(e_network_message_type message_type, long message_storage_size, void* message_storage) const
{
	const s_network_message_type* type_definition = &m_message_types[message_type];

	ASSERT(message_type >= 0 && message_type < k_network_message_type_count);
	ASSERT(message_storage_size);
	ASSERT(message_storage);

	if (type_definition->dispose_function)
	{
		type_definition->dispose_function(message_storage_size, message_storage);
	}
}