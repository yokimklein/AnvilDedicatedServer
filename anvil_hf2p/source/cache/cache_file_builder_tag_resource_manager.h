#pragma once
#include "cseries\cseries.h"
#include "cache\cache_file_tag_resource_location_table.h"
#include "tag_files\tag_groups.h"
#include "tag_files\tag_resource_internals.h"
#include <scenario\scenario_map_variant.h>

enum e_cache_file_resource_type
{
	_cache_file_resource_type_structure_bsp_cache_file_tag_resources = 0,
	_cache_file_resource_type_bitmap_texture_interop_resource,
	_cache_file_resource_type_bitmap_texture_interleaved_interop_resource,
	_cache_file_resource_type_sound_resource_definition,
	_cache_file_resource_type_model_animation_tag_resource,
	_cache_file_resource_type_render_geometry_api_resource_definition,
	_cache_file_resource_type_bink_resource,
	_cache_file_resource_type_structure_bsp_tag_resources,

	k_cache_file_resource_type_count
};

enum e_cache_file_resource_data_flags
{
	_cache_file_resource_data_flags_has_pageable_data = 0,
	_cache_file_resource_data_flags_has_optional_data,

	k_cache_file_resource_data_flags
};

// $TODO: Needs updating for render_models.dat & lightmaps.dat
enum e_cache_file_tag_resource_location_flags
{
	_cache_file_tag_resource_location_flags_valid_checksum = 0,
	_cache_file_tag_resource_location_flags_resources,
	_cache_file_tag_resource_location_flags_textures,
	_cache_file_tag_resource_location_flags_textures_b,
	_cache_file_tag_resource_location_flags_audio,
	_cache_file_tag_resource_location_flags_video,

	//_cache_file_tag_resource_location_flags_render_models,
	//_cache_file_tag_resource_location_flags_lightmaps,
	// likely replaces the unused entry?

	_cache_file_tag_resource_location_flags_unused,
	_cache_file_tag_resource_location_flags_only_full_valid_checksum,

	k_cache_file_tag_resource_location_flags,

	k_cache_file_tag_resource_location_mask
	= FLAG(_cache_file_tag_resource_location_flags_resources)
	| FLAG(_cache_file_tag_resource_location_flags_textures)
	| FLAG(_cache_file_tag_resource_location_flags_textures_b)
	| FLAG(_cache_file_tag_resource_location_flags_audio)
	| FLAG(_cache_file_tag_resource_location_flags_video)
	//| FLAG(_cache_file_tag_resource_location_flags_render_models)
	//| FLAG(_cache_file_tag_resource_location_flags_lightmaps)
};

struct s_cache_file_resource_checksum
{
	ulong crc_value;
};

struct s_cache_file_resource_file_location
{
	// runtime
	short resource_handle_salt;

	c_flags<e_cache_file_tag_resource_location_flags, byte, k_cache_file_tag_resource_location_flags> flags;
	char codec_index; // e_cache_file_compression_codec

	long unknown; // new since ms23

	union
	{
		// persistent file index, dependant on what location flag is active
		long shared_file_location_index;

		// runtime combined offset, at runtime all resource files are a combined buffer
		ulong file_offset;
	};

	ulong file_size;
	ulong size;
	s_cache_file_resource_checksum checksum;
	s_cache_file_resource_streaming_sublocation_table streaming_sublocation_table;
};
static_assert(sizeof(s_cache_file_resource_file_location) == 0x28);
static_assert(OFFSETOF(s_cache_file_resource_file_location, shared_file_location_index) == 0x08);
static_assert(OFFSETOF(s_cache_file_resource_file_location, file_offset) == 0x08);
static_assert(OFFSETOF(s_cache_file_resource_file_location, size) == 0x10);

struct s_render_texture_format
{
	short width;
	short height;
	char depth;
	char mipmap_count_including_highest;
	char bitmap_type;
	char is_high_res_bitmap;
	ulong xenon_d3d_format;
	char bm_format;
	char bm_curve;
	ushort bm_flags;
};
static_assert(sizeof(s_render_texture_format) == 0x10);

struct s_render_texture_descriptor
{
	s_tag_data base_pixel_data;
	s_tag_data high_res_pixel_data;
	s_render_texture_format texture;
};
static_assert(sizeof(s_render_texture_descriptor) == 0x38);

union control_data_t
{
	// $TODO: add other control data structures
	s_render_texture_descriptor render_texture;
};

