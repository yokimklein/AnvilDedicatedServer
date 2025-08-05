#include "simulation_view.h"

void c_simulation_view::set_view_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier)
{
	ASSERT(exists());
	ASSERT(establishment_mode >= 0 && establishment_mode < k_simulation_view_establishment_mode_count);
	ASSERT(m_world != NULL);
	ASSERT(m_channel != NULL);
	if (is_dead(NULL))
	{
		ASSERT(establishment_mode == _simulation_view_establishment_mode_none);
	}

	DECLFUNC(0x335B0, void, __thiscall, c_simulation_view*, e_simulation_view_establishment_mode, long)(this, establishment_mode, establishment_identifier);
}

bool c_simulation_view::established() const
{
	ASSERT(exists());
	if (m_channel)
	{
		return m_channel_interface.established();
	}
	return false;
}

bool c_simulation_view::exists() const
{
	return m_view_type != _simulation_view_type_none;
}

e_simulation_view_establishment_mode c_simulation_view::get_view_establishment_mode() const
{
	ASSERT(exists());
	ASSERT(m_view_establishment_mode >= 0 && m_view_establishment_mode < k_simulation_view_establishment_mode_count);
	return m_view_establishment_mode;
}

bool c_simulation_view::ready_to_establish() const
{
	ASSERT(exists());
	if (m_view_establishment_mode != _simulation_view_establishment_mode_connect)
	{
		return false;
	}
	if (m_remote_establishment_mode != 1)
	{
		return false;
	}
	return true;
}

const char* c_simulation_view::get_view_description() const
{
	return NULL; // $TODO:
}

bool c_simulation_view::is_dead(e_simulation_view_reason* out_death_reason) const
{
	ASSERT(exists());
	ASSERT(m_view_death_reason >= 0 && m_view_death_reason < k_simulation_view_reason_count);
	if (out_death_reason)
	{
		*out_death_reason = m_view_death_reason;
	}
	return m_view_death_reason != _simulation_view_reason_none;
}