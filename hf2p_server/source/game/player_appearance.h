#pragma once
#include <cstdint>
#include <windows.h>

// IDs are based on globals\hf2p_globals.hf2p_globals > RaceArmors[0].Genders[0].ArmorObjects
// this assumes you are playing as a spartan, genders use the same IDs
enum e_armor_type : byte
{
	_armor_air_assault,
	_armor_ballista,
	_armor_blaster,
	_armor_boxhead,
	_armor_bushido,
	_armor_buzzsaw,
	_armor_cain,
	_armor_cain_rare,
	_armor_chameleon,
	_armor_cyclops,
	_armor_defender,
	_armor_demo,
	_armor_diablo,
	_armor_dutch,
	_armor_dynamo,
	_armor_enigma,
	_armor_eod,
	_armor_eva,
	_armor_excavator,
	_armor_explorer,
	_armor_extractor,
	_armor_gargoyle,
	_armor_gargoyle_rare,
	_armor_ghoul,
	_armor_gladiator,
	_armor_goku,
	_armor_guardian,
	_armor_gungnir,
	_armor_halberd,
	_armor_hammerhead,
	_armor_hellcat,
	_armor_hoplite,
	_armor_iron,
	_armor_jawbone,
	_armor_juggernaut,
	_armor_knight,
	_armor_lancer,
	_armor_lotus,
	_armor_mac,
	_armor_major,
	_armor_mantis,
	_armor_mantis_rare,
	_armor_mercenary,
	_armor_mirmillion,
	_armor_mongoose,
	_armor_navigator,
	_armor_nihard,
	_armor_ninja,
	_armor_ninja_rare,
	_armor_omni,
	_armor_optimus,
	_armor_oracle,
	_armor_orbital,
	_armor_paladin,
	_armor_paladin_rare,
	_armor_panther,
	_armor_pharaoh,
	_armor_pilot,
	_armor_poncho,
	_armor_prophet,
	_armor_pulse,
	_armor_ranger,
	_armor_razor,
	_armor_recon,
	_armor_renegade,
	_armor_renegade_rare,
	_armor_rhino,
	_armor_rogue,
	_armor_rogue_rare,
	_armor_rookie,
	_armor_scanner,
	_armor_scout,
	_armor_seeker,
	_armor_shark,
	_armor_shark_rare,
	_armor_silencer,
	_armor_silverback,
	_armor_solar,
	_armor_spectrum,
	_armor_stealth,
	_armor_stealth_rare,
	_armor_strider,
	_armor_swat,
	_armor_tanker,
	_armor_triage,
	_armor_utility,
	_armor_vortex,
	_armor_warrior,
	_armor_wasp,
	_armor_widow_maker
};

// IDs are based on multiplayer\multiplayer_globals.multiplayer_globals > Universal[0].GameVariantWeapons
enum e_weapon_type : byte
{
	_battle_rifle,
	_assault_rifle,
	_plasma_pistol, // weapon tag missing in ms29
	_spike_rifle, // weapon tag missing in ms29
	_smg,
	_carbine,
	_energy_sword,
	_magnum,
	_needler,
	_plasma_rifle,
	_rocket_launcher,
	_shotgun,
	_sniper_rifle,
	_brute_shot,
	_unarmed, // 'useless' energy sword variant
	_beam_rifle,
	_spartan_laser,
	_none, // no weapon reference
	_gravity_hammer,
	_excavator, // weapon tag missing in ms29
	_flamethrower, // weapon tag missing in ms29
	_missile_pod,
	_dmr,
	_assault_rifle_v1,
	_assault_rifle_v2,
	_assault_rifle_v3,
	_assault_rifle_v4,
	_assault_rifle_v5, // weapon tag missing in ms29
	_assault_rifle_v6,
	_battle_rifle_v1,
	_battle_rifle_v2,
	_battle_rifle_v3, // weapon tag missing in ms29
	_battle_rifle_v4,
	_battle_rifle_v5,
	_battle_rifle_v6,
	_dmr_v1,
	_dmr_v2,
	_dmr_v3,
	_dmr_v4,
	_dmr_v5, // weapon tag missing in ms29
	_dmr_v6,
	_smg_v1,
	_smg_v2,
	_smg_v3,
	_smg_v4, // weapon tag missing in ms29
	_smg_v5,
	_smg_v6, // weapon tag missing in ms29
	_plasma_rifle_v1, // weapon tag missing in ms29
	_plasma_rifle_v2, // weapon tag missing in ms29
	_plasma_rifle_v3, // weapon tag missing in ms29
	_plasma_rifle_v4, // weapon tag missing in ms29
	_plasma_rifle_v5, // weapon tag missing in ms29
	_plasma_rifle_v6, // weapon tag missing in ms29
	_covenant_carbine_v1, // weapon tag missing in ms29
	_covenant_carbine_v2, // weapon tag missing in ms29
	_covenant_carbine_v3, // weapon tag missing in ms29
	_covenant_carbine_v4, // weapon tag missing in ms29
	_covenant_carbine_v5, // weapon tag missing in ms29
	_covenant_carbine_v6, // weapon tag missing in ms29
	_excavator_v1, // weapon tag missing in ms29
	_excavator_v2, // weapon tag missing in ms29
	_excavator_v3, // weapon tag missing in ms29
	_magnum_v1,
	_magnum_v2,
	_magnum_v3,
	_plasma_pistol_v1, // weapon tag missing in ms29
	_plasma_pistol_v2, // weapon tag missing in ms29
	_plasma_pistol_v3, // weapon tag missing in ms29
	_needle_rifle, // weapon tag missing in ms29
	_battle_rifle_v7
};

