#pragma once
#include "text\font_cache.h"

enum e_font_package_status
{
	_font_package_unavailable,
	_font_package_delayed_loading,
	_font_package_loading,
	_font_package_ready,

	k_font_package_status_count,
};

struct s_font_package
{
	word table_offset;
	word table_count;
	word data_offset;
	word data_size;
};

struct s_font_package_file_header;
e_font_package_status __fastcall font_package_get(long package_index, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count> flags, ulong character_key, const s_font_package** out_package);
const s_font_character* __fastcall font_package_get_character(const s_font_package* font_package, ulong character_key);
long font_package_table_find_character(const s_font_package_file_header* package_header, ulong character_key);
void __fastcall font_package_cache_new();