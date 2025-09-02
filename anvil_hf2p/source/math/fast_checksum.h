#pragma once
#include <cseries\cseries.h>
#include <lookup3.c>

/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.
https://burtleburtle.net/bob/c/lookup3.c
-------------------------------------------------------------------------------
*/

template<typename T>
ulong fast_checksum(ulong initval, const T* key)
{
    return hashlittle(key, sizeof(T), initval);
}

ulong fast_checksum_new() { return -1; };