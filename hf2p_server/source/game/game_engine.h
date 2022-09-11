#pragma once
#include "..\cseries\cseries.h"

// uncommented maps are included in cache 11.1.601838
enum e_map_id : long
{
	//         this is the birthday of one of the halo 3 devs' wife
	//                 23rd October 1971
	           _mainmenu = 0x10231971, // Mainmenu,
	           _zanzibar = 30,  // Last Resort
			   _s3d_turf = 31,  // Icebox
	//        _construct = 300, // Construct
	//         _dlc_5050 = 301, // 
	//          _dlc_new = 302, // 
	//        _dlc_reuse = 303, // 
	           _deadlock = 310, // High Ground
			   _guardian = 320, // Guardian
	//        _isolation = 330, // Isolation
	         _riverworld = 340, // Valhalla
	//        _salvation = 350, // Epitaph
	//        _snowbound = 360, // Snowbound
	//      _warthog_inc = 370, // Warthog Inc
	              _chill = 380, // Narrows
	          _cyberdyne = 390, // The Pit
	//           _shrine = 400, // Sandtrap
	        _bunkerworld = 410, // Standoff
	//        _landslide = 420, // Landslide
	//        _beachhead = 430, // Beachhead
	//            _docks = 440, // Longshore
	//        _pump_haus = 450, // Pump Haus
	//            _shaft = 460, // Shaft
	//       _sidewinder = 470, // Avalanche
	//        _warehouse = 480, // Foundry
	//          _descent = 490, // Assembly
	//        _spacecamp = 500, // Orbital
	//          _volcano = 510, // Volcano
	//          _lockout = 520, // Blackout
	//           _armory = 580, // Rat's Nest
	//        _ghosttown = 590, // Ghost Town
	//         _chillout = 600, // Cold Storage
	        _s3d_reactor = 700, // Reactor
	//                   = 701, // 
	//                   = 702, // 
	           _s3d_edge = 703, // Edge
	//                   = 704, // 
	      _s3d_avalanche = 705, // Diamondback
	//    _s3d_waterfall = 706, // Whiteout
	//                   = 707, // 
	//                   = 708, // 
	//_s3d_sky_bridgenew = 709, // Sky Bridge New
	//             _damn = 710, // Dam
	//      _s3d_lockout = 711, // Lockup
	//                   = 712, // 
	//                   = 713, // 
	//                   = 714, // 
	//   _s3d_powerhouse = 715, // Powerhouse
	//                   = 716, // 
	//                   = 717, // 
	//                   = 718, // 
	//                   = 719, // 
	//          _midship = 720, // Heretic
	//          _sandbox = 730, // Sandbox
	//         _fortress = 740, // Citadel

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

enum e_engine_variant : long
{
	_engine_variant_base,
	_engine_variant_ctf,
	_engine_variant_slayer,
	_engine_variant_oddball,
	_engine_variant_king,
	_engine_variant_sandbox,
	_engine_variant_vip,
	_engine_variant_juggernaut,
	_engine_variant_territories,
	_engine_variant_assault,
	_engine_variant_infection,
};