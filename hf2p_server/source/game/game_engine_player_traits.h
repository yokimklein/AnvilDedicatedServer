#pragma once
#include <cstdint>
#include <windows.h>

struct c_player_health_traits
{
	uint8_t m_damage_resistance;
	uint8_t m_shield_recharge_rate;
	uint8_t m_shield_vampirism;
	uint8_t m_headshot_immunity;
	uint8_t m_shield_multiplier;
	uint8_t unknown5;
	uint8_t unknown6;
	uint8_t unknown7;
};

struct c_player_weapon_traits
{
	int16_t m_grenade_count;
	uint8_t m_primary_weapon;
	uint8_t m_secondary_weapon;
	uint8_t m_damage_modifier;
	uint8_t m_grenade_recharge;
	uint8_t m_infinite_ammo;
	uint8_t m_weapon_pickup;
};

struct c_player_movement_traits
{
	uint8_t m_player_speed;
	uint8_t m_player_gravity;
	uint8_t m_vehicle_use;
	uint8_t unknown4;
};

struct c_player_appearance_traits
{
	uint8_t m_active_camo;
	uint8_t m_waypoint;
	uint8_t m_aura;
	uint8_t m_forced_color;
};

struct c_player_sensor_traits
{
	uint16_t m_motion_tracker_mode;
	uint16_t m_motion_tracker_range;
};

struct c_player_traits
{
	c_player_health_traits health_traits;
	c_player_weapon_traits weapon_traits;
	c_player_movement_traits movement_traits;
	c_player_appearance_traits appearance_traits;
	c_player_sensor_traits sensor_traits;
};