struct s_cache_file_resource_fixup_location
{
	long encoded_fixup_location;
	long encoded_fixup_value;
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

struct s_tag_resource_interop_location
{
	c_tag_resource_fixup encoded_interop_location;
	long interop_type_index; // long_block_index
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

struct s_cache_file_resource_runtime_data_new
{
	s_tag_reference owner_tag;
	short resource_salt;
	c_enum<e_cache_file_resource_type, char, _cache_file_resource_type_structure_bsp_cache_file_tag_resources, k_cache_file_resource_type_count> resource_type;
	char control_alignment_bits;
	c_typed_tag_data<control_data_t> control_data;
	c_tag_resource_fixup root_fixup;
	c_typed_tag_block<s_cache_file_resource_fixup_location> control_fixups;
	c_typed_tag_block<s_tag_resource_interop_location> interop_locations;
	c_flags<e_cache_file_resource_data_flags, ulong, k_cache_file_resource_data_flags> flags;
};
static_assert(sizeof(s_cache_file_resource_runtime_data_new) == 0x48);

struct s_cache_file_tag_resource_data
{
	s_cache_file_resource_file_location file_location;
	s_cache_file_resource_runtime_data_new runtime_data;
};
static_assert(sizeof(s_cache_file_tag_resource_data) == 0x70);

struct s_cache_file_tag_parentage
{
	s_tag_reference child_tag;
	ushort flags;
	short resource_owner_index;
	c_typed_tag_block<long> parent_indices; // s_cache_file_tag_parentage block indices
	c_typed_tag_block<long> child_indices;  // s_cache_file_tag_parentage block indices
};
static_assert(sizeof(s_cache_file_tag_parentage) == 0x2C);

struct s_tag_resource_prediction_quantum;
struct s_tag_resource_prediction_atom;
struct s_tag_resource_prediction_molecule;
struct s_tag_resource_prediction_molecule_key;
struct s_tag_resource_cache_prediction_table
{
	c_typed_tag_block<s_tag_resource_prediction_quantum> quanta;
	c_typed_tag_block<s_tag_resource_prediction_atom> atoms;
	c_typed_tag_block<long> molecule_atoms;
	c_typed_tag_block<s_tag_resource_prediction_molecule> molecules;
	c_typed_tag_block<s_tag_resource_prediction_molecule_key> molecule_keys;
};
static_assert(sizeof(s_tag_resource_cache_prediction_table) == 0x3C);

struct s_cache_file_zone_manifest;
struct s_cache_file_resource_gestalt
{
	c_typed_tag_block<s_cache_file_tag_resource_data*> resources;
	ulong resources_size;
	ulong shared_files_available;

	// $TODO: map the rest of this struct
	char data[0x15C];
	//c_typed_tag_block<s_cache_file_zone_manifest> designer_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> global_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> unattached_designer_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> dvd_forbidden_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> dvd_always_streaming_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> default_bsp_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> static_bsp_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> dynamic_bsp_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> cinematic_zone_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> zones_only_zone_set_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> expected_zone_set_manifests;
	//c_typed_tag_block<s_cache_file_zone_manifest> fully_populated_zone_set_manifests;
	//
	//// something removed around here since ms23 (12 bytes)
	//s_tag_data naive_control_data;
	//ulong minimum_complete_pageable_data_size;
	//ulong minimum_required_pageable_data_size;
	//ulong minimum_required_dvd_data_size;
	//ulong global_pageable_data_size;
	//ulong optional_control_data_size;
	//
	//s_tag_block global_resource_usage;
	//s_tag_block bsp_game_attachments;
	//s_tag_block debug_resources_per_model_variant;
	//s_tag_block debug_resources_per_combat_dialogue;
	//s_tag_block debug_resources_per_tag;
	//s_tag_block resource_debug_definitions;
	//s_tag_block resource_layouts;
	//s_tag_block resource_properties;
	//
	//c_typed_tag_block<s_cache_file_tag_parentage> parentages;
	//s_tag_resource_cache_prediction_table prediction_table;
	//
	//// Mat is in a really bad mood
	//e_campaign_id campaign_id;
	//
	//// Next time we don't put things that the game depends on outside of tool, guerilla, or sapien
	//e_map_id map_id;
	//
	////s_cache_file_tag_resource_data* resource_blocks[];
};
static_assert(sizeof(s_cache_file_resource_gestalt) == 0x170); // 0x0C smaller than ms23's
static_assert(OFFSETOF(s_cache_file_resource_gestalt, resources) == 0x00);
static_assert(OFFSETOF(s_cache_file_resource_gestalt, resources_size) == 0x0C);
static_assert(OFFSETOF(s_cache_file_resource_gestalt, shared_files_available) == 0x10);
//static_assert(OFFSETOF(s_cache_file_resource_gestalt, resource_blocks) == 0x170);
