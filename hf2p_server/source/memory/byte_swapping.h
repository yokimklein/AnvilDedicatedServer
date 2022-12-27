#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/memory/byte_swapping.hpp

#include <intrin.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)