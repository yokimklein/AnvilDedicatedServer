#include "simulation_view.h"

void c_simulation_view::set_view_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier)
{
	void(__thiscall* set_view_establishment)(c_simulation_view* thisptr, e_simulation_view_establishment_mode establishment_mode, long establishment_identifier) = reinterpret_cast<decltype(set_view_establishment)>(module_base + 0x335B0);
	set_view_establishment(this, establishment_mode, establishment_identifier);
}