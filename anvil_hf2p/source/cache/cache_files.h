#pragma once
#include "cseries\cseries.h"
#include "tag_files\files.h"
#include "tag_files\tag_field.h"
#include "memory\data.h"
#include "tag_files\tag_groups.h"
#include "cache\cache_file_builder_tag_resource_manager.h"
#include "cache\cache_file_tag_resource_runtime.h"
#include "cseries\language.h"
#include "memory\secure_signature.h"

void* __fastcall tag_get(tag group_tag, datum_index tag_index);
tag tag_get_group_tag(datum_index tag_index);
long __fastcall cache_file_get_global_tag_index(tag group_tag);
const char* cache_file_get_build_number();

#define TAG_GET(GROUP, TYPE, INDEX) ((TYPE*)tag_get((GROUP), (INDEX)))
#define TAG_GET_SAFE(GROUP, TYPE, INDEX) ((INDEX) != NONE ? ((TYPE*)tag_get((GROUP), (INDEX))) : NULL)
#define TAG_BLOCK_GET_ELEMENT(BLOCK, INDEX, TYPE) ((TYPE*)tag_block_get_element_with_size((BLOCK), (INDEX), sizeof(TYPE)))

enum e_cache_file_shared_file_type
{
	_cache_file_shared_file_type_ui = 0, // mainmenu, map headers store the tags.dat creation date in this slot
	_cache_file_shared_file_type_resources,
	_cache_file_shared_file_type_textures,
	_cache_file_shared_file_type_textures_b,
	_cache_file_shared_file_type_audio,
	_cache_file_shared_file_type_video,
	_cache_file_shared_file_type_render_models,
	_cache_file_shared_file_type_lightmaps,

	k_number_of_cache_file_shared_file_types
};

enum e_cache_file_section
{
	_cache_file_debug_section = 0,
	_cache_file_resource_section,
	_cache_file_tag_section,
	_cache_file_language_pack_section,

	k_number_of_cache_file_sections
};

constexpr long k_cache_file_header_signature = 'head';
constexpr long k_cache_file_footer_signature = 'foot';
constexpr long k_cache_file_version = 18;

struct s_cache_file_section_file_bounds
{
	long offset;
	long size;
};
static_assert(sizeof(s_cache_file_section_file_bounds) == 0x8);

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

		short scenario_type;
		short shared_cache_file_type;

		bool uncompressed;
		bool tracked_build;
		bool valid_shared_resource_usage;
		byte header_flags;

		s_file_last_modification_date slot_modification_date;

		long low_detail_texture_number;
		ulong low_detail_texture_offset;
		ulong low_detail_texture_byte_count;

		long string_id_count;
		long string_id_data_count;
		ulong string_id_index_offset;
		ulong string_id_data_offset;

		// _cache_file_shared_file_type_resources also makes tags load from maps\tags.dat instead of the map
		c_flags<e_cache_file_shared_file_type, byte, k_number_of_cache_file_shared_file_types> shared_file_flags;

		s_file_last_modification_date creation_date;
		c_static_array<s_file_last_modification_date, k_number_of_cache_file_shared_file_types> shared_creation_date;

		c_static_string<k_tag_string_length> name;
		c_enum<e_language, long, _language_invalid, k_language_count> language;
		c_static_string<k_tag_long_string_length> tag_path;
		long minor_version_number;

		long debug_tag_name_count;
		ulong debug_tag_name_data_offset;
		long debug_tag_name_data_size;
		ulong debug_tag_name_index_offset;

		// halo online uses this for the tag patch records
		s_cache_file_section_file_bounds reports;

		byte __data2F4[0x4];

		// the cache builder's windows user name, xored with a fixed 32-byte key (05116aa3cab507df50e75b756b4abbf4e8548fc6d6cc921597dcf5eeb93c013c)
		// decodes to "builder" in every halo online map from ms23 to ms30
		c_static_string<k_tag_string_length> creator_name;

		byte __data318[0x10];

		qword signature_marker;

		s_network_http_request_hash content_hashes[1];
		s_rsa_signature rsa_signature;

		c_static_array<long, k_number_of_cache_file_sections> section_offsets;
		c_static_array<s_cache_file_section_file_bounds, k_number_of_cache_file_sections> original_section_bounds;

		s_cache_file_shared_resource_usage shared_resource_usage;

		long tag_cache_offsets;
		long tag_count;

		e_map_id map_id;
		long scenario_index;
		long cache_file_resource_gestalt_index;

		byte padding[0x584];

		tag footer_signature;
	};
};
static_assert(sizeof(s_cache_file_header) == 0x3390);
static_assert(OFFSETOF(s_cache_file_header, path) == 0x1C);
static_assert(OFFSETOF(s_cache_file_header, build_number) == 0x11C);
static_assert(OFFSETOF(s_cache_file_header, scenario_type) == 0x13C);
static_assert(OFFSETOF(s_cache_file_header, shared_file_flags) == 0x168);
static_assert(OFFSETOF(s_cache_file_header, creation_date) == 0x16C);
static_assert(OFFSETOF(s_cache_file_header, shared_creation_date) == 0x174);
static_assert(OFFSETOF(s_cache_file_header, name) == 0x1B4);
static_assert(OFFSETOF(s_cache_file_header, tag_path) == 0x1D8);
static_assert(OFFSETOF(s_cache_file_header, reports) == 0x2EC);
static_assert(OFFSETOF(s_cache_file_header, signature_marker) == 0x328);
static_assert(OFFSETOF(s_cache_file_header, content_hashes) == 0x330);
static_assert(OFFSETOF(s_cache_file_header, rsa_signature) == 0x344);
static_assert(OFFSETOF(s_cache_file_header, section_offsets) == 0x444);
static_assert(OFFSETOF(s_cache_file_header, original_section_bounds) == 0x454);
static_assert(OFFSETOF(s_cache_file_header, shared_resource_usage) == 0x474);
static_assert(OFFSETOF(s_cache_file_header, tag_cache_offsets) == 0x2DF4);
static_assert(OFFSETOF(s_cache_file_header, map_id) == 0x2DFC);
static_assert(OFFSETOF(s_cache_file_header, scenario_index) == 0x2E00);
static_assert(OFFSETOF(s_cache_file_header, cache_file_resource_gestalt_index) == 0x2E04);
static_assert(OFFSETOF(s_cache_file_header, footer_signature) == 0x338C);
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

