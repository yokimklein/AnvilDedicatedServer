#pragma once
#include "cseries\cseries.h"

enum
{
	k_subpart_bitvector_size_in_bits = 16384,

	k_maximum_region_memory_items = 512,
	k_maximum_item_markers = 6,
	k_maximum_sky_items = 6,
};

template<typename t_type, word k_maximum_count>
class c_simple_list
{
public:
	c_simple_list()
	{
		m_maximum_count = k_maximum_count;
	}

	long add()
	{
		if (m_count >= m_maximum_count)
			return NONE;

		return m_count++;
	}

	void clear()
	{
		m_count = 0;
	}

	long get_count()
	{
		return m_count;
	}

	t_type* list_iterator_next(long* iterator)
	{
		if (*iterator >= m_count - 1)
		{
			*iterator = NONE;
			return NULL;
		}

		return data[++*iterator];
	}

	t_type* list_iterator_new(long* iterator, long starting_index)
	{
		*iterator = starting_index - 1;

		return list_iterator_next(iterator);
	}

	void set_count(word count)
	{
		ASSERT(count <= m_maximum_count);
		m_count = count;
	}

	bool valid(long index)
	{
		return index >= 0 && index < m_count;
	}

	t_type& operator[](long index)
	{
		ASSERT(valid(index));
		return data[index];
	}

	word m_maximum_count;
	word m_count;

	t_type data[k_maximum_count];
};
static_assert(sizeof(c_simple_list<long, 6>) == 0x1C);

struct s_zone_cluster
{
	s_cluster_reference cluster_reference;
	short zone_index;
};
static_assert(sizeof(s_zone_cluster) == 0x4);

struct s_lod_transparency
{
	byte model_alpha;
	byte instance_alpha;
	byte shadow_alpha;
	byte unused_alpha;
};
static_assert(sizeof(s_lod_transparency) == 0x4);

struct s_visible_object_hierarchy
{
	short flags;
	word region_cluster_bitvector_start_index;
	long first_visibility_object_index;
	word visibility_object_index_count;
	s_lod_transparency lod_transparency;
};
static_assert(sizeof(s_visible_object_hierarchy) == 0x10);

struct s_shader_extern_info;
struct s_render_object_info
{
	long region_count;
	long render_model_index;
	long skinning_memory_designator;
	byte skinning_matrix_count;
	byte region_mesh_indices[16];
	word region_z_sort_offset_enum_index[16];
	word __unknown3E[16];
	short lod_index;
	short lightmap_object_index;
	ulong clip_plane_masks;
	s_shader_extern_info* render_info;
};
static_assert(sizeof(s_render_object_info) == 0x6C);

struct s_visible_object_render_visibility
{
	s_render_object_info info;
	long object_index;
	ulong* subpart_bitvector;
	byte flags;
};
static_assert(sizeof(s_visible_object_render_visibility) == 0x78);

struct s_visible_instance_list
{
	s_zone_cluster cluster;
	word instance_list_index;
	word first_visible_instance_list_instance_index;
	word visible_instance_list_count;
	word mesh_index;
};
static_assert(sizeof(s_visible_instance_list) == 0xC);

struct s_visible_instances
{
	short flags;
	word structure_bsp_instance_index;
	short structure_bsp_index;
	word region_cluster_bitvector_start_index;
	byte alpha_byte;
	ulong* part_bitvector;
};
static_assert(sizeof(s_visible_instances) == 0x10);

struct s_visible_clusters
{
	short flags;
	s_zone_cluster cluster;
	word region_cluster_index;
	word mesh_index;
	ulong* part_bitvector;
};
static_assert(sizeof(s_visible_clusters) == 0x10);

