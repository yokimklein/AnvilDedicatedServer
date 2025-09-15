#pragma once
#include <cseries\cseries.h>
#include <game\players.h>
#include <memory\data.h>
#include <game\game_globals.h>
#include <game\game_engine.h>
#include <simulation\simulation_gamestate_entities.h>
#include <game\player_mapping.h>
#include <game\game_time.h>
#include <game\survival_mode.h>
#include <ai\actors.h>
#include <camera\director_globals.h>
#include <cache\restricted_memory_regions.h>
#include <multithreading\threads.h>

template<long index, void(__cdecl* tls_pre_overwrite_fixup_callback)(void*) = nullptr, void(__cdecl* tls_post_copy_fixup_callback)(void*) = nullptr, void(__cdecl* tls_update_callback)(void*) = nullptr>
struct t_restricted_allocation_manager : public c_allocation_base
{
public:
	t_restricted_allocation_manager()
	{
		m_member_index = NONE;
		m_thread_id = NONE;
	}
	~t_restricted_allocation_manager()
	{

	}

	void* allocate(ulong allocation, const char* name) override
	{
		// $TODO:
		return NULL;
	}

	void deallocate(void* buffer) override
	{
		// $TODO:
		return;
	}

	void* reserve_memory(const char* name, const char* type, unsigned int allocation, long alignment_bits)
	{
		ASSERT(!valid());
		m_member_index = restricted_region_add_member(index, name, type, allocation, alignment_bits, tls_update_callback, tls_pre_overwrite_fixup_callback, tls_post_copy_fixup_callback);
		m_thread_id = get_current_thread_index();
		return restricted_region_get_member_address(index, m_member_index);
	}

	bool valid() const
	{
		return m_member_index != NONE && m_thread_id != NONE;
	}

protected:
	long m_member_index;
	long m_thread_id;
};
static_assert(sizeof(t_restricted_allocation_manager<NONE>) == 0xC);

