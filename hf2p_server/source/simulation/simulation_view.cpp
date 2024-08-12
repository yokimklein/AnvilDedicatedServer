#include "simulation_view.h"

void c_simulation_view::set_view_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier)
{
	DECLFUNC(0x335B0, void, __thiscall, c_simulation_view*, e_simulation_view_establishment_mode, long)(this, establishment_mode, establishment_identifier);
}

bool c_simulation_view::established()
{
	assert(exists());
	if (m_channel != nullptr)
		return m_simulation_interface.established();
	return false;
}

bool c_simulation_view::exists()
{
	return m_view_type.get() != _simulation_view_type_none;
}

e_simulation_view_establishment_mode c_simulation_view::get_view_establishment_mode()
{
	assert(exists());
	assert(m_view_establishment_mode >= 0 && m_view_establishment_mode < k_simulation_view_establishment_mode_count);
	return m_view_establishment_mode;
}

bool c_simulation_view::ready_to_establish()
{
	assert(exists());
	if (m_view_establishment_mode != _simulation_view_establishment_mode_connect)
		return false;
	if (m_valid_view_establishment_mode != 1)
		return false;
	return true;
}

const char* c_simulation_view::get_view_description()
{
	return NULL; // TODO
}