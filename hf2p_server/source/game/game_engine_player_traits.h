#pragma once
#include <cseries\cseries.h>
#pragma pack(push, 1)

enum e_damage_resistance_percentage_setting
{
	_damage_resistance_percentage_setting_unchanged = 0,

	_damage_resistance_percentage_setting_10_percent,   // 0.1
	_damage_resistance_percentage_setting_50_percent,   // 0.5
	_damage_resistance_percentage_setting_90_percent,   // 0.9
	_damage_resistance_percentage_setting_100_percent,  // 1.0
	_damage_resistance_percentage_setting_110_percent,  // 1.1
	_damage_resistance_percentage_setting_150_percent,  // 1.5
	_damage_resistance_percentage_setting_200_percent,  // 2.0
	_damage_resistance_percentage_setting_300_percent,  // 3.0
	_damage_resistance_percentage_setting_500_percent,  // 5.0
	_damage_resistance_percentage_setting_1000_percent, // 10.0
	_damage_resistance_percentage_setting_2000_percent, // 20.0
	_damage_resistance_percentage_setting_invulnerable, // 1000.0

	k_damage_resistance_percentage_settings,
	k_damage_resistance_percentage_setting_default = _damage_resistance_percentage_setting_100_percent
};

constexpr real k_damage_resistance_percentage_values[k_damage_resistance_percentage_settings - _damage_resistance_percentage_setting_10_percent]
{
	0.1f,
	0.5f,
	0.9f,
	1.0f,
	1.1f,
	1.5f,
	2.0f,
	3.0f,
	5.0f,
	10.0f,
	20.0f,
	1000.0f
};

enum e_shield_recharge_rate_percentage_setting
{
	_shield_recharge_rate_percentage_setting_unchanged = 0,

	_shield_recharge_rate_percentage_setting_negative_25_percent, // -0.25
	_shield_recharge_rate_percentage_setting_negative_10_percent, // -0.1
	_shield_recharge_rate_percentage_setting_negative_5_percent,  // -0.05
	_shield_recharge_rate_percentage_setting_0_percent,           // 0.0
	_shield_recharge_rate_percentage_setting_50_percent,          // 0.5
	_shield_recharge_rate_percentage_setting_90_percent,          // 0.9
	_shield_recharge_rate_percentage_setting_100_percent,         // 1.0
	_shield_recharge_rate_percentage_setting_110_percent,         // 1.1
	_shield_recharge_rate_percentage_setting_200_percent,         // 2.0

	k_shield_recharge_rate_percentage_settings,
	k_shield_recharge_rate_percentage_setting_default = _shield_recharge_rate_percentage_setting_100_percent
};

constexpr real k_shield_recharge_rate_percentage_values[k_shield_recharge_rate_percentage_settings - _shield_recharge_rate_percentage_setting_negative_25_percent]
{
	-0.25f,
	-0.1f,
	-0.05f,
	0.0f,
	0.5f,
	0.9f,
	1.0f,
	1.1f,
	2.0f
};

enum e_vampirism_percentage_setting
{
	_vampirism_percentage_setting_unchanged = 0,

	_vampirism_percentage_setting_0_percent,   // 0.0
	_vampirism_percentage_setting_10_percent,  // 0.1
	_vampirism_percentage_setting_25_percent,  // 0.25
	_vampirism_percentage_setting_50_percent,  // 0.5
	_vampirism_percentage_setting_100_percent, // 1.0

	k_vampirism_percentage_settings,
	k_vampirism_percentage_setting_default = _vampirism_percentage_setting_0_percent
};

constexpr real k_vampirism_percentage_values[k_vampirism_percentage_settings - _vampirism_percentage_setting_0_percent]
{
	0.0f,
	0.1f,
	0.25f,
	0.5f,
	1.0f
};

enum e_headshot_immunity_setting
{
	_headshot_immunity_setting_unchanged = 0,

	_headshot_immunity_setting_immune_to_headshots,
	_headshot_immunity_setting_not_immune_to_headshots,

	k_headshot_immunity_settings
};

enum e_shield_multiplier_setting
{
	_shield_multiplier_setting_unchanged = 0,