// halo online reuses the report slot for tag patch records, applied to tags with values from title storage
struct s_cache_file_report
{
	char key[256]; // 32 lowercase hex characters
	long tag_index;
	ulong address; // 0x40000000 + offset from the tag instance header
	long size;
	long field_type;
	long element;
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

#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
enum
{
	k_tag_cache_maximum_files_count = 35000,
	k_cache_file_maximum_resource_count = 32767,
};
#endif

struct s_cache_file_globals
{
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
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
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
	// ms30 stores the tag tables inline instead of pointing at allocations
	byte* tag_cache_base_address;
	ulong tag_loaded_size;
	ulong tag_cache_size; // k_tag_cache_maximum_size
	long tag_loaded_count;

	// tag_index_absolute_mapping[tag_index] = absolute_index;
	c_static_array<long, k_tag_cache_maximum_files_count> tag_index_absolute_mapping;

	// absolute_index_tag_mapping[absolute_index] = tag_index;
	c_static_array<long, k_tag_cache_maximum_files_count> absolute_index_tag_mapping;

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count> tag_instances;

	// tag_cache_offsets[tag_index] = offset of the tag in the tags section
	c_static_array<long, k_tag_cache_maximum_files_count> tag_cache_offsets;

	// physical_memory_malloc_fixed(k_tag_cache_maximum_size), copied to tag_cache_base_address when tags are loaded
	byte* tag_cache_allocation;

	// filled by cache_files_populate_resource_offsets
	c_static_array<ulong, k_cache_file_maximum_resource_count> resource_offsets;

	// resource pointers kept by cache_files_populate_resource_gestalt for resident tags
	c_static_array<void*, k_cache_file_maximum_resource_count> resident_resources;

	// tags kept loaded across scenario_tags_load calls
	long resident_tag_loaded_count;
	ulong resident_tag_loaded_size;
	bool resident_tags_valid;
	bool resident_tags_loaded;

	bool tags_loaded;
	byte : 8;

	long tag_total_count;
	long : 32;
#endif

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
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
static_assert(sizeof(s_cache_file_globals) == 0x3510);
static_assert(OFFSETOF(s_cache_file_globals, tag_instances) == 0x08);
static_assert(OFFSETOF(s_cache_file_globals, tag_index_absolute_mapping) == 0x0C);
static_assert(OFFSETOF(s_cache_file_globals, header) == 0x28);
static_assert(OFFSETOF(s_cache_file_globals, resource_gestalt) == 0x34C8);
static_assert(OFFSETOF(s_cache_file_globals, resource_file_counts_mapping) == 0x34CC);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
static_assert(sizeof(s_cache_file_globals) == 0xCC088);
static_assert(OFFSETOF(s_cache_file_globals, tag_loaded_count) == 0x0C);
static_assert(OFFSETOF(s_cache_file_globals, tag_index_absolute_mapping) == 0x10);
static_assert(OFFSETOF(s_cache_file_globals, absolute_index_tag_mapping) == 0x222F0);
static_assert(OFFSETOF(s_cache_file_globals, tag_instances) == 0x445D0);
static_assert(OFFSETOF(s_cache_file_globals, tag_cache_offsets) == 0x668B0);
static_assert(OFFSETOF(s_cache_file_globals, tag_cache_allocation) == 0x88B90);
static_assert(OFFSETOF(s_cache_file_globals, resource_offsets) == 0x88B94);
static_assert(OFFSETOF(s_cache_file_globals, resident_resources) == 0xA8B90);
static_assert(OFFSETOF(s_cache_file_globals, resident_tag_loaded_count) == 0xC8B8C);
static_assert(OFFSETOF(s_cache_file_globals, tags_loaded) == 0xC8B96);
static_assert(OFFSETOF(s_cache_file_globals, tag_total_count) == 0xC8B98);
static_assert(OFFSETOF(s_cache_file_globals, header) == 0xC8BA0);
static_assert(OFFSETOF(s_cache_file_globals, resource_gestalt) == 0xC8BA0 + 0x34A0);
static_assert(OFFSETOF(s_cache_file_globals, resource_file_counts_mapping) == 0xC8BA0 + 0x34A4);
#endif

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
