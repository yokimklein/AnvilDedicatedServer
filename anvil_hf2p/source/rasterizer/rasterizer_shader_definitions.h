#pragma once
#include "cseries\cseries.h"
#include "tag_files\tag_groups.h"

__interface IDirect3DVertexShader9;
__interface IDirect3DPixelShader9;

enum e_entry_point
{
	_entry_point_default = 0,
	_entry_point_albedo,
	_entry_point_static_default,
	_entry_point_static_per_pixel,
	_entry_point_static_per_vertex,
	_entry_point_static_sh,
	_entry_point_static_prt_ambient,
	_entry_point_static_prt_linear,
	_entry_point_static_prt_quadratic,
	_entry_point_dynamic_light,
	_entry_point_shadow_generate,
	_entry_point_shadow_apply,
	_entry_point_active_camo,
	_entry_point_lightmap_debug_mode,
	_entry_point_vertex_color_lighting,
	_entry_point_water_tessellation,
	_entry_point_water_shading,
	_entry_point_dynamic_light_cinematic,
	_entry_point_z_only,
	_entry_point_sfx_distort,

	k_number_of_entry_points
};

enum e_rasterizer_constant_table_type
{
	_rasterizer_constant_table_type_vertex = 0,
	_rasterizer_constant_table_type_pixel,

	// MCC
	//_rasterizer_constant_table_type_compute,

	k_rasterizer_constant_table_type_count
};

struct s_compiled_shader_reference
{
	char start_index;
	char count;
};
static_assert(sizeof(s_compiled_shader_reference) == 0x2);

struct s_rasterizer_vertex_shader_entry_point
{
	const s_compiled_shader_reference* get_shader_reference(e_vertex_type vertex_type) const;

	c_typed_tag_block<s_compiled_shader_reference> vertex_types;
};
static_assert(sizeof(s_rasterizer_vertex_shader_entry_point) == sizeof(s_tag_block));

class c_rasterizer_constant_table_definition
{
public:
	s_tag_block constants;
	c_enum<e_rasterizer_constant_table_type, char, _rasterizer_constant_table_type_vertex, k_rasterizer_constant_table_type_count> type;

	// pad
	byte qersaui[0x3];
};
static_assert(sizeof(c_rasterizer_constant_table_definition) == 0x10);

class c_rasterizer_compiled_shader
{
public:
	// ..:xenon compiled shader
	// ..:dx9 compiled shader
	s_tag_data compiled_shader[2]; // c_rasterizer::e_platform::k_platform_count

	// Xenon constant table
	// DX9 constant table
	c_rasterizer_constant_table_definition rasterizer_constant_table[2]; // c_rasterizer::e_platform::k_platform_count

	// ..:gprs
	long gprs;
};
static_assert(sizeof(c_rasterizer_compiled_shader) == 0x4C);

class c_rasterizer_compiled_vertex_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
public:
	IDirect3DVertexShader9* get_d3d_shader() const;

	IDirect3DVertexShader9* runtime_shader;
};
static_assert(sizeof(c_rasterizer_compiled_vertex_shader) == 0x50);

class c_rasterizer_compiled_pixel_shader :
	public c_rasterizer_compiled_shader // compiled shader splut
{
public:
	IDirect3DPixelShader9* get_d3d_shader() const;

	IDirect3DPixelShader9* runtime_shader;
};
static_assert(sizeof(c_rasterizer_compiled_pixel_shader) == 0x50);

class c_rasterizer_vertex_shader
{
public:
	static const c_rasterizer_vertex_shader* get(long definition_index);
	static c_rasterizer_vertex_shader* get_modifiable(long definition_index);

	const s_rasterizer_vertex_shader_entry_point* get_entry_point(long entry_point) const;
	const c_rasterizer_compiled_vertex_shader* get_compiled_shader(long shader_index) const;
	const c_rasterizer_compiled_vertex_shader* get_compiled_shader(e_vertex_type vertex_type, e_entry_point entry_point, long shader_index) const;
	IDirect3DVertexShader9* get_d3d_shader(e_vertex_type vertex_type, e_entry_point entry_point, long shader_index) const;

	c_flags<e_entry_point, ulong, k_number_of_entry_points> entry_point_flags;
	c_typed_tag_block<s_rasterizer_vertex_shader_entry_point> entry_points;
	long version;
	c_typed_tag_block<c_rasterizer_compiled_vertex_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_vertex_shader) == 0x20);

class c_rasterizer_pixel_shader
{
public:
	static const c_rasterizer_pixel_shader* get(long definition_index);
	static c_rasterizer_pixel_shader* get_modifiable(long definition_index);

	const c_rasterizer_compiled_pixel_shader* get_compiled_shader(e_entry_point entry_point, long shader_index) const;
	IDirect3DPixelShader9* get_d3d_shader(e_entry_point entry_point, long shader_index) const;

	c_flags<e_entry_point, ulong, k_number_of_entry_points> entry_point_flags;
	c_typed_tag_block<s_compiled_shader_reference> entry_points;
	long version;
	c_typed_tag_block<c_rasterizer_compiled_pixel_shader> compiled_shader;
};
static_assert(sizeof(c_rasterizer_pixel_shader) == 0x20);