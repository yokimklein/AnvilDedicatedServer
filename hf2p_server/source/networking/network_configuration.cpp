#include "network_configuration.h"

REFERENCE_DECLARE(0x10383D0, s_network_configuration, g_network_configuration);
REFERENCE_DECLARE(0x1038282, bool, g_network_configuration_initialized);

s_network_configuration* get_network_configuration()
{
	ASSERT(g_network_configuration_initialized); // Accessing network configuration prior to its initialization!
	return &g_network_configuration;
}