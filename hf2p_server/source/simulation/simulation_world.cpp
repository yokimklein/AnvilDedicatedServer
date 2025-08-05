#include "simulation_world.h"
#include <stdio.h>

void c_simulation_world::update_establishing_view(c_simulation_view* view)
{
    ASSERT(exists());
    ASSERT(is_authority());
    ASSERT(view);
    ASSERT(!view->established());
    if (view->get_view_establishment_mode() == _simulation_view_establishment_mode_established)
    {
        return;
    }
    if (view->ready_to_establish())
    {
        long establishment_identifier = m_next_view_establishment_identifier++;
        printf("MP/NET/SIMULATION: c_simulation_world::update_establishing_view: simulation connected, go established - advancing remote client view %s (mode %d -> %d, new identifier %d)\n",
            view->get_view_description(),
            view->get_view_establishment_mode(),
            _simulation_view_establishment_mode_established,
            establishment_identifier);
        view->set_view_establishment(_simulation_view_establishment_mode_established, establishment_identifier);
    }
    else if (view->get_view_establishment_mode() != _simulation_view_establishment_mode_connect)
    {
        printf("MP/NET/SIMULATION: c_simulation_world::update_establishing_view: view ready to connect, advancing remote client view %s (mode %d -> %d)\n",
            view->get_view_description(),
            view->get_view_establishment_mode(),
            _simulation_view_establishment_mode_connect);
        view->set_view_establishment(_simulation_view_establishment_mode_connect, NONE);
    }
}

bool c_simulation_world::exists()
{
    return m_world_type > _simulation_world_type_none;
}

c_simulation_world* simulation_get_world()
{
    ASSERT(simulation_globals.initialized);
    ASSERT(simulation_globals.world);
    ASSERT(simulation_globals.world->exists());
    return simulation_globals.world;
}

bool c_simulation_world::is_distributed()
{
    ASSERT(exists());
    return m_world_type == _simulation_world_type_distributed_game_authority || m_world_state == _simulation_world_type_distributed_game_client;
}

bool c_simulation_world::is_authority()
{
    ASSERT(exists());
    return m_world_type != _simulation_world_type_synchronous_game_client && m_world_state != _simulation_world_type_synchronous_playback_client && m_world_state != _simulation_world_type_distributed_game_client;
}

bool c_simulation_world::is_active()
{
    ASSERT(exists());
    return m_world_state == _simulation_world_state_active;
}

c_simulation_entity_database* c_simulation_world::get_entity_database()
{
    ASSERT(exists());
    ASSERT(is_distributed());
    ASSERT(m_distributed_world);
    return &m_distributed_world->m_entity_database;
}

c_simulation_event_handler* c_simulation_world::get_event_handler()
{
    ASSERT(exists());
    ASSERT(is_distributed());
    ASSERT(m_distributed_world);
    return &m_distributed_world->m_entity_database.m_event_handler;
}

long c_simulation_world::get_machine_index_by_identifier(s_machine_identifier const* remote_machine_identifier)
{
    ASSERT(exists());
    ASSERT(m_watcher);
    return m_watcher->get_machine_index_by_identifier(remote_machine_identifier);
}