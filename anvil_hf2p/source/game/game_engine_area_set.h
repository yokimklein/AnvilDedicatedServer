#pragma once
#include <cseries\cseries.h>
#include <objects\multiplayer_game_objects.h>
#include <math\real_math.h>
#include <game\game.h>

class c_area
{
public:
	void set_selected(bool selected);

	s_multiplayer_object_boundary_geometry_data m_boundary_data;
	real_argb_color m_color;
	long m_object_index;
};
static_assert(sizeof(c_area) == 0x64);

template<typename t_area_type, long k_area_max_count>
class c_area_set
{
	static_assert(__is_base_of(c_area, t_area_type));

public:
	t_area_type* begin()
	{
		return m_areas.begin();
	}

	t_area_type* end()
	{
		return &m_areas.get_elements()[m_number_of_areas];
	}

	void activate_all()
	{
		if (!game_is_authoritative())
		{
			return;
		}

		for (t_area_type* area = begin(); area != end(); ++area)
		{
			area->set_selected(true);
		}
	}

	c_static_array<t_area_type, k_area_max_count> m_areas;
	long m_number_of_areas;
	long m_team_designator;
	long m_area_object_type_flags;
	long m_area_iteration_match_flags;
};
static_assert(sizeof(c_area_set<c_area, 1>) == 0x74);