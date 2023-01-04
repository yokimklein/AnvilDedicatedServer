#pragma once
#include "..\saved_games\saved_game_files.h"
#include "..\dllmain.h"

// uncommented maps are included in cache 11.1.601838
enum e_map_id : long
{
//             this is the birthday of one of the halo 3 devs' wife
//                         23rd October 1971
	           _mainmenu = 0x10231971, // Mainmenu,
	           _zanzibar = 30,  // Last Resort
	           _s3d_turf = 31,  // Icebox
//            _construct = 300, // Construct
//             _dlc_5050 = 301, // 
//              _dlc_new = 302, // 
//            _dlc_reuse = 303, // 
	           _deadlock = 310, // High Ground
	           _guardian = 320, // Guardian
//            _isolation = 330, // Isolation
             _riverworld = 340, // Valhalla
//            _salvation = 350, // Epitaph
//            _snowbound = 360, // Snowbound
//          _warthog_inc = 370, // Warthog Inc
                  _chill = 380, // Narrows
              _cyberdyne = 390, // The Pit
//               _shrine = 400, // Sandtrap
            _bunkerworld = 410, // Standoff
//            _landslide = 420, // Landslide
//            _beachhead = 430, // Beachhead
//                _docks = 440, // Longshore
//            _pump_haus = 450, // Pump Haus
//                _shaft = 460, // Shaft
//           _sidewinder = 470, // Avalanche
//            _warehouse = 480, // Foundry
//              _descent = 490, // Assembly
//            _spacecamp = 500, // Orbital
//              _volcano = 510, // Volcano
//              _lockout = 520, // Blackout
//               _armory = 580, // Rat's Nest
//            _ghosttown = 590, // Ghost Town
//             _chillout = 600, // Cold Storage
            _s3d_reactor = 700, // Reactor
//                       = 701, // 
//                       = 702, // 
               _s3d_edge = 703, // Edge
//                       = 704, // 
          _s3d_avalanche = 705, // Diamondback
//        _s3d_waterfall = 706, // Whiteout
//                       = 707, // 
//                       = 708, // 
//    _s3d_sky_bridgenew = 709, // Sky Bridge New
//                 _damn = 710, // Dam
//          _s3d_lockout = 711, // Lockup
//                       = 712, // 
//                       = 713, // 
//                       = 714, // 
//       _s3d_powerhouse = 715, // Powerhouse
//                       = 716, // 
//                       = 717, // 
//                       = 718, // 
//                       = 719, // 
//              _midship = 720, // Heretic
//              _sandbox = 730, // Sandbox
//             _fortress = 740, // Citadel

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
s3d_tutorial // Tutorial
*/
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
	float boundry_width;
	float boundry_length;
	float boundry_top;
	float boundry_bottom;
};

struct s_map_variant_placement
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

struct s_map_variant_budget_entry
{
	long tag_index;
	byte runtime_minimum;
	byte runtime_maximum;
	byte runtime_current;
	byte design_time_maximum;
	real cost;
};

class c_map_variant
{
public:
	c_map_variant();
	void create_default(e_map_id map_id);

	s_saved_game_item_metadata base;
	short : 16;
	ushort total_placements_count;
	ushort used_placements_count;
	ushort budget_entry_count;
	long map_id;
	real_bounds world_x_bounds;
	real_bounds world_y_bounds;
	real_bounds world_z_bounds;
	long content_type;
	real maximum_budget;
	real current_budget;
	long : 32;
	long : 32;
	s_map_variant_placement placements[640];
	ushort scenario_indices[16];
	s_map_variant_budget_entry budget_entries[256];
	char unused[320];
};
static_assert(sizeof(c_map_variant) == 0xE090); // is this actually 0xE108 now?