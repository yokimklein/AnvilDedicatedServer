#include "network_utilities.h"

void network_get_build_identifiers(int* executable_type, int* executable_version, int* compatible_version)
{
	*executable_type = 5;
	*executable_version = 11;
	*compatible_version = 11;
}