	_shield_multiplier_setting_0x, // 0
	_shield_multiplier_setting_1x, // 1
	_shield_multiplier_setting_2x, // 2
	_shield_multiplier_setting_3x, // 3
	_shield_multiplier_setting_4x, // 4

	k_shield_multiplier_settings,
	k_shield_multiplier_setting_default = _shield_multiplier_setting_1x
};

constexpr long k_shield_multiplier_values[k_shield_multiplier_settings - _shield_multiplier_setting_0x]
{
	0,
	1,
	2,
	3,
	4
};

enum e_damage_modifier_percentage_setting
{
	_damage_modifier_percentage_setting_unchanged = 0,

	_damage_modifier_percentage_setting_0_percent,   // 0.0
	_damage_modifier_percentage_setting_25_percent,  // 0.25
	_damage_modifier_percentage_setting_50_percent,  // 0.5
	_damage_modifier_percentage_setting_75_percent,  // 0.75
	_damage_modifier_percentage_setting_90_percent,  // 0.9
	_damage_modifier_percentage_setting_100_percent, // 1.0
	_damage_modifier_percentage_setting_110_percent, // 1.1
	_damage_modifier_percentage_setting_125_percent, // 1.25
	_damage_modifier_percentage_setting_150_percent, // 1.5
	_damage_modifier_percentage_setting_200_percent, // 2.0
	_damage_modifier_percentage_setting_300_percent, // 3.0
	_damage_modifier_percentage_setting_fatality,    // 1000.0

	k_damage_modifier_percentage_settings,
	k_damage_modifier_percentage_setting_default = _damage_modifier_percentage_setting_100_percent
};

constexpr real k_damage_modifier_percentage_values[k_damage_modifier_percentage_settings - _damage_modifier_percentage_setting_0_percent]
{
	0.0f,
	0.25f,
	0.5f,
	0.75f,
	0.9f,
	1.0f,
	1.1f,
	1.25f,
	1.5f,
	2.0f,
	3.0f,
	1000.0f
};

enum e_grenade_count_setting
{
	_grenade_count_setting_unchanged = 0,
	_grenade_count_setting_map_default,

	_grenade_count_setting_none,    // 0
	_grenade_count_setting_one,     // 1
	_grenade_count_setting_two,     // 2
	//_grenade_count_setting_three, // 3
	//_grenade_count_setting_four,  // 4

	k_grenade_count_settings,
	k_grenade_count_setting_default = _grenade_count_setting_two
};

constexpr char k_grenade_count_values[k_grenade_count_settings - _grenade_count_setting_none]
{
	0,
	1,
	2,
	//3,
	//4
};

enum e_recharging_grenades_setting
{
	// UNCHANGED
	_recharging_grenades_setting_unchanged = 0,

	// ENABLED
	_recharging_grenades_setting_enabled,

	// DISABLED
	_recharging_grenades_setting_disabled,

	//// ENABLED INCLUDING FIREBOMBS
	//_recharging_grenades_setting_enabled_including_firebombs,


	k_recharging_grenades_settings
};

enum e_infinite_ammo_setting
{
	// UNCHANGED
	_infinite_ammo_setting_unchanged = 0,

	// DISABLED
	_infinite_ammo_setting_disabled,

	// ENABLED
	_infinite_ammo_setting_enabled,

	//// BOTTOMLESS CLIP
	//_infinite_ammo_setting_bottomless_clip,


	k_infinite_ammo_settings
};

enum e_weapon_pickup_setting
{
	// UNCHANGED
	_weapon_pickup_setting_unchanged = 0,

	// ENABLED
	_weapon_pickup_setting_allowed,

	// DISABLED
	_weapon_pickup_setting_disallowed,


	k_weapon_pickup_settings
};

enum e_player_speed_setting
{
	_player_speed_setting_unchanged = 0,

	_player_speed_setting_25_percent,  // 0.25
	_player_speed_setting_50_percent,  // 0.5
	_player_speed_setting_75_percent,  // 0.75
	_player_speed_setting_90_percent,  // 0.9
	_player_speed_setting_100_percent, // 1.0
	_player_speed_setting_110_percent, // 1.1
	_player_speed_setting_125_percent, // 1.25
	_player_speed_setting_150_percent, // 1.5
	_player_speed_setting_200_percent, // 2.0
	_player_speed_setting_300_percent, // 3.0

