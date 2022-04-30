#pragma once
#include <cstdint>

void network_get_build_identifiers(long* executable_type, long* executable_version, long* compatible_version);
uint32_t network_get_time();