struct s_thread_local_storage
{
	byte* __unknown0;
	c_smart_data_array<actor_datum> actor_data;
	c_smart_data_array<player_datum> players;
	c_smart_data_array<object_header_datum> object_headers; // object
	s_game_engine_globals* game_engine_globals;
	byte* __unknown14; // prop_ref
	byte* hs_thread_deterministic_data;
	byte* __unknown1C;
	struct global_preferences* global_preferences;
	game_globals_storage* game_globals;
	byte* __unknown28; // squad
	struct s_scripted_camera_globals* scripted_camera_globals;
	players_global_data* players_globals;
	byte* __unknown34; // effect (survival_mode_globals?)
	byte* __unknown38; // prop
	byte* __unknown3C; // command scripts
	byte* __unknown40; // clump
	byte* __unknown44;
	byte* __unknown48;
	struct s_player_control_globals* player_control_globals;
	byte* __unknown50; // s_object_globals* object_globals?
	byte* campaign_metagame_globals;
	byte* breakable_surface_globals;
	byte* timing_samples_global;
	byte* __unknown60; // joint state
	game_time_globals_definition* game_time_globals;
	byte* __unknown68;
	byte* __unknown6C;
	struct s_data_array* object_looping_sounds_data;
	byte* __unknown74;
	byte* __unknown78;
	byte* __unknown7C; // effect event
	byte* __unknown80;
	s_player_mapping_globals* player_mapping_globals;
	byte* __unknown88; // lights
	byte* __unknown8C;
	byte* __unknown90;
	s_survival_mode_globals* survival_mode_globals;
	byte* __unknown98; // recycling_group
	byte* __unknown9C; // effect location
	byte* g_cortana_globals;
	byte* __unknownA4; // rasterizer game states?
	byte* __unknownA8; // vocalization records
	struct s_font_cache_globals* g_font_cache_globals;
	byte* __unknownB0;
	struct s_game_sound_globals* game_sound_globals;
	byte* __unknownB8;
	byte* __unknownBC; // impacts
	s_director_globals* director_globals;
	byte* __unknownC4; // cached object render states
	byte* __unknownC8; // device groups
	byte* __unknownCC;
	byte* __unknownD0;
	byte* __unknownD4;
	byte* __unknownD8; // swarm
	byte* __unknownDC; // g_object_scripting_state?
	byte* __unknownE0; // flocks
	byte* effect_counts;
	byte* hs_runtime_globals;
	byte* __unknownEC; // objectives
	byte* __unknownF0;
	byte* __unknownF4; // squad group
	byte* __unknownF8; // cloth
	byte* __unknownFC;
	byte* __unknown100; // particle_system
	byte* __unknown104;
	struct s_data_array* breakable_surface_set_broken_events_data;
	byte* __unknown10C;
	byte* __unknown110; // tracking
	byte* __unknown114; // widget
	byte* __unknown118;
	byte* __unknown11C;
	byte* __unknown120;
	byte* __unknown124; // squad_patrol
	byte* __unknown128; // leaf system
	byte* __unknown12C;
	c_smart_data_array<simulation_gamestate_entity_datum> simulation_gamestate_entities; // sim. gamestate entities
	byte* __unknown134; // dynamic firing points
	byte* __unknown138; // particle_emitter
	byte* __unknown13C; // particle_location
	byte* __unknown140; // contrail_system
	byte* __unknown144; // light_volume_system
	byte* __unknown148; // beam_system
	byte* local_game_engine_globals;
	byte* __unknown150; // loopobjects
	byte* __unknown154; // havok proxies
	byte* __unknown158;
	byte* __unknown15C;
	byte* __unknown160; // decal_system
	byte* __unknown164;
	struct s_data_array* effect_geometry_sample_data;
	byte* __unknown16C;
	byte* __unknown170; // c_particle_emitter_gpu::s_row
	byte* __unknown174;
	byte* g_water_interaction_events;
	byte* __unknown17C;
	byte* __unknown180; // object list header 
	byte* __unknown184; // screen_effect
	byte* __unknown188; // hs globals
	byte* game_sound_scripted_impulses_globals;
	byte* __unknown190;
	byte* __unknown194; // formations
	byte* __unknown198; // c_particle_emitter_gpu
	byte* __unknown19C; // particles
	byte* __unknown1A0; // antenna
	byte* __unknown1A4; // muffin
	byte* __unknown1A8; // c_beam_gpu
	byte* __unknown1AC; // c_light_volume_gpu
	byte* __unknown1B0; // contrail
	byte* __unknown1B4; // light_volume
	byte* __unknown1B8; // beam
	byte* __unknown1BC; // impact_arrays
	byte* __unknown1C0;
	byte* g_rasterizer_implicit_geometry_globals;
	byte* __unknown1C8; // c_contrail_gpu::s_row
	byte* hue_saturation_control;
	byte* __unknown1D0;
	byte* __unknown1D4; // c_beam_gpu::s_row
	byte* __unknown1D8; // c_light_volume_gpu::s_row
	byte* __unknown1DC; // contrail
	byte* __unknown1E0; // light_volume
	byte* __unknown1E4; // beam
	bool g_file_errors_suppressed;
	byte* __unknown1EC; // actor firing-position owner
	byte* __unknown1F0;
	byte* __unknown1F4; // ragdolls
	byte* __unknown1F8;
	byte* __unknown1FC; // object activation regions
	byte* __unknown200;
	byte* __unknown204; // c_contrail_gpu
	byte* __unknown208; // recorded animations
	byte* __unknown20C;
	byte* __unknown210;
	byte* __unknown214; // contrail_profile
	byte* __unknown218; // decal
	byte* effect_messaging_queue;
	byte* effect_lightprobes;
	byte* player_control_globals_deterministic;
	byte* __unknown228;
	byte* __unknown22C; // list object reference
	byte* __unknown230;
	byte* __unknown234;
	byte* __unknown238;
	byte* g_object_message_queue;
	byte* __unknown240;
	byte* __unknown244;
	byte* __unknown248;
	byte* __unknown24C;
	byte* __unknown250;
	byte* __unknown254;
	byte* __unknown258; // hs dist. globals
	byte* __unknown25C; // tracking hs thread
	byte* __unknown260; // cluster collideable object refe
	byte* __unknown264; // collideable object cluster refe
	byte* __unknown268;
	byte* __unknown26C; // chud widgets
	byte* __unknown270; // chud widgets
	byte* __unknown274; // chud widgets
	byte* __unknown278; // chud widgets
	byte* __unknown27C;
	byte* __unknown280;
	byte* __unknown284;
	byte* __unknown288;
	byte* __unknown28C;
	byte* __unknown290;
	byte* __unknown294;
	byte* __unknown298;
	byte* __unknown29C; // non-det hs thread
	byte* random_math_globals;
	byte* __unknown2A4; // cluster noncollideable object r
	byte* __unknown2A8; // noncollideable object cluster r
	byte* __unknown2AC;
	byte* __unknown2B0;
	byte* __unknown2B4;
	byte* __unknown2B8;
	byte* __unknown2BC;
	byte* __unknown2C0;
	byte* __unknown2C4;
	byte* __unknown2C8;
	long g_registered_thread_index;
	byte* __unknown2D0; // cluster light reference
	byte* __unknown2D4; // light cluster reference
	byte* __unknown2D8;
	byte* __unknown2DC;
	byte* __unknown2E0;
	byte* __unknown2E4;
	byte* __unknown2E8;
	byte* __unknown2EC;
	byte* __unknown2F0;
	void* g_restricted_address[k_total_restricted_memory_regions];
	byte* __unknown308;
	byte* __unknown30C;
	byte* __unknown310;
	byte* __unknown314;
	byte* __unknown318;
	byte* __unknown31C;
	byte* __unknown320;
	byte* __unknown324;
	byte* __unknown328;
	byte* __unknown32C;
	byte* __unknown330;
	byte* __unknown334;
	byte* __unknown338;
	byte* __unknown33C;
	byte* __unknown340;
	byte* __unknown344;
	byte* __unknown348;
	byte* __unknown34C;
};
static_assert(sizeof(s_thread_local_storage) == 0x350);
static_assert(0x30 == OFFSETOF(s_thread_local_storage, players_globals));

#define TLS_DATA_GET_VALUE_REFERENCE(NAME) decltype(get_tls()->NAME)& NAME = get_tls()->NAME

s_thread_local_storage* get_tls();