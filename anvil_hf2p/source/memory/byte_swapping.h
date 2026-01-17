#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/memory/byte_swapping.hpp

#include <intrin.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)

#define bswap_uns16(value) _byteswap_ushort(value)
#define bswap_uns32(value) _byteswap_ulong(value)
#define bswap_uns64(value) _byteswap_uint64(value)
#define bswap_uns16_inplace(value) value = bswap_uns16(value)
#define bswap_uns32_inplace(value) value = bswap_uns32(value)
#define bswap_uns64_inplace(value) value = bswap_uns64(value)