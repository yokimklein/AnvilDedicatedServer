#pragma once
#include <cseries\cseries.h>
#include <text\unicode.h>

constexpr long k_player_configuration_maximum_loadouts = 3; // 5 in earlier builds

enum e_gender
{
	_gender_male,
	_gender_female,

	k_gender_count
};

// IDs are based on globals\hf2p_globals.hf2p_globals > RaceArmors[0].Genders[0].ArmorObjects
// this assumes you are playing as a spartan, genders use the same IDs
enum e_armor // non original name
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
	_armor_widow_maker,

	k_armor_count
};

enum e_armor_colors // ew american spelling
{
	_armor_color_primary,
	_armor_color_secondary,
	_armor_color_visor,
	_armor_color_lights,
	_armor_color_holo,

	k_armor_colors_count
};

// IDs are based on multiplayer\multiplayer_globals.multiplayer_globals > Universal[0].GameVariantWeapons
enum e_weapon // non-original name
{
	_weapon_random = -2,
	_weapon_none = -1,
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
	_unknown, // no weapon reference
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
	_battle_rifle_v7,

	k_weapon_count
};

// IDs are based on globals\globals.globals > Grenades
enum e_grenade
{
	_frag_grenade,
	_plasma_grenade,
	_claymore_grenade,
	_firebomb_grenade, // tag missing in ms29

	k_grenade_count
};

// TODO - find out what these are
enum e_support_package
{
	k_support_package_count
};

enum e_modifiers
{
	_safety_booster, // bool (Forcefully ejects the player they're in on destruction)
	_explosives_damage, // unknown
	_explosives_aoe_size_boost, // real 0.0-max (Additional AOE for all grenades. Not sure what this is measured in)

	// The number of nades scavenged per type is the number of empty grenade slots IF the empty slot count is <= your current grenade count, otherwise it is your grenade count
	// This is clamped by the modifier value so that it cannot exceed it
	// This is done per type, with frag grenades prioritised first, then plasma, spike & flame
	// Empty grenade slots are calculated using the globals tag's Grenades > MaximumCount field, not the modifier maximum
	// You cannot recover grenades from your previous life's body, though theoretically you can from your bodies preceeding your previous life
	// Once a body has been scavenged from, it cannot be scavenged from again by any player
	_grenade_scavenger, // int 0-4, default 1 (Total number of grenades to try scavenge off a body)