	k_player_speed_settings,
	k_player_speed_setting_default = _player_speed_setting_100_percent
};

constexpr real k_player_speed_values[k_player_speed_settings - _player_speed_setting_25_percent]
{
	0.25f,
	0.5f,
	0.75f,
	0.9f,
	1.0f,
	1.1f,
	1.25f,
	1.5f,
	2.0f,
	3.0f
};

enum e_player_gravity_setting
{
	_player_gravity_setting_unchanged = 0,

	_player_gravity_setting_50_percent,  // 0.5
	_player_gravity_setting_75_percent,  // 0.75
	_player_gravity_setting_100_percent, // 1.0
	_player_gravity_setting_150_percent, // 1.5
	_player_gravity_setting_200_percent, // 2.0

	k_player_gravity_settings,
	k_player_gravity_setting_default = _player_gravity_setting_100_percent
};

constexpr real k_player_gravity_values[k_player_gravity_settings - _player_gravity_setting_50_percent]
{
	0.5f,
	0.75f,
	1.0f,
	1.5f,
	2.0f
};

enum e_vehicle_usage_setting
{
	// UNCHANGED
	_vehicle_usage_setting_unchanged = 0,

	// NONE
	_vehicle_usage_setting_none,

	// PASSENGER ONLY
	_vehicle_usage_setting_passenger_only,

	// FULL
	_vehicle_usage_setting_full,


	k_vehicle_usage_settings
};

enum e_active_camo_setting
{
	// UNCHANGED
	_active_camo_setting_unchanged = 0,

	// OFF (0.0)
	_active_camo_setting_off,

	// POOR (0.33)
	_active_camo_setting_poor,

	// GOOD (0.66)
	_active_camo_setting_good,

	// INVISIBLE (1.0)
	_active_camo_setting_invisible,


	k_active_camo_settings
};

enum e_waypoint_setting
{
	// UNCHANGED
	_waypoint_setting_unchanged = 0,

	// OFF
	_waypoint_setting_off,

	// ALLIES
	_waypoint_setting_allies,

	// ALL
	_waypoint_setting_all,


	k_waypoint_settings
};

enum e_aura_setting
{
	// UNCHANGED
	_aura_setting_unchanged = 0,

	// OFF
	_aura_setting_off,

	// TEAM COLOR
	_aura_setting_team_color,

	// BLACK
	_aura_setting_black,

	// WHITE
	_aura_setting_white,


	k_aura_settings
};

enum e_forced_change_color_setting
{
	// UNCHANGED
	_forced_change_color_setting_unchanged = 0,

	// OFF
	_forced_change_color_setting_off,

	// RED
	_forced_change_color_setting_mp_team_red,

	// BLUE
	_forced_change_color_setting_mp_team_blue,

	// GREEN
	_forced_change_color_setting_mp_team_green,

	// YELLOW
	_forced_change_color_setting_mp_team_yellow,

	// PURPLE
	_forced_change_color_setting_mp_team_purple,

	// ORANGE
	_forced_change_color_setting_mp_team_orange,

	// BROWN
	_forced_change_color_setting_mp_team_brown,

	// GREY (PINK)
	_forced_change_color_setting_mp_team_grey,

	// XTRA 1 (PRIMARY COLOR)
	_forced_change_color_setting_extra1,

	// XTRA 2 (SECONDARY COLOR)
	_forced_change_color_setting_extra2,

	// XTRA 3 (TERTIARY COLOR)
	_forced_change_color_setting_extra3,

	// XTRA 4 (QUATERNARY COLOR)
	_forced_change_color_setting_extra4,


	k_forced_change_color_settings
};

enum e_motion_tracker_setting
{
	// UNCHANGED
	_motion_tracker_setting_unchanged = 0,

	// OFF
	_motion_tracker_setting_off,

	// FRIENDLY ONLY
	_motion_tracker_setting_moving_friendly_bipeds_moving_neutral_vehicles,

