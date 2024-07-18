#pragma once
#include <cseries\cseries.h>

enum e_simulation_event_type;
class c_replication_outgoing_event;
struct s_simulation_view_telemetry_data;
class c_bitstream;
class c_simulation_event_definition
{
public:
	virtual e_simulation_event_type __cdecl event_type() = 0;
	virtual char const* __cdecl event_type_name() = 0;
	virtual long __cdecl payload_size() = 0;
	virtual long __cdecl number_of_entity_references() = 0;
	virtual bool __cdecl reference_delays_entity_deletion() = 0;
	virtual bool __cdecl event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) = 0;
	virtual long __cdecl minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*) = 0;
	virtual real __cdecl calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) = 0;
	virtual void __cdecl write_description_to_string(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real, long, char*) = 0;
	virtual long* __cdecl maximum_required_bits(long* out_bits, c_replication_outgoing_event*, long) = 0; // unsure of the name, pc only?
	virtual void __cdecl event_payload_encode(long, void const*, c_bitstream*) = 0;
	virtual bool __cdecl event_payload_decode(long, void*, c_bitstream*) = 0;
	virtual void __cdecl prepare_event_data_for_gameworld(long, void*) = 0;
	virtual bool __cdecl apply_game_event(long, long const*, long, void const*) = 0;
};