// IDs are based on globals\globals.globals > Grenades
enum e_grenade_type : byte
{
	_frag_grenade,
	_plasma_grenade,
	_claymore_grenade
	//_firebomb_grenade - removed in later builds
};

// IDs are unknown - TODO
enum e_support_package : byte
{

};

// IDs are based on multiplayer\multiplayer_globals.multiplayer_globals > Universal[0].Equipment
enum e_tactical_package : byte
{
	_empty,
	_jammer,
	_powerdrain,
	_invisibility,
	_invisibility_vehicle,
	_bubbleshield,
	_superflare,
	_regenerator,
	_tripmine,
	_auto_turret,
	_deployable_cover,
	_forced_reload,
	_concussive_blast,
	_tank_mode,
	_mag_pulse,
	_hologram,
	_reactive_armor,
	_bomb_run,
	_armor_lock,
	_adrenaline,
	_lightning_strike,
	_scrambler,
	_weapon_jammer,
	_ammo_pack,
	_consumable_vision,
	_bubbleshield_tutorial,
	_consumable_vision_tutorial
};

struct s_player_appearance_unknown3
{
	uint16_t unknown0;
	uint16_t unknown2;
	uint16_t unknown4;
	uint16_t unknown6;
	uint16_t unknown8;
	uint16_t unknownA;
	uint16_t unknownC;
	byte unknownE;
	byte unknownF;
};
static_assert(sizeof(s_player_appearance_unknown3) == 0x10);

struct s_player_appearance_unknown2
{
	s_player_appearance_unknown3 unknown[50];
	long unknown_count; // max of 50
};
static_assert(sizeof(s_player_appearance_unknown2) == 0x324);

struct s_player_appearance_unknown1
{
	s_player_appearance_unknown2 unknown1[2];
	uint32_t unknown2[2]; // `unknown1` hashes? only set if `unknown1.unknown_count` != 0
};
static_assert(sizeof(s_player_appearance_unknown1) == 0x650);

struct s_player_appearance // lives in player_appearance.h?
{
	byte flags; // gender
	byte player_model_index; // 0 - male spartan, 1 - elite, 2 - female spartan, 3 - female elite?
	byte align2[2];
	s_player_appearance_unknown1 unknown_struct;
	wchar_t service_tag[5];
	byte padding[2];
};
static_assert(sizeof(s_player_appearance) == 0x660);

struct s_s3d_player_loadout
{
	byte flags; // gender
	e_armor_type armor;
	e_weapon_type primary_weapon;
	e_weapon_type secondary_weapon;
	e_grenade_type grenade;
	e_support_package support_pack;
	e_tactical_package tactical_packs[4];
};
static_assert(sizeof(s_s3d_player_loadout) == 0xA);

struct s_s3d_player_armor
{
	uint32_t armor_indices[95];
};
static_assert(sizeof(s_s3d_player_armor) == 0x17C);

struct s_s3d_player_appearance
{
	byte unknown; // has loadouts? current loadout?
	s_s3d_player_loadout loadouts[3]; // 3 loadout sets
	byte align1F[1];
	s_s3d_player_armor armors[3]; // unused since the armr system/single sets were introduced?
};
static_assert(sizeof(s_s3d_player_appearance) == 0x494);

struct s_s3d_player_customization
{
	byte unknown0;
	byte unknown1;
	byte nameplate;
	byte emblem;
	uint32_t colours[5]; // primary, secondary, visor, lights, holo? real_argb
	byte data[4];
};
static_assert(sizeof(s_s3d_player_customization) == 0x1C);