	_stamina_restore_near_death, // real 0.0-max, default 0.0 (Seconds restored to sprint meter when near death?)
	_cooldown_reset, // bool (Allows players to spawn without the 10 second equipment cooldown)
	_reflex_boost, // real -0.99900001-max, default 0.0 (Sprint exit speed)
	_zoom_radar, // bool (Keeps radar on the hud whilst scoped with any weapon)
	_cooldown_boost, // real 0.0-max, default 0.0 (Cooldown multiplier + 1.0)
	_enable_nemesis_mechanics, // bool (Enable Nemesis/Avenger mechanics)
	_silent_sprint, // bool (Disables sprint sounds)
	// If multiple passengers use this, only the highest modifier value is used
	// If you are using shield_regen_rate, only the highest value of the two will be used
	_vehicle_shield_regen_speed, // real 0.0-max, default 0.0 (Shield regen speed/rate for all friendly passengers in your vehicle)
	// If multiple passengers use this, only the smallest modifier value is used
	// If you are using shield_regen_delay, only the smallest value of the two will be used
	_vehicle_shield_regen_delay, // real -max-0.0, default 0.0 (Negative number of seconds + 1.0 removed from the shield regen delay for all friendly passengers in your vehicle)
	// Seems to apply to your entire team, only the highest value on your team is used
	_scavenger_aura, // real 0.0-max (Weapon ammo scavenging. I'm not actually sure what this does, the code suggests it modifies the ammo counts, not the pickup radius?)
	_armor_to_vehicle, // int 0-0, default 0 (Code suggests this is unimplemented? Though you could use a negative value here and it would work? Armor tends to be health)
	_armor_to_player_in_vehicle, // real 0.0-max ()
	_armor_to_player_in_vehicle_transferred, // real 0.0-max ()
	_sprint_speed, // real max-max (Additional speed multiplier added onto Globals > PlayerControl > SprintFullSpeedMultiplier)
	_sprint_stamina, // real 0.0-max, default 0.0 (Seconds added on top of Globals > PlayerControl > StaminaDepleteRestoreTime)
	_reload_speed, // real -0.99900001-max, default 0.0 (Reload speed multiplier for all weapons + 1.0)
	_weapon_switch_speed, // real -0.99900001-max, default 0.0 (Weapon switch speed multiplier for all weapons + 1.0)
	_ammo_max,
	_melee_damage, // real 0.0-max ()
	_melee_resist, // real 0.0-max ()
	// This value is multiplied by 1 to a max of 3 every time the player dies
	// There's nothing in the code to suggest this occurs when they die without scoring as the modifier description says
	_revenge_shield_boost, // real 0.0-max, default 0.0 (The amount your shields are multiplied by on spawn + 1.0, a value of 1.0 would grand you an extra layer of shields every time a player successively kills you)
	_respawn_modifier, // int 0-max (Number of seconds removed from your respawn timer)
	_grenades_max, // int 0-100 (Number of additional grenades to spawn with on top of the 2 of the type assigned to your loadout)
	_motion_sensor_range, // real 0.0-max (Number of additional meters of range on top of the gamemode default)
	// If you are in a vehicle and a passenger is using vehicle_shield_regen_speed, only the highest value of the two will be used
	_shield_regen_rate, // real 0.0-max, default 0.0 (Shield regen speed/rate)
	// If you are in a vehicle and a passenger is using vehicle_shield_regen_delay, only the smallest value of the two will be used
	_shield_regen_delay, // real -max-0.0, default 0.0 (Negative number of seconds + 1.0 removed from the shield regen delay)
	_teflon_armor, // real max-max, default 0.0 (Float chance vs RNG that sticky projectiles don't stick. I'm not sure what the chance is)
	_explosion_resist, // real 0.0-max (1.0 - explosion resistance)
	_vehicle_ram_damage, // real 0.0-max ()
	_turret_damage, // real 0.0-max ()
	_machine_gun_rof_acceleration_time, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_grenade_warning, // bool (Enables a hud widget which displays the direction of incoming grenades)
	_consumable_duration, // real 0.001-max (Hologram & equipment duration multiplier + 1.0)
	_energy_increase_max, // int 0-2, default 0 (Number of additional energy bars the player has from the default 3 to a max of 5)
	_energy_increase_count_on_spawn, // int -1-4, default 0 (Number of additional charged energy bars the player spawns with from the default 1)
	_energy_regeneration_rate, // real 0.0-max (Energy regeneration rate + 1.0 in seconds?)
	_assault_rifle_rounds_per_second, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_assault_rifle_damage, // real 0.0-max ()
	_assault_rifle_ammo_capacity,
	_assault_rifle_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_battle_rifle_rounds_per_second, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_battle_rifle_fire_recovery_time, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_battle_rifle_damage, // real 0.0-max ()
	_battle_rifle_ammo_capacity,
	_battle_rifle_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_shotgun_spread,
	_shotgun_damage, // real 0.0-max ()
	_shotgun_ammo_capacity,
	_shotgun_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_sniper_rifle_zoom_protection, // bool (Stops sniper weapon types from unzooming when the player is hit?)
	_sniper_rifle_damage, // real 0.0-max ()
	_sniper_rifle_ammo_capacity,
	_sniper_rifle_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_dmr_zoom_protection, // bool (Stops dmr weapon types from unzooming when the player is hit?)
	_dmr_damage, // real 0.0-max ()
	_dmr_ammo_capacity,
	_dmr_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_magnum_rounds_per_second, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_magnum_penetration,
	_magnum_accuracy, // real -0.99900001-0.9, default 0.0 (1.0 - Modifier is multiplier)
	_magnum_damage, // real 0.0-max ()
	_magnum_ammo_capacity,
	_magnum_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_plasma_pistol_charge_up_time, // real -0.99900001-max, default 0.0 (1.0 - Modifier is multiplier)
	_plasma_pistol_damage, // real 0.0-max ()
	_plasma_pistol_ammo_capacity,
	_plasma_pistol_heat_per_round, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_frag_grenade_aoe_size, // real 0.0-max (Additional AOE for frag grenades. Not sure what this is measured in)
	_frag_grenade_damage, // real 0.0-max ()
	_frag_grenade_ammo_capacity,
	_plasma_grenade_aoe_size, // real 0.0-max (Additional AOE for plasma grenades. Not sure what this is measured in)
	_plasma_grenade_damage, // real 0.0-max ()
	_plasma_grenade_ammo_capacity,
	_mauler_damage, // real 0.0-max ()
	_spiker_damage, // real 0.0-max ()
	_brute_shot_damage, // real 0.0-max ()
	_plasma_rifle_heat_per_round, // real -0.99900001-max, default 0.0 (Multiplier + 1.0)
	_detonate_on_player_cdt, // bool (Grenades explode instantly on contact with enemy players) CDT = ??Tether, enables tether flag for grenade projectile types
	_detonate_on_vehicle_cdt, // bool (Grenades explode instantly on contact with vehicles)
	_enemies_always_on_radar, // bool (Always shows enemies on radar even if they are moving slow or standing still)
	_plant_plasma_on_death, // bool (Attaches a silent, long fuse plasma grenade to the player's hand on death)
	_shield_regen_rate_near_killed_enemies, // real 0.0-max, default 0.0 (Shield regen rate/speed added onto your standard rate when near 1 or more enemy bodies)
	_shield_recharge_on_melee_kill, // bool (Instantly restores shield vitality to 1.0 on melee kill)
	_frag_grenades_max, // int 0-max (Number of additional frag grenades to spawn with, regardless of loadout grenades)
	_plasma_grenades_max, // int 0-max (Number of additional plasma grenades to spawn with, regardless of loadout grenades)
	_claymore_grenade_aoe_size, // real 0.0-max (Additional AOE for spike grenades. Not sure what this is measured in)
	_smg_damage, // real 0.0-max ()
	_smg_reload_speed, // real -0.99900001-max, default 0.0 (Additional multiplier added to _reload_speed)
	_vehicle_autoflip, // bool (If the player is sat in a vehicle and it flips over, it will flip itself instead of ejecting the players)
	_headshot_heal,
	_weapon_accuracy,

