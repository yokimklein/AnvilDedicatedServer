#pragma once
#include "..\..\cseries\cseries.h"

struct s_replication_event_manager_view_statistics
{
    long __unknown0;
    long events_pending;
    long events_in_transit;
};
static_assert(sizeof(s_replication_event_manager_view_statistics) == 0xC);

class c_replication_event_manager_view
{
	
};