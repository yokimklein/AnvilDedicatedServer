#pragma once
#include "..\cseries\cseries.h"
#include "..\game\players.h"
#include "data.h"
#include "..\game\game_globals.h"
#include "..\simulation\simulation_gamestate_entities.h"
#include "..\game\player_mapping.h"
#include "..\game\game_time.h"

struct s_thread_local_storage
{
	byte* __unknown0;
	byte* __unknown4; // actor
	c_smart_data_array<s_player_datum>* players; // players
	c_smart_data_array<s_object_header>* object_headers; // object
	s_game_engine_globals* game_engine_globals;
	byte* __unknown14; // prop_ref
	byte* hs_thread_deterministic_data;
	byte* __unknown1C;
	struct global_preferences* global_preferences;
	game_globals_storage* game_globals;
	byte* __unknown28; // squad
	struct s_scripted_camera_globals* scripted_camera_globals;
	struct players_global_data* players_globals;
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
	s_game_time_globals* game_time_globals;
	byte* __unknown68;
	byte* __unknown6C;
	struct s_data_array* object_looping_sounds_data;
	byte* __unknown74;
	byte* __unknown78;
	byte* __unknown7C; // effect event
	byte* __unknown80;
	s_player_mapping* player_mapping_globals;
	byte* __unknown88; // lights
	byte* __unknown8C;
	byte* __unknown90;
	byte* __unknown94;
	byte* __unknown98; // recycling_group
	byte* __unknown9C; // effect location
	byte* __unknownA0;
	byte* __unknownA4;
	byte* __unknownA8; // vocalization records
	byte* __unknownAC;
	byte* __unknownB0;
	struct s_game_sound_globals* game_sound_globals;
	byte* __unknownB8;
	byte* __unknownBC; // impacts
	byte* director_globals;
	byte* __unknownC4; // cached object render states
	byte* __unknownC8; // device groups
	byte* __unknownCC;
	byte* __unknownD0;
	byte* __unknownD4;
	byte* __unknownD8; // swarm
	byte* __unknownDC; // g_object_scripting_state?
	byte* __unknownE0; // flocks
	byte* effect_counts;
	byte* __unknownE8;
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
	c_smart_data_array<simulation_gamestate_entity_datum>* simulation_gamestate_entities; // sim. gamestate entities
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
	byte* __unknown178;
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
	byte* __unknown1C4;
	byte* __unknown1C8; // c_contrail_gpu::s_row
	byte* __unknown1CC;
	byte* __unknown1D0;
	byte* __unknown1D4; // c_beam_gpu::s_row
	byte* __unknown1D8; // c_light_volume_gpu::s_row
	byte* __unknown1DC; // contrail
	byte* __unknown1E0; // light_volume
	byte* __unknown1E4; // beam
	byte* __unknown1E8;
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
	byte* __unknown2CC;
	byte* __unknown2D0; // cluster light reference
	byte* __unknown2D4; // light cluster reference
	byte* __unknown2D8;
	byte* __unknown2DC;
	byte* __unknown2E0;
	byte* __unknown2E4;
	byte* __unknown2E8;
	byte* __unknown2EC;
	byte* __unknown2F0;
	byte* __unknown2F4;
	byte* __unknown2F8;
	byte* __unknown2FC;
	byte* __unknown300;
	byte* __unknown304;
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

s_thread_local_storage* get_tls();