	// NORMAL
	_motion_tracker_setting_moving_bipeds_moving_vehicles,

	// ALWAYS
	_motion_tracker_setting_all_bipeds_moving_vehicles,


	k_motion_tracker_settings
};

enum e_motion_tracker_range_setting
{
	_motion_tracker_range_setting_unchanged = 0,

	_motion_tracker_range_setting_10_meters,  // 10.0
	_motion_tracker_range_setting_15_meters,  // 15.0
	_motion_tracker_range_setting_25_meters,  // 25.0
	_motion_tracker_range_setting_50_meters,  // 50.0
	_motion_tracker_range_setting_75_meters,  // 75.0
	_motion_tracker_range_setting_100_meters, // 100.0
	_motion_tracker_range_setting_150_meters, // 150.0

	k_motion_tracker_range_settings,
	//k_motion_tracker_range_setting_default = _motion_tracker_range_setting_0_meters // 0.0
};

constexpr real k_motion_tracker_range_values[k_motion_tracker_range_settings - _motion_tracker_range_setting_10_meters]
{
	10.0f,
	15.0f,
	25.0f,
	50.0f,
	75.0f,
	100.0f,
	150.0f
};

class c_player_trait_shield_vitality
{
public:
	void set(c_player_trait_shield_vitality const* traits, bool force);

	e_damage_resistance_percentage_setting get_damage_resistance_percentage_setting() const;
	void set_damage_resistance_percentage_setting(e_damage_resistance_percentage_setting damage_resistance_percentage_setting, bool force);
	real get_damage_resistance_percentage() const;
	void set_damage_resistance_percentage(real damage_resistance_percentage, bool force);

	e_shield_recharge_rate_percentage_setting get_shield_recharge_rate_percentage_setting() const;
	void set_shield_recharge_rate_percentage_setting(e_shield_recharge_rate_percentage_setting shield_recharge_rate_percentage_setting, bool force);
	real get_shield_recharge_rate_percentage() const;
	void set_shield_recharge_rate_percentage(real shield_recharge_rate_percentage, bool force);

	e_vampirism_percentage_setting get_vampirism_percentage_setting() const;
	void set_vampirism_percentage_setting(e_vampirism_percentage_setting vampirism_percentage_setting, bool force);
	real get_vampirism_percentage() const;
	void set_vampirism_percentage(real vampirism_percentage, bool force);

	e_headshot_immunity_setting get_headshot_immunity_setting() const;
	void set_headshot_immunity_setting(e_headshot_immunity_setting headshot_immunity_setting, bool force);
	bool get_headshot_immunity() const;
	void set_headshot_immunity(bool headshot_immunity, bool force);

	e_shield_multiplier_setting get_shield_multiplier_setting() const;
	void set_shield_multiplier_setting(e_shield_multiplier_setting shield_multiplier_setting, bool force);
	long get_shield_multiplier() const;
	void set_shield_multiplier(long shield_multiplier, bool force);
	real get_maximum_shield_vitality();

protected:
	c_enum<e_damage_resistance_percentage_setting, char, _damage_resistance_percentage_setting_unchanged, k_damage_resistance_percentage_settings> m_damage_resistance_percentage_setting;
	c_enum<e_shield_recharge_rate_percentage_setting, char, _shield_recharge_rate_percentage_setting_unchanged, k_shield_recharge_rate_percentage_settings> m_shield_recharge_rate_percentage_setting;
	c_enum<e_vampirism_percentage_setting, char, _vampirism_percentage_setting_unchanged, k_vampirism_percentage_settings> m_vampirism_percentage_setting;
	c_enum<e_headshot_immunity_setting, char, _headshot_immunity_setting_unchanged, k_headshot_immunity_settings> m_headshot_immunity_setting;
	c_enum<e_shield_multiplier_setting, char, _shield_multiplier_setting_unchanged, k_shield_multiplier_settings> m_shield_multiplier_setting;

	// elephant
	byte pad[3];
};
static_assert(sizeof(c_player_trait_shield_vitality) == 0x8);

class c_player_trait_weapons
{
public:
	void set(c_player_trait_weapons const* traits, bool force);