struct s_visible_items
{
	c_simple_list<s_visible_object_hierarchy, 768> root_objects;    // address: 0x018BF6E8, offset: 0x00000
	c_simple_list<s_visible_object_render_visibility, 896> objects; // address: 0x018C26EC, offset: 0x30040
	c_simple_list<s_visible_instance_list, 1024> instance_list;     // address: 0x018DCAF0, offset: 0x1D408
	c_simple_list<s_visible_instances, 1152> instances;             // address: 0x018DFAF4, offset: 0x2040C
	c_simple_list<s_visible_clusters, 348> clusters;                // address: 0x018E42F8, offset: 0x24C10
	c_simple_list<long, 50> lights;                                // address: 0x018E58BC, offset: 0x261D4
	c_simple_list<long, 6> skies;                                  // address: 0x018E5988, offset: 0x262A0
	c_static_flags<16384> visible_subpart_bitvector;                // address: 0x018E59A4, offset: 0x262BC
	word visible_subpart_bitvector_count;                          // address: 0x018E61A4, offset: 0x26ABC
	word visiblity_region_cluster_bitvector_count;                 // address: 0x018E61A6, offset: 0x26ABE
	c_static_flags<65536> visibility_region_cluster_bitvector;      // address: 0x018E61A8, offset: 0x26AC0
	word __unknown28AC0[2048];                                     // address: 0x018E81A8, offset: 0x28AC0
};
static_assert(sizeof(s_visible_items) == 0x29AC0);

class c_visible_items
{
public:
	struct s_marker_indices
	{
		word root_objects_starting_index;
		word objects_starting_index;
		word instance_list_starting_index;
		word instances_starting_index;
		word cluster_starting_index;
		word light_starting_index;
		word visible_subpart_bitvector_count;
		word visiblity_region_cluster_bitvector_count;
		word sky_starting_index;
	};

	static void __cdecl clear();
	static void __cdecl clear_all();

	static long __cdecl get_camera_root_objects_count();
	static word __cdecl get_cluster_starting_index();
	static word __cdecl get_instance_list_starting_index();
	static word __cdecl get_instances_starting_index();
	static word __cdecl get_light_starting_index();
	static word __cdecl get_objects_starting_index();
	static word __cdecl get_root_objects_starting_index();
	static word __cdecl get_sky_starting_index();
	static void __cdecl pop_marker();
	static void __cdecl push_marker();

	//private:
	static long& m_marker_count;
	static s_visible_items& m_items;
	static s_marker_indices(&m_marker_indices)[k_maximum_item_markers];
};

struct visibility_volume
{
	byte vector_planes[0x10 * 6];
	//__m128 vector_planes[6];

	short projection_index;
	short region_cluster_index;
	long plane_index;
	short plane_bsp_index;
	short parent_volume_index;
	real_rectangle2d frustum_bounds;
	real portal_nearest_z;
	real_point3d world_vertices[5];
	real_rectangle3d world_bounds;
	real_plane3d world_planes[6];
	real_vector3d world_edge_vectors[4];
};
static_assert(sizeof(visibility_volume) == 0x164);
static_assert(0x000 == OFFSETOF(visibility_volume, vector_planes));
static_assert(0x060 == OFFSETOF(visibility_volume, projection_index));
static_assert(0x062 == OFFSETOF(visibility_volume, region_cluster_index));
static_assert(0x064 == OFFSETOF(visibility_volume, plane_index));
static_assert(0x068 == OFFSETOF(visibility_volume, plane_bsp_index));
static_assert(0x06A == OFFSETOF(visibility_volume, parent_volume_index));
static_assert(0x06C == OFFSETOF(visibility_volume, frustum_bounds));
static_assert(0x07C == OFFSETOF(visibility_volume, portal_nearest_z));
static_assert(0x080 == OFFSETOF(visibility_volume, world_vertices));
static_assert(0x0BC == OFFSETOF(visibility_volume, world_bounds));
static_assert(0x0D4 == OFFSETOF(visibility_volume, world_planes));
static_assert(0x134 == OFFSETOF(visibility_volume, world_edge_vectors));

