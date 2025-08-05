#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_default.h>
#include <game\game_engine_vip_traits.h>
#include <game\game_engine_area_set.h>
#include <objects\multiplayer_game_objects.h>

struct s_game_engine_vip_variant_definition;
class c_game_engine_vip_variant : public c_game_engine_base_variant
{
public:
	c_game_engine_vip_variant* constructor();

	void set(c_game_engine_vip_variant const* variant, bool force);
	void set(s_game_engine_vip_variant_definition const* definition);

	bool get_single_vip() const;
	void set_single_vip(bool single_vip);

	bool get_destination_zones_enabled() const;
	void set_destination_zones_enabled(bool destination_zones_enabled);

	bool get_should_end_round_on_vip_death() const;
	void set_end_round_on_vip_death(bool end_round_on_vip_death);

	//virtual long get_score_to_win_round() override;?
	void set_score_to_win_round(short score_to_win_round);

	char get_kill_points() const;
	void set_kill_points(char kill_points);

	char get_takedown_points() const;
	void set_takedown_points(char takedown_points);

	char get_kill_as_vip_points() const;
	void set_kill_as_vip_points(char kill_as_vip_points);

	char get_vip_death_points() const;
	void set_vip_death_points(char vip_death_points);

	char get_destination_arrival_points() const;
	void set_destination_arrival_points(char destination_arrival_points);

	char get_suicide_points() const;
	void set_suicide_points(char suicide_points);

	char get_betrayal_points() const;
	void set_betrayal_points(char betrayal_points);

	char get_vip_suicide_points() const;
	void set_vip_suicide_points(char vip_suicide_points);

	e_vip_vip_selection_settings get_vip_selection() const;
	void set_vip_selection(e_vip_vip_selection_settings vip_selection);

	e_vip_zone_movement_settings get_zone_movement() const;
	long get_zone_movement_time_in_seconds() const;
	void set_zone_movement(e_vip_zone_movement_settings zone_movement);

	e_vip_zone_order_settings get_zone_order() const;
	void set_zone_order(e_vip_zone_order_settings zone_order);

	short get_influence_radius() const;
	void set_influence_radius(short influence_radius);

	c_player_traits* get_vip_team_traits_writeable();
	c_player_traits const* get_vip_team_traits() const;
	void set_vip_team_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_vip_influence_traits_writeable();
	c_player_traits const* get_vip_influence_traits() const;
	void set_vip_influence_traits(c_player_traits const* traits, bool force);

	c_player_traits* get_vip_traits_writeable();
	c_player_traits const* get_vip_traits() const;
	void set_vip_traits(c_player_traits const* traits, bool force);

protected:
	c_enum<long, short, 0, 500> m_score_to_win_round;         // default: 15
	c_enum<long, short, 0, 500> m_early_win_score;            // default: 10, halo online specific
	c_flags<e_vip_variant_flags, word_flags, k_vip_variant_flags> m_variant_flags;
	c_enum<long, char, -10, 10> m_kill_points;                // default: 0
	c_enum<long, char, -10, 10> m_takedown_points;            // default: 0
	c_enum<long, char, -10, 10> m_kill_as_vip_points;         // default: 0
	c_enum<long, char, -10, 10> m_vip_death_points;           // default: 0
	c_enum<long, char, -10, 10> m_destination_arrival_points; // default: 0
	c_enum<long, char, -10, 10> m_suicide_points;             // default: 0
	c_enum<long, char, -10, 10> m_betrayal_points;            // default: 0
	c_enum<long, char, -10, 10> m_vip_suicide_points;         // default: 0
	c_enum<e_vip_vip_selection_settings, byte, _vip_vip_selection_settings_random, k_vip_vip_selection_settings> m_vip_selection;
	c_enum<e_vip_zone_movement_settings, byte, _vip_zone_movement_settings_off, k_number_of_vip_zone_movement_settings> m_zone_movement;
	c_enum<e_vip_zone_order_settings, byte, _vip_zone_order_settings_random, k_vip_zone_order_settings> m_zone_order;

	byte m_pad1[1];

	c_enum<long, short, 0, 50> m_influence_radius;
	c_player_traits m_vip_team_traits;
	c_player_traits m_vip_influence_traits;
	c_player_traits m_vip_traits;
};
static_assert(sizeof(c_game_engine_vip_variant) == 0x238);

struct c_vip_engine : c_game_engine
{
public:
	//void dump_settings(s_file_reference* file) const;
};

struct c_destination_zone : c_area
{
	byte_flags m_team_designator_flags;
};
static_assert(sizeof(c_destination_zone) == 0x68);

struct s_vip_globals
{
	c_static_array<long, 8> vip_player_index;
	long defending_team_index;
	c_static_array<s_multiplayer_object_boundary_geometry_data, 8> vip_influence_area;
	c_area_set<c_destination_zone, 12> zones;
};
static_assert(sizeof(s_vip_globals) == 0x794);