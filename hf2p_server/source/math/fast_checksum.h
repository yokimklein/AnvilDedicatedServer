#pragma once
#include <windows.h>

const static DWORD k_network_session_shared_membership_seed = 0x21517881;

DWORD fast_checksum_new();

template<DWORD k_seed, typename t_data_type, size_t k_chunk_size = sizeof(t_data_type) / 12>
DWORD fast_checksum(DWORD a1, t_data_type* data)
{
	const void* a2 = data;

	int v2; // eax
	unsigned int v3; // er9
	DWORD v4; // er10
	DWORD v5; // er11
	unsigned int v6; // eax
	DWORD v7; // er10
	int v8; // er8
	int v9; // er11
	int v10; // ecx
	DWORD v11; // eax
	unsigned int v12; // er11
	int v13; // er10
	unsigned int v14; // ecx
	DWORD v15; // er11
	unsigned int v16; // er10
	int v17; // eax
	unsigned int v18; // ecx
	DWORD v19; // er10
	unsigned int v20; // eax
	int v21; // er11
	unsigned int v22; // ecx
	DWORD v23; // eax
	unsigned int v24; // er11
	int v25; // er10
	unsigned int v26; // ecx
	unsigned int v27; // er10
	int v28; // eax
	int v29; // ecx
	unsigned int v30; // eax
	unsigned int v31; // er9
	unsigned int v32; // er10
	unsigned int v33; // eax
	unsigned int v34; // er8
	unsigned int v35; // er10

	v2 = a1 - k_seed;
	v3 = 0;
	v4 = a1 - k_seed;
	v5 = a1 - k_seed;
	do
	{
		v6 = *((DWORD*)a2 + 2) + v2;
		++v3;
		v7 = *((DWORD*)a2 + 1) + v4;
		v8 = *(DWORD*)a2;
		a2 = (char*)a2 + 12;
		v9 = (v6 >> 28) ^ (v8 - v6 + v5);
		v10 = 16 * v6;
		v11 = v7 + v6;
		v12 = v10 ^ v9;
		v13 = (v12 >> 26) ^ (v7 - v12);
		v14 = v12 << 6;
		v15 = v11 + v12;
		v16 = v14 ^ v13;
		v17 = HIBYTE(v16) ^ (v11 - v16);
		v18 = v16 << 8;
		v19 = v15 + v16;
		v20 = v18 ^ v17;
		v21 = HIWORD(v20) ^ (v15 - v20);
		v22 = v20 << 16;
		v23 = v19 + v20;
		v24 = v22 ^ v21;
		v25 = (v24 << 19) ^ (v19 - v24);
		v26 = v24 >> 13;
		v5 = v23 + v24;
		v27 = v26 ^ v25;
		v28 = (v27 >> 28) ^ (v23 - v27);
		v29 = 16 * v27;
		v4 = v5 + v27;
		v2 = v29 ^ v28;
	} while (v3 < k_chunk_size);
	v30 = (v4 ^ v2) - ((v4 << 14) ^ ((unsigned __int64)v4 >> 18));
	v31 = ((*(DWORD*)a2 + v5) ^ v30) - ((v30 << 11) ^ (v30 >> 21));
	v32 = (v31 ^ v4) - ((v31 >> 7) ^ (v31 << 25));
	v33 = (v32 ^ v30) - ((v32 << 16) ^ ((unsigned __int64)v32 >> 16));
	v34 = (v31 ^ v33) - ((16 * v33) ^ (v33 >> 28));
	v35 = (v34 ^ v32) - ((v34 << 14) ^ (v34 >> 18));
	return (v35 ^ v33) - ((v35 >> 8) ^ (v35 << 24));
}