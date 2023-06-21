#include "fast_checksum.h"
#include <lookup3.c>

/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.
https://burtleburtle.net/bob/c/lookup3.c
-------------------------------------------------------------------------------
*/

ulong fast_checksum(const void* key, size_t length, ulong initval)
{
    return hashlittle(key, length, initval);
}

ulong fast_checksum_new()
{
    return -1;
}