	e_grenade_count_setting get_initial_grenade_count_setting() const;
	void set_initial_grenade_count_setting(e_grenade_count_setting initial_grenade_count_setting, bool force);
	char get_initial_grenade_count() const;
	void set_initial_grenade_count(long initial_grenade_count, bool force);

	char get_initial_primary_weapon_absolute_index() const;
	void set_initial_primary_weapon_absolute_index(char initial_primary_weapon_absolute_index, bool force);
	long get_initial_primary_weapon() const;
	void set_initial_primary_weapon(long initial_primary_weapon, bool force);

	char get_initial_secondary_weapon_absolute_index() const;
	void set_initial_secondary_weapon_absolute_index(char initial_secondary_weapon_absolute_index, bool force);
	long get_initial_secondary_weapon() const;
	void set_initial_secondary_weapon(long initial_secondary_weapon, bool force);

	e_damage_modifier_percentage_setting get_damage_modifier_percentage_setting() const;
	void set_damage_modifier_percentage_setting(e_damage_modifier_percentage_setting damage_modifier_percentage_setting, bool force);
	real get_damage_modifier_percentage() const;
	void set_damage_modifier_percentage(real damage_modifier_percentage, bool force);

	e_recharging_grenades_setting get_recharging_grenades_setting() const;
	void set_recharging_grenades_setting(e_recharging_grenades_setting recharging_grenades_setting, bool force);
	bool get_recharging_grenades() const;
	void set_recharging_grenades(bool recharging_grenades, bool force);

	e_infinite_ammo_setting get_infinite_ammo_setting() const;
	void set_infinite_ammo_setting(e_infinite_ammo_setting infinite_ammo_setting, bool force);
	bool infinite_ammo_enabled() const;
	void set_infinite_ammo(bool infinite_ammo, bool force);

	e_weapon_pickup_setting get_weapon_pickup_setting() const;
	void set_weapon_pickup_allowed_setting(e_weapon_pickup_setting weapon_pickup_setting, bool force);
	bool get_weapon_pickup_allowed() const;
	void set_weapon_pickup_allowed(bool weapon_pickup_allowed, bool force);

protected:
	c_enum<e_grenade_count_setting, word, _grenade_count_setting_unchanged, k_grenade_count_settings> m_initial_grenade_count_setting;
	char m_initial_primary_weapon_absolute_index;
	char m_initial_secondary_weapon_absolute_index;
	c_enum<e_damage_modifier_percentage_setting, char, _damage_modifier_percentage_setting_unchanged, k_damage_modifier_percentage_settings> m_damage_modifier_percentage_setting;
	c_enum<e_recharging_grenades_setting, char, _recharging_grenades_setting_unchanged, k_recharging_grenades_settings> m_recharging_grenades_setting;
	c_enum<e_infinite_ammo_setting, char, _infinite_ammo_setting_unchanged, k_infinite_ammo_settings> m_infinite_ammo_setting;
	c_enum<e_weapon_pickup_setting, char, _weapon_pickup_setting_unchanged, k_weapon_pickup_settings> m_weapon_pickup_setting;
};
static_assert(sizeof(c_player_trait_weapons) == 0x8);

class c_player_trait_movement
{
public:
	void set(c_player_trait_movement const* traits, bool force);

	e_player_speed_setting get_speed_setting() const;
	void set_speed_setting(e_player_speed_setting speed_setting, bool force);
	real get_speed() const;
	void set_speed(real speed, bool force);

	e_player_gravity_setting get_gravity_setting() const;
	void set_gravity_setting(e_player_gravity_setting gravity_setting, bool force);
	real get_gravity() const;
	void set_gravity(real gravity, bool force);

	e_vehicle_usage_setting get_vehicle_usage_setting() const;
	void set_vehicle_usage_setting(e_vehicle_usage_setting vehicle_usage_setting, bool force);

protected:
	c_enum<e_player_speed_setting, char, _player_speed_setting_unchanged, k_player_speed_settings> m_speed_setting;
	c_enum<e_player_gravity_setting, char, _player_gravity_setting_unchanged, k_player_gravity_settings> m_gravity_setting;
	c_enum<e_vehicle_usage_setting, char, _vehicle_usage_setting_unchanged, k_vehicle_usage_settings> m_vehicle_usage_setting;

