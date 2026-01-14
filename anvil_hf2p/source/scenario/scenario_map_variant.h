#pragma once
#include <saved_games\saved_game_files.h>

constexpr long k_number_of_map_variant_simulation_entities = 80;

enum e_campaign_id
{
	_campaign_id_default = 1,
	_campaign_id_none = NONE,
};

// uncommented maps are included in cache 11.1.601838
enum e_map_id
{
				 _map_id_none = -1,
	_map_id_use_scenario_path = -2,
					_mainmenu = 0x10231971, // Mainmenu (this is the birthday of a halo 3 developer's wife, 23rd October 1971)
					_zanzibar = 30,  // Last Resort
					_s3d_turf = 31,  // Icebox
//		           _construct = 300, // Construct
//		            _dlc_5050 = 301, // 
//		             _dlc_new = 302, // 
//		           _dlc_reuse = 303, // 
		            _deadlock = 310, // High Ground
		            _guardian = 320, // Guardian
//		           _isolation = 330, // Isolation
		          _riverworld = 340, // Valhalla
//		           _salvation = 350, // Epitaph
//		           _snowbound = 360, // Snowbound
//		         _warthog_inc = 370, // Warthog Inc
		               _chill = 380, // Narrows
		           _cyberdyne = 390, // The Pit
//		              _shrine = 400, // Sandtrap
		         _bunkerworld = 410, // Standoff
//		           _landslide = 420, // Landslide
//		           _beachhead = 430, // Beachhead
//		               _docks = 440, // Longshore
//		           _pump_haus = 450, // Pump Haus
//		               _shaft = 460, // Shaft
//		          _sidewinder = 470, // Avalanche
//		           _warehouse = 480, // Foundry
//		             _descent = 490, // Assembly
//		           _spacecamp = 500, // Orbital
//		             _volcano = 510, // Volcano
//		             _lockout = 520, // Blackout
//		                      = 530, // 
//		                      = 540, // 
//		                      = 550, // 
//		                      = 560, // 
//		                      = 570, // 
//		              _armory = 580, // Rat's Nest
//		           _ghosttown = 590, // Ghost Town
//		            _chillout = 600, // Cold Storage
		         _s3d_reactor = 700, // Reactor
//		                      = 701, // 
//		                      = 702, // 
		            _s3d_edge = 703, // Edge
//		                      = 704, // 
		       _s3d_avalanche = 705, // Diamondback
//		       _s3d_waterfall = 706, // Whiteout
//		                      = 707, // 
//		                      = 708, // 
//		   _s3d_sky_bridgenew = 709, // Sky Bridge New
//		                _damn = 710, // Dam
//		         _s3d_lockout = 711, // Lockup
//		                      = 712, // 
//		                      = 713, // 
//		                      = 714, // 
//		      _s3d_powerhouse = 715, // Powerhouse
//		                      = 716, // 
//		                      = 717, // 
//		                      = 718, // 
//		                      = 719, // 
//		             _midship = 720, // Heretic
//		             _sandbox = 730, // Sandbox
//		            _fortress = 740, // Citadel
//		        _s3d_tutorial = 877, // Tutorial

/*
// unknown map ids
the_gash
s3d_dale
s3d_hangar
s3d_rock
s3d_sidewinder
s3d_underwater
s3d_well
s3d_countdown
s3d_haven
s3d_drydock // Dry Dock
s3d_burial_mounds
s3d_chillout // Abeyance
s3d_beaver_creek
*/
	k_map_id_count
};

struct s_map_variant_placement_properties
{
	word_flags game_engine_flags;
	byte_flags object_flags;
	char team_affiliation;
	byte shared_storage;
	byte respawn_time;
	char object_type;
	char boundry_shape;
	real boundry_width;
	real boundry_length;
	real boundry_top;
	real boundry_bottom;
};

struct s_variant_object_datum
{
	word_flags flags;
	short : 16;
	long object_index;
	long editor_object_index;
	long budget_index;
	real_point3d position;
	real_vector3d right;
	real_vector3d up;
	c_object_identifier parent_object_identifier;
	s_map_variant_placement_properties properties;
};
static_assert(sizeof(s_variant_object_datum) == 0x54);

struct s_variant_quota
{
	long tag_index;
	byte runtime_minimum;
	byte runtime_maximum;
	byte runtime_current;
	byte design_time_maximum;
	real cost;
};
static_assert(sizeof(s_variant_quota) == 0xC);

class c_map_variant
{
public:
	c_map_variant();
	void create_default(e_map_id map_id);
	datum_index get_chunk_gamestate_index(long chunk_index);
	void place_new_object(long map_variant_index, long unknown, bool do_not_force);

	s_content_item_metadata m_metadata;
	short version; // 12 in HO
	ushort scenario_object_count;
	ushort variant_object_count;
	ushort placeable_object_quota_count;
	long map_id;
	real_rectangle3d world_bounds;
	c_enum<e_content_item, long, _content_item_usermap, k_content_item_count> content_type;
	real maximum_budget;
	real current_budget;
	bool showing_helpers;
	bool built_in;
	long cache_checksum;
	c_static_array<s_variant_object_datum, 640> objects;
	c_static_array<short, 16> scenario_indices;
	c_static_array<s_variant_quota, 256> quotas;
	c_static_array<datum_index, k_number_of_map_variant_simulation_entities> simulation_gamestate_indices;
};
static_assert(sizeof(c_map_variant) == 0xE090);

c_map_variant* game_engine_get_runtime_map_variant();