struct visibility_projection
{
	bool parallel_projection_flag;
	byte pad0[0x3];
	real_matrix4x3 world_to_basis;
	real_matrix4x3 basis_to_world;
	bool near_bounded_flag;
	byte pad1[0x3];
	real near_distance;
	real_plane3d near_plane;
	bool far_bounded_flag;
	bool far_bounded_spherical_flag;
	byte pad2[0x2];
	real far_distance;
	bool volume_bounded_flag;
	byte pad3[0x3];
	visibility_volume volume;
	long convex_hull_point_count;
	real_point2d convex_hull_points[4];
};
static_assert(sizeof(visibility_projection) == 0x218);
static_assert(0x000 == OFFSETOF(visibility_projection, parallel_projection_flag));
static_assert(0x001 == OFFSETOF(visibility_projection, pad0));
static_assert(0x004 == OFFSETOF(visibility_projection, world_to_basis));
static_assert(0x038 == OFFSETOF(visibility_projection, basis_to_world));
static_assert(0x06C == OFFSETOF(visibility_projection, near_bounded_flag));
static_assert(0x06D == OFFSETOF(visibility_projection, pad1));
static_assert(0x070 == OFFSETOF(visibility_projection, near_distance));
static_assert(0x074 == OFFSETOF(visibility_projection, near_plane));
static_assert(0x084 == OFFSETOF(visibility_projection, far_bounded_flag));
static_assert(0x085 == OFFSETOF(visibility_projection, far_bounded_spherical_flag));
static_assert(0x086 == OFFSETOF(visibility_projection, pad2));
static_assert(0x088 == OFFSETOF(visibility_projection, far_distance));
static_assert(0x08C == OFFSETOF(visibility_projection, volume_bounded_flag));
static_assert(0x08D == OFFSETOF(visibility_projection, pad3));
static_assert(0x090 == OFFSETOF(visibility_projection, volume));
static_assert(0x1F4 == OFFSETOF(visibility_projection, convex_hull_point_count));
static_assert(0x1F8 == OFFSETOF(visibility_projection, convex_hull_points));

struct visibility_cluster
{
	s_cluster_reference cluster_reference;
	short volume_counts[6];
	short first_volume_indices[6];
};
static_assert(sizeof(visibility_cluster) == 0x1A);
static_assert(0x0 == OFFSETOF(visibility_cluster, cluster_reference));
static_assert(0x2 == OFFSETOF(visibility_cluster, volume_counts));
static_assert(0xE == OFFSETOF(visibility_cluster, first_volume_indices));

struct s_visibility_region
{
	short projection_count;
	visibility_projection projections[6];
	short cluster_count;
	visibility_cluster clusters[128];
	long cluster_remap_table[128];
	short volume_count;
	visibility_volume volumes[512];
};
static_assert(sizeof(s_visibility_region) == 0x2E39C);
static_assert(0x0000 == OFFSETOF(s_visibility_region, projection_count));
static_assert(0x0004 == OFFSETOF(s_visibility_region, projections));
static_assert(0x0C94 == OFFSETOF(s_visibility_region, cluster_count));
static_assert(0x0C96 == OFFSETOF(s_visibility_region, clusters));
static_assert(0x1998 == OFFSETOF(s_visibility_region, cluster_remap_table));
static_assert(0x1B98 == OFFSETOF(s_visibility_region, volume_count));
static_assert(0x1B9C == OFFSETOF(s_visibility_region, volumes));

enum e_collection_shape
{
	_visibility_collection_shape_projections = 0,
	_visibility_collection_shape_sphere,
	_visibility_collection_shape_pvs,

	k_number_collection_shape_types
};

enum e_collection_type
{
	_visibility_collection_type_camera = 0,
	_visibility_collection_type_light,
	_visibility_collection_type_lightmap_shadow,
	_visibility_collection_type_cinematic_shadow,

	k_number_collection_types
};

