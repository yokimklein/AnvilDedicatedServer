#include "simulation_world.h"
#include "assert.h"

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

bool c_simulation_world::exists()
{
    return this->m_world_type > _simulation_world_type_none;
}

c_simulation_world* simulation_get_world()
{
    assert(simulation_globals->initialized);
    assert(simulation_globals->world);
    assert(simulation_globals->world->exists());
    return simulation_globals->world;
}

bool c_simulation_world::is_distributed()
{
    assert(this->exists());
    return this->m_world_type == _simulation_world_type_distributed_game_authority || this->m_world_state == _simulation_world_type_distributed_game_client;
}

bool c_simulation_world::is_authority()
{
    assert(this->exists());
    return this->m_world_type != _simulation_world_type_synchronous_game_client && this->m_world_state != _simulation_world_type_synchronous_playback_client && this->m_world_state != _simulation_world_type_distributed_game_client;
}

bool c_simulation_world::is_active()
{
    assert(this->exists());
    return m_world_state == _simulation_world_state_active;
}

c_simulation_entity_database* c_simulation_world::get_entity_database()
{
    assert(this->exists());
    assert(this->is_distributed());
    assert(this->m_distributed_world);
    return &this->m_distributed_world->m_entity_database;
}