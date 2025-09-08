#pragma once
#include "cseries\cseries.h"
#include "multithreading\synchronized_value.h"
#include "cseries\language.h"
#include "tag_files\files.h"

enum
{
	k_latest_font_package_header_version = 0xC0000003,

	k_font_package_file_size = 0x8000,
	k_font_package_entry_count = 8,
	k_font_count = 16,

	k_latest_font_header_version = 0xF0000005,
	k_font_header_kerning_pair_index_count = 256,
};

enum e_font_index
{
	_font_index_none = NONE,
	_font_index_fallback = -2,

	k_maximum_font_index_count = 10
};

#pragma pack(push, 1)
struct s_font_loading_state
{
	char debug_filename[32];
	e_font_index font_index;
	bool file_open;
	s_file_reference file_reference;
	byte pad0[0x3];
	long task_id;
	c_synchronized_long started;
	c_synchronized_long failed;
	c_synchronized_long finished;
};
static_assert(sizeof(s_font_loading_state) == 0x148);
#pragma pack(pop)

struct s_kerning_pair
{
	byte second_character;
	char offset;
};
static_assert(sizeof(s_kerning_pair) == 0x2);

struct s_font_package_entry
{
	ulong first_character_key;
	ulong last_character_key;
};
static_assert(sizeof(s_font_package_entry) == 0x8);

struct s_font_package_font
{
	long header_offset;
	long header_size;
	short package_table_index;
	short package_table_count;
};
static_assert(sizeof(s_font_package_font) == 0xC);

struct s_font_package_file_header
{
	long version;
	long font_count;
	s_font_package_font fonts[k_font_count];
	long font_mapping[k_font_count];
	long header_data_offset;
	long header_data_size;
	long package_table_offset;
	long package_table_count;
};
static_assert(sizeof(s_font_package_file_header) == 0x118);

struct s_font_character
{
	short character_width;
	word packed_size;
	short bitmap_height;
	short bitmap_width;
	short initial_offset;
	short bitmap_origin_y;
};
static_assert(sizeof(s_font_character) == 0xC);

struct s_font_header
{
	long version;
	char debug_name[32];
	short ascending_height;
	short descending_height;
	short leading_height;
	short leading_width;
	long kerning_pairs_offset;
	long kerning_pair_count;
	byte character_first_kerning_pair_index[k_font_header_kerning_pair_index_count];
	long location_table_offset;
	long location_table_count;
	long character_count;
	long character_data_offset;
	long character_data_size_bytes;
	ulong no_such_character_data_location;
	long maximum_packed_pixel_size_bytes;
	long maximum_unpacked_pixel_size_bytes;
	long total_packed_pixel_size_bytes;
	long total_unpacked_pixel_size_bytes;
};
static_assert(sizeof(s_font_header) == 0x15C);

struct s_font_package_file
{
	s_font_package_file_header header;
	s_font_header font_headers[k_font_count];
	byte data[0x6928];
};
static_assert(sizeof(s_font_package_file) == k_font_package_file_size);

struct s_font_globals
{
	bool initialized;
	bool cached_to_hard_drive;
	bool emergency_mode;
	bool fonts_unavailable;
	e_language language;
	long failure_retry_count;
	c_synchronized_long async_error;
	s_font_loading_state package_loading_state;
	const s_font_package_file_header* font_package_header;

	//byte header_storage[k_font_package_file_size];
	s_font_package_file package_file;
};
static_assert(sizeof(s_font_globals) == 0x815C);

enum e_font_id;
extern const s_font_header* font_get_header_internal(e_font_index internal_index);
extern e_font_index font_get_font_index(e_font_id font);
extern const s_font_header* font_get_header(e_font_id font);
extern void font_load_idle(s_font_loading_state* loading_state, bool* out_failure_reported);
extern bool font_in_emergency_mode();
extern const s_font_package_file_header* font_get_package_header_internal();
extern void __fastcall font_initialize_emergency();

extern s_font_globals& g_font_globals;