struct s_visibility_input
{
	s_visibility_region region;
	s_cluster_reference cluster_reference;
	long user_index;
	long player_window_index;
	long flags;
	short projection_count;
	real_point3d sphere_center;
	real sphere_radius;
	e_collection_type collection_type;
	e_collection_shape collection_shape;
	long visible_items_marker_index;
	ulong visible_cluster_bitvector[16][8];
	c_static_array<c_static_array<char, 256>, 16> cluster_to_visibility_cluster_lookup;
};
static_assert(sizeof(s_visibility_input) == 0x2F5CC);
static_assert(0x0 == OFFSETOF(s_visibility_input, region));
static_assert(0x2E39C == OFFSETOF(s_visibility_input, cluster_reference));
static_assert(0x2E3A0 == OFFSETOF(s_visibility_input, user_index));
static_assert(0x2E3A4 == OFFSETOF(s_visibility_input, player_window_index));
static_assert(0x2E3A8 == OFFSETOF(s_visibility_input, flags));
static_assert(0x2E3AC == OFFSETOF(s_visibility_input, projection_count));
static_assert(0x2E3B0 == OFFSETOF(s_visibility_input, sphere_center));
static_assert(0x2E3BC == OFFSETOF(s_visibility_input, sphere_radius));
static_assert(0x2E3C0 == OFFSETOF(s_visibility_input, collection_type));
static_assert(0x2E3C4 == OFFSETOF(s_visibility_input, collection_shape));
static_assert(0x2E3C8 == OFFSETOF(s_visibility_input, visible_items_marker_index));
static_assert(0x2E3CC == OFFSETOF(s_visibility_input, visible_cluster_bitvector));
static_assert(0x2E5CC == OFFSETOF(s_visibility_input, cluster_to_visibility_cluster_lookup));

class c_visibility_collection
{
public:
	s_cluster_reference get_cluster_reference() const
	{
		ASSERT(m_input != NULL);
		return m_input->cluster_reference;
	}

	e_collection_type get_collection_type()
	{
		ASSERT(m_input != NULL);
		ASSERT(m_input->collection_type >= 0 && m_input->collection_type < k_number_collection_types);
		return m_input->collection_type;
	}

	s_visibility_input* get_input()
	{
		return m_input;
	}

	const s_visibility_region* get_region() const
	{
		ASSERT(m_input != NULL);
		return &m_input->region;
	}

	e_collection_shape prepare_collection_for_build(long flags, e_collection_type collection_type, const visibility_projection* projections, long projection_count, s_cluster_reference initial_cluster_reference, long intersection_marker_index, const real_point3d* sphere_center, real sphere_radius, long user_index, long player_window_index);

	static long __cdecl add_root_object(long object_index, const real_point3d* object_center, real object_radius, long player_window_index, bool lit, bool shadow_casting, bool fully_contained, long region_cluster_memory, s_lod_transparency lod_transparency, bool calculate_lod, bool ignore_first_person_objects, long ignore_first_person_user_index, word* a13);
	static void __cdecl expand_sky_object(long player_window_index);
	static void __cdecl generate_objects_visible_subparts();

	s_visibility_input* m_input;

	word* __unknown4;
	// pointer to `s_visible_items::__unknown28AC0`
};
static_assert(sizeof(c_visibility_collection) == 0x8);

struct s_visibility_globals
{
	s_visibility_input g_camera_visibility_input;
	c_visibility_collection camera_visibility;
	c_visible_items visible_items;
};
static_assert(sizeof(s_visibility_globals) == 0x2F5D8);

struct __declspec(align(16)) c_visibility_globals_keeper
{
	c_visibility_globals_keeper()
	{
		m_initialized = false;
	}

	~c_visibility_globals_keeper()
	{
	}

	c_visibility_globals_keeper* get()
	{
		//ASSERT(restricted_region_locked_for_current_thread(k_game_state_render_region));
		return this;
	}

	void initialize()
	{
		m_initialized = true;
		csmemset(&m_visibility_globals, 0, sizeof(s_visibility_globals));
	}

	s_visibility_globals m_visibility_globals;
	bool m_initialized;
};
static_assert(sizeof(c_visibility_globals_keeper) == 0x2F5E0);

extern c_visibility_globals_keeper& g_visibility_globals_keeper;

extern c_visibility_collection* get_global_camera_collection();
extern bool __vectorcall visibility_volume_test_sphere(visibility_volume* volume, const real_point3d* point, real radius);