	// shark
	byte pad;
};
static_assert(sizeof(c_player_trait_movement) == 0x4);

// Traits that affect the player's appearance
class c_player_trait_appearance
{
public:
	void set(c_player_trait_appearance const* traits, bool force);

	e_active_camo_setting get_active_camo_setting() const;
	void set_active_camo_setting(e_active_camo_setting active_camo_setting, bool force);

	e_waypoint_setting get_waypoint_setting() const;
	void set_waypoint_setting(e_waypoint_setting waypoint_setting, bool force);

	e_aura_setting get_aura_setting() const;
	void set_aura_setting(e_aura_setting aura_setting, bool force);

	e_forced_change_color_setting get_forced_change_color_setting() const;
	void set_forced_change_color_setting(e_forced_change_color_setting forced_change_color_setting, bool force);

protected:
	c_enum<e_active_camo_setting, char, _active_camo_setting_unchanged, k_active_camo_settings> m_active_camo_setting;
	c_enum<e_waypoint_setting, char, _waypoint_setting_unchanged, k_waypoint_settings> m_waypoint_setting;
	c_enum<e_aura_setting, char, _aura_setting_unchanged, k_aura_settings> m_aura_setting;
	c_enum<e_forced_change_color_setting, char, _forced_change_color_setting_unchanged, k_forced_change_color_settings> m_forced_change_color_setting;
};
static_assert(sizeof(c_player_trait_appearance) == 0x4);

// Traits that affect the HUD motion sensor behavior
class c_player_trait_sensors
{
public:
	void set(c_player_trait_sensors const* traits, bool force);

	e_motion_tracker_setting get_motion_tracker_setting() const;
	void set_motion_tracker_setting(e_motion_tracker_setting motion_tracker_setting, bool force);

	e_motion_tracker_range_setting get_motion_tracker_range_setting() const;
	void set_motion_tracker_range_setting(e_motion_tracker_range_setting motion_tracker_range_setting, bool force);
	real get_motion_tracker_range_meters() const;

protected:
	c_enum<e_motion_tracker_setting, short, _motion_tracker_setting_unchanged, k_motion_tracker_settings> m_motion_tracker_setting;
	c_enum<e_motion_tracker_range_setting, short, _motion_tracker_range_setting_unchanged, k_motion_tracker_range_settings> m_motion_tracker_range_setting;
};
static_assert(sizeof(c_player_trait_sensors) == 0x4);

class c_player_traits
{
public:
	void set(c_player_traits const* traits, bool force);

	c_player_trait_shield_vitality* get_shield_vitality_traits_writeable();
	c_player_trait_shield_vitality const* get_shield_vitality_traits() const;
	void set_shield_vitality_traits(c_player_trait_shield_vitality const* traits, bool force);

	c_player_trait_weapons* get_weapons_traits_writeable();
	c_player_trait_weapons const* get_weapons_traits() const;
	void set_weapons_traits(c_player_trait_weapons const* traits, bool force);

	c_player_trait_movement* get_movement_traits_writeable();
	c_player_trait_movement const* get_movement_traits() const;
	void set_movement_traits(c_player_trait_movement const* traits, bool force);

	c_player_trait_appearance* get_appearance_traits_writeable();
	c_player_trait_appearance const* get_appearance_traits() const;
	void set_appearance_traits(c_player_trait_appearance const* traits, bool force);

	c_player_trait_sensors* get_sensor_traits_writeable();
	c_player_trait_sensors const* get_sensor_traits() const;
	void set_sensor_traits(c_player_trait_sensors const* traits, bool force);

protected:
	c_player_trait_shield_vitality m_shield_vitality_traits;
	c_player_trait_weapons m_weapon_traits;
	c_player_trait_movement m_movement_traits;
	c_player_trait_appearance m_appearance_traits;
	c_player_trait_sensors m_sensor_traits;
};
static_assert(sizeof(c_player_traits) == 0x1C);

#pragma pack(pop)