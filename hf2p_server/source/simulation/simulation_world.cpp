#include "simulation_world.h"

void c_simulation_world::update_establishing_view(c_simulation_view* simulation_view)
{
    long view_establishment_mode = simulation_view->m_view_establishment_mode;
    if (view_establishment_mode != _simulation_view_establishment_mode_established)
    {
        if (view_establishment_mode == _simulation_view_establishment_mode_connect)
        {
            if (simulation_view->m_valid_view_establishment_identifier == 1)
            {
                long establishment_identifier = this->m_establishment_identifier;
                this->m_establishment_identifier = establishment_identifier + 1;
                simulation_view->set_view_establishment(_simulation_view_establishment_mode_established, establishment_identifier);
            }
        }
        else
        {
            simulation_view->set_view_establishment(_simulation_view_establishment_mode_connect, -1);
        }
    }
}