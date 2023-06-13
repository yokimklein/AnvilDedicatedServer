#pragma once
#include <cseries\cseries.h>

ulong fast_checksum(const void* key, size_t length, ulong initval); // hashlittle
ulong fast_checksum_new();