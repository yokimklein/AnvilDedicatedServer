#pragma once

#define INTEGER_RANDOM(seed, str) _random(seed, str, __FILE__, __LINE__)
#define INTEGER_RANDOM_RANGE(seed, lower_bound, delta) ((lower_bound) + (((delta) * INTEGER_RANDOM(seed)) >> 16))

dword get_local_random_seed();
extern dword* get_local_random_seed_address();
extern word _random(ulong* seed, const char* string, const char* file, ulong line);