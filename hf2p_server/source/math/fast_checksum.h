#pragma once
#include <windows.h>
#include <stdint.h>

uint32_t fast_checksum(const void* key, size_t length, uint32_t initval); // hashlittle
uint32_t fast_checksum_new();