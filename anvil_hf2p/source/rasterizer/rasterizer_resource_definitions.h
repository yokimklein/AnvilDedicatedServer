#pragma once
#include <d3d9.h>

enum e_vertex_type
{
	_vertex_type_world = 0,
	_vertex_type_rigid,
	_vertex_type_skinned,
	_vertex_type_particle_model,
	_vertex_type_flat_world,
	_vertex_type_flat_rigid,
	_vertex_type_flat_skinned,
	_vertex_type_screen,
	_vertex_type_debug,
	_vertex_type_transparent,
	_vertex_type_particle,
	_vertex_type_contrail,
	_vertex_type_light_volume,
	_vertex_type_simple_chud,
	_vertex_type_fancy_chud,
	_vertex_type_decorator,
	_vertex_type_tiny_position,
	_vertex_type_patchy_fog,
	_vertex_type_water,
	_vertex_type_ripple,
	_vertex_type_implicit,
	_vertex_type_beam,
	_vertex_type_dual_quat,

	k_number_of_vertex_types
};

enum e_lighting_vertex_types
{
	// default
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_none, _transfer_vertex_none)
	// _entry_point_static_prt_ambient
	// _entry_point_static_prt_linear
	// _entry_point_static_prt_quadratic
	// _entry_point_active_camo
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_none, transfer_vertex_type)
	_lighting_vertex_none = 0,

	// _entry_point_vertex_color_lighting
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_vert_color, _transfer_vertex_none)
	_lighting_vertex_vert_color,

	// _entry_point_static_per_pixel
	// _entry_point_lightmap_debug_mode
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_lightmap_uv, _transfer_vertex_none)
	_lighting_vertex_lightmap_uv,

	// _entry_point_static_per_vertex
	// - get_d3d_vertex_declaration(base_vertex_type, _lighting_vertex_per_vertex_lightprobe, _transfer_vertex_none)
	_lighting_vertex_per_vertex_lightprobe,

	k_number_of_lighting_vertex_types
};

enum e_transfer_vector_vertex_types
{
	_transfer_vertex_none = 0,
	_transfer_prt_ambient_only,
	_transfer_prt_linear,
	_transfer_prt_quadratic,

	k_number_of_transfer_vertex_types
};

class c_vertex_declaration_table
{
public:
	static IDirect3DVertexDeclaration9* get_d3d_vertex_declaration(e_vertex_type base_vertex_type, e_lighting_vertex_types lighting_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type);
	static IDirect3DVertexDeclaration9* get_d3d_vertex_declaration_by_entry_point(e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, long entry_point);
	static bool set(e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, long entry_point);

	static D3DVERTEXELEMENT9(&lighting_vertex_decls)[k_number_of_lighting_vertex_types][64];
	static D3DVERTEXELEMENT9(&transfer_vertex_decls)[k_number_of_transfer_vertex_types][64];
	static D3DVERTEXELEMENT9(&other_vertex_decls)[4][64]; // 1: contrail, 2: beam, 3: light_volume
	static D3DVERTEXELEMENT9(&base_vertex_decls)[k_number_of_vertex_types][64];
	static IDirect3DVertexDeclaration9* (&m_vertex_declarations)[k_number_of_vertex_types][k_number_of_lighting_vertex_types][k_number_of_transfer_vertex_types];
};