	k_modifiers_count
};

// IDs are based on multiplayer\multiplayer_globals.multiplayer_globals > Universal[0].Equipment
enum e_tactical_package
{
	_tactical_package_empty,
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
	_consumable_vision_tutorial,

	k_tactical_package_count
};

struct s_emblem_info
{
	struct
	{
		struct
		{
			word __unknown0;
			word __unknown2;
			word __unknown4;
			word __unknown6;
			word __unknown8;
			word __unknownA;
			word __unknownC;
			byte __unknownE;
			byte __unknownF;
		} __unknown0[50];

		long __unknown0_count;
	} __unknown0[2];

	// checksums calculated with fast_checksum / hashlittle
	dword __unknown0_checksums[2];
};
static_assert(sizeof(s_emblem_info) == 0x650);

#pragma pack(push, 4)
struct s_player_appearance
{
	s_player_appearance() :
		flags(),
		player_model_choice(), 
		emblem_info(),
		service_tag(),
		pad1(),  // we have to initialise these so leftover garbage bytes don't screw with the membership update checksum
		pad2()
	{
	};

	byte flags; // gender
	byte player_model_choice; // 0 - male spartan, 1 - elite
	byte pad1[2];
	s_emblem_info emblem_info;
	c_static_wchar_string<5> service_tag;
	byte pad2[2];
};
static_assert(sizeof(s_player_appearance) == 0x660);
#pragma pack(pop)

struct s_s3d_player_loadout
{
	c_enum<e_gender, byte, _gender_male, k_gender_count> gender;
	c_enum<e_armor, byte, _armor_air_assault, k_armor_count> armor_suit;
	c_enum<e_weapon, byte, _weapon_random, k_weapon_count> primary_weapon;
	c_enum<e_weapon, byte, _weapon_random, k_weapon_count> secondary_weapon;
	c_enum<e_grenade, byte, _frag_grenade, k_grenade_count> grenade;
	c_enum<e_support_package, byte, k_support_package_count, k_support_package_count> support_pack; // actually boosters? or just for the in game playercard? support packs are mechanically handled by the player modifiers
	c_enum<e_tactical_package, byte, _tactical_package_empty, k_tactical_package_count> tactical_packs[4];
};
static_assert(sizeof(s_s3d_player_loadout) == 0xA);

struct s_s3d_player_modifiers
{
	// bools are represented as values 0.0 (false) and !0.0 (true)
	// integers are cast from real type to int
	// floats are stored as is
	real modifier_values[k_modifiers_count];
};
static_assert(sizeof(s_s3d_player_modifiers) == 0x17C);

#pragma pack(push, 4)
struct s_s3d_player_container
{
	s_s3d_player_container() :
		override_api_data(),
		loadouts(),
		pad(),  // we have to initialise this so leftover garbage bytes don't screw with the membership update checksum
		modifiers()
	{
	};

	// when true, scaleform menus will display the player's loadout as it is set on the game server rather than the API
	bool override_api_data;
	s_s3d_player_loadout loadouts[k_player_configuration_maximum_loadouts];
	byte pad[1];
	s_s3d_player_modifiers modifiers[k_player_configuration_maximum_loadouts];
};
static_assert(sizeof(s_s3d_player_container) == 0x494);
#pragma pack(pop)

struct s_s3d_player_customization
{
	s_s3d_player_customization() :
		override_api_data(),
		active_loadout_index(),
		account_label(),
		emblem(),
		colors(),
		padding()
	{
	};

	// when true, scaleform menus will display the player's loadout as it is set on the game server rather than the API
	bool override_api_data;
	byte active_loadout_index; // loadout index, 0-2
	byte account_label; // nameplate
	byte emblem;
	ulong colors[k_armor_colors_count];
	byte padding[4];
};
static_assert(sizeof(s_s3d_player_customization) == 0x1C);