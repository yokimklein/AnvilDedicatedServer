#include "random_math.h"
#include "cseries\cseries.h"
#include <windows.h>

REFERENCE_DECLARE(0x4EBF024, dword, random_seed_local);

dword get_local_random_seed()
{
    return *get_local_random_seed_address();
}

dword* get_local_random_seed_address()
{
    return &random_seed_local;
}

word _random(ulong* seed, const char* string, const char* file, ulong line)
{
	//random_seed_debug(seed, "random", string, file, line);
	*seed = 0x19660D * *seed + 0x3C6EF35F;

	return HIWORD(*seed);
}
