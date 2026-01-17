#pragma once
#include "cseries\cseries.h"
#include "tag_files\files.h"
#include "tag_files\tag_field.h"
#include "memory\data.h"
#include "tag_files\tag_groups.h"
#include "cache\cache_file_builder_tag_resource_manager.h"

void* __fastcall tag_get(tag group_tag, datum_index tag_index);
tag tag_get_group_tag(datum_index tag_index);
long __fastcall cache_file_get_global_tag_index(tag group_tag);
const char* cache_file_get_build_number();

#define TAG_GET(GROUP, TYPE, INDEX) ((TYPE*)tag_get((GROUP), (INDEX)))
#define TAG_GET_SAFE(GROUP, TYPE, INDEX) ((INDEX) != NONE ? ((TYPE*)tag_get((GROUP), (INDEX))) : NULL)
#define TAG_BLOCK_GET_ELEMENT(BLOCK, INDEX, TYPE) ((TYPE*)tag_block_get_element_with_size((BLOCK), (INDEX), sizeof(TYPE)))

enum e_cache_file_shared_file_type
{
	_cache_file_shared_file_type_ui = 0,
	_cache_file_shared_file_type_resources,
	_cache_file_shared_file_type_textures,
	_cache_file_shared_file_type_audio,
	_cache_file_shared_file_type_textures_b,
	_cache_file_shared_file_type_video,
	_cache_file_shared_file_type_render_models,
	_cache_file_shared_file_type_lightmaps,

	k_number_of_cache_file_shared_file_types
};

enum : datum_index
{
	_datum_index_none = 0xFFFFFFFF
};

#pragma pack(push, 4)
union s_cache_file_header
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		tag header_signature;
		long version;
		long file_size;

		long compressed_file_padding;
		long tags_offset;
		ulong tag_buffer_offset;
		ulong total_tags_size;

		c_static_string<k_tag_long_string_length> path;
		c_static_string<k_tag_string_length> build_number;

		byte data_todo[0x3254];
	};
};
static_assert(sizeof(s_cache_file_header) == 0x3390);
static_assert(OFFSETOF(s_cache_file_header, path) == 0x1C);
static_assert(OFFSETOF(s_cache_file_header, build_number) == 0x11C);
#pragma pack(pop)

union cache_file_tag_instance
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		ulong checksum;
		ulong total_size;
		short dependency_count;
		short data_fixup_count;
		short resource_fixup_count;
		short : 16;

		// offset from `base`
		ulong offset;

		s_cache_file_tag_group tag_group;

		__pragma(warning(disable : 4200)) ulong dependencies[];
	};

	const char* get_name()
	{
		REFERENCE_DECLARE((ulong)(base + total_size), c_static_string<k_tag_long_string_length>, tag_name);
		return tag_name.get_string();
	}

	void* get()
	{
		return base + offset;
	}

	template<typename t_type>
	t_type* cast_to()
	{
		return static_cast<t_type*>(get());
	}
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

struct s_file_reference_persist
{
	tag signture;
	ushort flags;
	short location;
	char path[108];
	ulong handle;
	long position;
};
static_assert(sizeof(s_file_reference_persist) == 0x7C);

struct s_cache_file_report
{
	char __unknown0[32];
	ulong hash[5];
	s_file_reference_persist file_reference;
	ulong __unknownB0[20];
	ulong __unknown100;
	ulong __unknown104;
	ulong __unknown108;
	ulong __unknown10C;
	ulong __unknown110;
};
static_assert(sizeof(s_cache_file_report) == 0x114);

struct s_cache_file_reports
{
	long count;
	s_cache_file_report* elements;
};
static_assert(sizeof(s_cache_file_reports) == 0x8);

union s_cache_file_section_header
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		ulong __unknown0;
		long file_offsets;
		long file_count;
		ulong __unknownC;
		s_file_last_modification_date creation_date;
		ulong __unknown18;
		ulong __unknown1C;
	};
};
static_assert(sizeof(s_cache_file_section_header) == 0x20);

struct s_cache_file_globals
{
	bool tags_loaded;

	// physical_memory_malloc_fixed(sizeof(long) * header.tag_count)
	//c_static_array<long, k_tag_cache_maximum_files_count>& tag_cache_offsets;
	long* tag_cache_offsets;

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	//c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;
	//c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;
	cache_file_tag_instance** tag_instances;
	
	// tag_index_absolute_mapping[tag_index] = absolute_index;
	//c_static_array<long, k_tag_cache_maximum_files_count>& tag_index_absolute_mapping;
	long* tag_index_absolute_mapping;
	
	// absolute_index_tag_mapping[absolute_index] = tag_index;
	//c_static_array<long, k_tag_cache_maximum_files_count>& absolute_index_tag_mapping;
	long* absolute_index_tag_mapping;

	long tag_loaded_count;
	long tag_total_count;

	//byte(&tag_cache_base_address)[k_tag_cache_maximum_size];
	byte* tag_cache_base_address;
	ulong tag_loaded_size;
	ulong tag_cache_size; // k_tag_cache_maximum_size

	s_cache_file_header header;

	s_file_reference tags_section;

	s_cache_file_resource_gestalt* resource_gestalt;

	// resource_file_counts_mapping[resource_file_index] = resource_count;
	c_static_array<long, 7> resource_file_counts_mapping; // likely 7 now

	// $TODO: verify struct from this point down
	s_cache_file_reports reports;

	c_static_array<const char*, 7> resource_files;
	const char* map_directory;
};
static_assert(sizeof(s_cache_file_globals) == 0x3510);
static_assert(OFFSETOF(s_cache_file_globals, tag_instances) == 0x08);
static_assert(OFFSETOF(s_cache_file_globals, tag_index_absolute_mapping) == 0x0C);
static_assert(OFFSETOF(s_cache_file_globals, header) == 0x28);
static_assert(OFFSETOF(s_cache_file_globals, resource_gestalt) == 0x34C8);
static_assert(OFFSETOF(s_cache_file_globals, resource_file_counts_mapping) == 0x34CC);

union cache_address
{
	ulong value;
	struct
	{
		ulong offset : 30;
		ulong persistent : 1;
		ulong : 1;
	};
};
static_assert(sizeof(cache_address) == 0x4);

extern char const* k_cache_strings_file;
extern char const* k_cache_tags_file;
extern char const* k_cache_tag_list_file;
extern char const* k_cache_resources_file;
extern char const* k_cache_textures_file;
extern char const* k_cache_textures_b_file;
extern char const* k_cache_audio_file;
extern char const* k_cache_video_file;
extern char const* k_cache_lightmaps_file;
extern char const* k_cache_render_models_file;
extern char const* k_cache_file_extension;
extern char const* k_cache_path_format;

extern byte const g_cache_file_creator_key[64];

extern s_tag_reference g_last_tag_accessed;

extern s_cache_file_globals& g_cache_file_globals;

extern const char* tag_get_name(long tag_name_index);
extern s_cache_file_resource_gestalt* __fastcall cache_files_populate_resource_gestalt();
extern void cache_file_tags_fixup_all_resources(c_wrapped_array<ulong>& resource_offsets, s_cache_file_resource_gestalt* resource_gestalt);
extern void __fastcall cache_files_populate_resource_offsets(c_wrapped_array<ulong>* resource_offsets);
