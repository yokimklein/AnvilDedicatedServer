#pragma once

// 4-character tag group identifier
typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

// 8-bit unsigned integer ranging from 0 to 255
typedef unsigned char uchar;
static_assert(sizeof(uchar) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
typedef unsigned short ushort;
static_assert(sizeof(ushort) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
typedef unsigned long ulong;
static_assert(sizeof(ulong) == 0x4);

// 64-bit signed integer ranging from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef long long long64;
static_assert(sizeof(long64) == 0x8);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
typedef unsigned long long ulong64;
static_assert(sizeof(ulong64) == 0x8);

// 32-character ascii string
typedef char string[32];
static_assert(sizeof(string) == 0x20);

// 256-character ascii string
typedef char long_string[256];
static_assert(sizeof(long_string) == 0x100);

// a 32-bit string identifier
typedef unsigned long string_id;
static_assert(sizeof(string_id) == 0x4);

// 8-bit unsigned integer ranging from 0 to 255
typedef unsigned char byte;
static_assert(sizeof(byte) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
typedef unsigned short word;
static_assert(sizeof(word) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
typedef unsigned long dword;
static_assert(sizeof(dword) == 0x4);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
typedef unsigned long long qword;
static_assert(sizeof(qword) == 0x8);

// 8-bit enumerator value
typedef char char_enum;
static_assert(sizeof(char_enum) == 0x1);

// 16-bit enumerator value
typedef short short_enum;
static_assert(sizeof(short_enum) == 0x2);

// 32-bit enumerator value
typedef long long_enum;
static_assert(sizeof(long_enum) == 0x4);

// 8-bit flags container
typedef byte byte_flags;
static_assert(sizeof(byte_flags) == 0x1);

// 16-bit flags container
typedef word word_flags;
static_assert(sizeof(word_flags) == 0x2);

// 32-bit flags container
typedef dword dword_flags;
static_assert(sizeof(dword_flags) == 0x4);

// 32-bit floating-point number ranging from 1.175494351e-38F to 3.402823466e+38F
typedef float real;
static_assert(sizeof(real) == 0x4);

// 64-bit floating-point number ranging from -7.2E+75 to 7.2E+75
typedef double real64;
static_assert(sizeof(real64) == 0x8);

typedef char utf8;
