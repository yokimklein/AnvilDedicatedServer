#include "simulation_world.h"
#include "assert.h"

void c_simulation_world::update_establishing_view(c_simulation_view* view)
{
    assert(exists());
    assert(is_authority());
    assert(view);
    assert(!view->established());
    if (view->get_view_establishment_mode() != _simulation_view_establishment_mode_established)
    {
        if (view->ready_to_establish())
        {
            long establishment_identifier = m_view_establishment_identifier++;
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
}

bool c_simulation_world::exists()
{
    return m_world_type > _simulation_world_type_none;
}

c_simulation_world* simulation_get_world()
{
    assert(simulation_globals.initialized);
    assert(simulation_globals.world);
    assert(simulation_globals.world->exists());
    return simulation_globals.world;
}

bool c_simulation_world::is_distributed()
{
    assert(exists());
    return m_world_type == _simulation_world_type_distributed_game_authority || m_world_state == _simulation_world_type_distributed_game_client;
}

bool c_simulation_world::is_authority()
{
    assert(exists());
    return m_world_type != _simulation_world_type_synchronous_game_client && m_world_state != _simulation_world_type_synchronous_playback_client && m_world_state != _simulation_world_type_distributed_game_client;
}

bool c_simulation_world::is_active()
{
    assert(exists());
    return m_world_state == _simulation_world_state_active;
}

c_simulation_entity_database* c_simulation_world::get_entity_database()
{
    assert(exists());
    assert(is_distributed());
    assert(m_distributed_world);
    return &m_distributed_world->m_entity_database;
}

c_simulation_event_handler* c_simulation_world::get_event_handler()
{
    assert(exists());
    assert(is_distributed());
    assert(m_distributed_world);
    return &m_distributed_world->m_entity_database.m_event_handler;
}

long c_simulation_world::get_machine_index_by_identifier(s_machine_identifier const* remote_machine_identifier)
{
    assert(exists());
    assert(m_watcher);
    return m_watcher->get_machine_index_by_identifier(remote_machine_identifier);
}