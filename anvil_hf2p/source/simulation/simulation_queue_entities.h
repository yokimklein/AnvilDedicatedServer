#pragma once

struct s_simulation_entity;

void __fastcall simulation_queue_entity_deletion_insert(s_simulation_entity* entity, bool force_cleanup);