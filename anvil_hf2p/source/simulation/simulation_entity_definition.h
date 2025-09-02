#pragma once
#include <memory\bitstream.h>
#include <simulation\game_interface\simulation_game_entities.h>

constexpr long k_simulation_entity_maximum_creation_data_size = 128;
constexpr long k_simulation_entity_maximum_state_data_size = 1024;

struct s_simulation_view_telemetry_data;
struct s_entity_update_data;
class c_simulation_entity_definition
{
public:
	virtual e_simulation_entity_type __thiscall entity_type();
	virtual char const* __thiscall entity_type_name();
	virtual long __thiscall state_data_size();
	virtual long __thiscall creation_data_size();
	virtual long __thiscall update_flag_count();
	virtual dword __thiscall initial_update_mask();
	virtual bool __thiscall entity_replication_required_for_view_activation(s_simulation_entity const*);
	virtual bool __thiscall entity_type_is_gameworld_object();
	virtual bool __thiscall gameworld_attachment_valid(long);
	virtual bool __thiscall entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	virtual void __thiscall creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*);
	virtual void __thiscall write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*);
	virtual void __thiscall update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*);
	virtual void __thiscall entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	virtual bool __thiscall entity_creation_decode(long, void*, c_bitstream*, bool);
	virtual bool __thiscall entity_update_encode(bool, dword, dword*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool);
	virtual bool __thiscall entity_update_decode(bool, dword*, long, void*, c_bitstream*, bool);
	virtual bool __thiscall entity_state_lossy_compare(void*, void*, long);
	virtual bool __thiscall entity_creation_lossy_compare(void*, void*, long);
	virtual void __thiscall build_creation_data(long, long, void*);
	virtual bool __thiscall build_baseline_state_data(long, void const*, long, void*);
	virtual bool __thiscall build_updated_state_data(s_simulation_entity const*, dword*, long, void*);
	virtual void __thiscall rotate_entity_indices(s_simulation_entity const*);
	virtual void __thiscall prepare_creation_data_for_gameworld(long, void*);
	virtual void __thiscall prepare_state_data_for_gameworld(dword, long, void*);
	virtual bool __thiscall create_game_entity(long, long, void const*, dword, long, void const*);
	virtual bool __thiscall update_game_entity(long, dword, long, void const*);
	virtual bool __thiscall delete_game_entity(long);
	virtual bool __thiscall promote_game_entity_to_authority(long);
	virtual void __thiscall write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*);


};