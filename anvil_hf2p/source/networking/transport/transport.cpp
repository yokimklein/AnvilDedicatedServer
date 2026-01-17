#include "transport.h"
#include "cseries\cseries.h"
#include "cseries\cseries_events.h"
#include "networking\transport\transport_security.h"

REFERENCE_DECLARE(0x4EBE948, s_transport_globals, transport_globals);

void transport_initialize()
{
	INVOKE(0x39C0, transport_initialize);
}

void transport_global_update()
{
	if (transport_globals.initialized)
	{
		bool available = transport_network_available();
		if (transport_globals.is_started != available)
		{
			transport_globals.is_started = available;
			event(_event_message, "networking:transport: Network interface connection restored, resetting networking");
			transport_startup();
		}
		transport_secure_address_resolve();
	}
}

bool transport_network_available()
{
	return transport_globals.initialized;
}

void transport_startup()
{
	INVOKE(0x3A50, transport_startup);
}

bool transport_available()
{
	return transport_globals.initialized && transport_globals.winsock_initialized;
}