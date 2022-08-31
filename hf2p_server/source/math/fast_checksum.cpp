#include "fast_checksum.h"
#include <iostream>
#include <cassert>
#include "lookup3.c"

/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.
https://burtleburtle.net/bob/c/lookup3.c
-------------------------------------------------------------------------------
*/

uint32_t fast_checksum(const void* key, size_t length, uint32_t initval)
{
    return hashlittle(key, length, initval);
}

uint32_t fast_checksum_new()
{
    return -1;
}
