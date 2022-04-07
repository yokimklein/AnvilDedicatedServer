#include "network_utilities.h"

void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version)
{
	*executable_type = 5;
	*executable_version = 11;
	*compatible_version = 11;
}