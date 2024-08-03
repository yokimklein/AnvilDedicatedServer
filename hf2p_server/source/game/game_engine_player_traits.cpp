#include "game_engine_player_traits.h"

e_shield_recharge_rate_percentage_setting c_player_trait_shield_vitality::get_shield_recharge_rate_percentage_setting() const
{
	return m_shield_recharge_rate_percentage_setting;
}

real c_player_trait_shield_vitality::get_shield_recharge_rate_percentage() const
{
	e_shield_recharge_rate_percentage_setting setting = get_shield_recharge_rate_percentage_setting();

	const long minimum_value = _shield_recharge_rate_percentage_setting_negative_25_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _shield_recharge_rate_percentage_setting_200_percent))
		return k_shield_recharge_rate_percentage_values[setting - minimum_value];

	return k_shield_recharge_rate_percentage_values[k_shield_recharge_rate_percentage_setting_default - minimum_value];
}

c_player_trait_shield_vitality const* c_player_traits::get_shield_vitality_traits() const
{
	return &m_shield_vitality_traits;
}