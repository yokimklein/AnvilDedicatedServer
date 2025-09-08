#pragma once
#include "rasterizer\rasterizer_vertex_definitions.h"
#include <tag_files\tag_groups.h>

enum _D3DRENDERSTATETYPE;
__interface IDirect3D9Ex;
__interface IDirect3DDevice9Ex;
__interface IDirect3DQuery9;
__interface IDirect3DVertexBuffer9;
__interface IDirect3DIndexBuffer9;
__interface IDirect3DVertexDeclaration9;

namespace DirectX
{
	struct XMMATRIX;
}

struct s_rasterizer_render_globals
{
	long resolution_width;
	long resolution_height;
	long unknown_width; // scale?
	long unknown_height; // scale?
	long back_buffer_width;
	long back_buffer_height;
	long shadow_resolution;
	void* window_handle;
	bool is_d3d9ex;
};
static_assert(sizeof(s_rasterizer_render_globals) == 0x24);

class c_rasterizer_index_buffer
{
public:
	enum e_primitive_type
	{
		_primitive_type_point_list = 0,
		_primitive_type_line_list,
		_primitive_type_list_strip,
		_primitive_type_triangle_list,
		_primitive_type_triangle_fan,
		_primitive_type_triangle_strip,
		_primitive_type_force_dword,

		k_primitive_type_count
	};

	//D3DPRIMITIVETYPE g_primitive_types[k_primitive_type_count]
	//{
	//	D3DPT_POINTLIST,
	//	D3DPT_LINELIST,
	//	D3DPT_LINESTRIP,
	//	D3DPT_TRIANGLELIST,
	//	D3DPT_TRIANGLEFAN,
	//	D3DPT_TRIANGLESTRIP,
	//	D3DPT_FORCE_DWORD
	//};
};

union rectangle2d;
class c_rasterizer_vertex_shader;
class c_rasterizer_pixel_shader;
enum e_vertex_type;
enum e_transfer_vector_vertex_types;
enum e_entry_point;
class c_rasterizer
{
public:
	enum e_splitscreen_res
	{
		_res_default = 0,
		_res_half_width,
		_res_half_size,
		_res_8_x_3_half_height,

		k_number_of_splitscreen_res_resolutions
	};

	enum e_separate_alpha_blend_mode
	{
		_separate_alpha_blend_off = 0,
		_separate_alpha_blend_opaque,
		_separate_alpha_blend_additive,
		_separate_alpha_blend_multiply,
		_separate_alpha_blend_to_constant,
		_separate_alpha_blend_unknown, // new since ms23?

		k_number_of_separate_alpha_blend_modes
	};

	enum e_stencil_mode
	{
		_stencil_mode_off = 0,
		_stencil_mode_old_stipple,
		_stencil_mode_volume_back_planes,
		_stencil_mode_volume_front_planes,
		_stencil_mode_volume_apply,
		_stencil_mode_volume_apply_hi_stencil_only,
		_stencil_mode_volume_apply_and_clear,
		_stencil_mode_volume_clear,
		_stencil_mode_volume_clear_hi_stencil,
		_stencil_mode_decorators,
		_stencil_mode_decals,

		_stencil_mode_tron_write,
		_stencil_mode_tron_read,
		_stencil_mode_ambient_object_clear_shadow_bit,
		_stencil_mode_glass_decals_write,
		_stencil_mode_glass_decals_read,
		_stencil_mode_object_rendering,

		_stencil_mode_unknown17,
		_stencil_mode_unknown18,
		_stencil_mode_unknown19,
		_stencil_mode_unknown20,
		_stencil_mode_unknown21, // new since ms23

		k_number_of_stencil_modes
	};

	enum e_gpr_allocation
	{
		_gpr_default_d3d = 0,
		_gpr_default,
		_gpr_max_to_pixel_shader,
		_gpr_max_to_vertex_shader,
		_gpr_all_to_vertex_shader,
		_gpr_decorator_allocation,
		_gpr_count
	};


	enum e_cull_mode
	{
		_cull_mode_off = 0x1,
		_cull_mode_cw = 0x2,
		_cull_mode_ccw = 0x3,
		_cull_mode_force_dword = 0x7FFFFFFF,
	};

	enum e_fill_mode
	{
		_fill_mode_point = 0x1,
		_fill_mode_wireframe = 0x2,
		_fill_mode_solid = 0x3,
		_fill_mode_force_dword = 0x7FFFFFFF,
	};

	enum e_alpha_blend_mode
	{
		_alpha_blend_opaque = 0,
		_alpha_blend_additive,
		_alpha_blend_multiply,
		_alpha_blend_alpha_blend,
		_alpha_blend_double_multiply,
		_alpha_blend_pre_multiplied_alpha,
		_alpha_blend_maximum,
		_alpha_blend_multiply_add,
		_alpha_blend_add_src_times_dstalpha,
		_alpha_blend_add_src_times_srcalpha,
		_alpha_blend_inv_alpha_blend,
		_alpha_blend_motion_blur_static,
		_alpha_blend_motion_blur_inhibit,

		k_number_of_alpha_blend_modes
	};

	enum e_surface
	{
		_surface_none = 0,
		_surface_disable,
		_surface_display,
		_surface_shadow_1,
		_surface_depth_stencil,
		_surface_depth_stencil_read_only,
		_surface_screenshot_composite_16f,
		_surface_accum_LDR,
		_surface_accum_HDR,
		_surface_albedo,
		_surface_albedo_debug,
		_surface_gbuf,
		_surface_normal,
		_surface_post_LDR,
		_surface_post_HDR,
		_surface_post_half_LDR,
		_surface_post_half_HDR,
		_surface_normal_half,
		_surface_depth_half_fp32,
		_surface_color_half_fp16_0,
		_surface_color_half_fp16_1,
		_surface_fullscreen_blur,
		_surface_shadow_color,
		_surface_depth_fp32,
		_surface_aux_reflection,
		_surface_aux_refraction,
		_surface_hdr_retrieve,
		_surface_ssao_noise,
		_surface_screenshot_composite_8bit,
		_surface_screenshot_display,
		_surface_screenshot_composite_16i,
		_surface_aux_bloom,
		_surface_aux_chud_overlay,
		_surface_aux_star,
		_surface_aux_small,
		_surface_aux_tiny,
		_surface_aux_mini,
		_surface_aux_exposure_0,
		_surface_aux_exposure_1,
		_surface_aux_exposure_2,
		_surface_aux_exposure_3,
		_surface_aux_exposure_4,
		_surface_aux_exposure_5,
		_surface_aux_exposure_6,
		_surface_aux_exposure_7,
		_surface_aux_small2,
		_surface_aux_tiny2,
		_surface_aux_mini2,
		_surface_aux_depth_of_field_high_rez,
		_surface_aux_depth_of_field_high_rez2,
		_surface_distortion,
		_surface_depth_camera_depth,
		_surface_depth_camera,
		_surface_aux_water_interaction_height,
		_surface_aux_water_interaction_slope,
		_surface_chud_turbulence,
		_surface_cortana_effect_mua_haha,
		_surface_depth_camera_texture,
		_surface_weather_occlusion,
		_surface_depth_visr,

		//_surface_dynamic_albedo,
		//_surface_dynamic_accumulation,
		//_surface_dynamic_normal,
		//_surface_dynamic_depth_stencil,
		//_surface_dynamic_depth_stencil_read_only,

		k_number_of_surfaces,
		k_surface_aux_exposure_count = 8,
	};

	enum e_sampler_address_mode
	{
		_sampler_address_wrap = 0,
		_sampler_address_clamp,
		_sampler_address_mirror,
		_sampler_address_border,
		_sampler_address_mirror_once,
		_sampler_address_mirror_once_border,

		k_number_of_sampler_address_modes
	};

	enum e_sampler_filter_mode
	{
		_sampler_filter_trilinear = 0,
		_sampler_filter_point,
		_sampler_filter_bilinear,
		_sampler_filter_anisotropic_1,
		_sampler_filter_anisotropic_2,
		_sampler_filter_anisotropic_3,
		_sampler_filter_anisotropic_4,
		_sampler_filter_lightprobe_texture_array,
		_sampler_filter_texture_array_quadlinear,
		_sampler_filter_texture_array_quadanisotropic_2,

		k_sampler_filter_modes
	};

	enum e_z_buffer_mode
	{
		_z_buffer_mode_write = 0,
		_z_buffer_mode_read,
		_z_buffer_mode_off,
		_z_buffer_mode_shadow_generate,
		_z_buffer_mode_shadow_generate_dynamic_lights,
		_z_buffer_mode_shadow_apply,
		_z_buffer_mode_decals,
		_z_buffer_mode_debug_geometry,
		_z_buffer_mode_unknown8,

		k_number_of_z_buffer_modes
	};

	enum e_platform
	{
		_platform_xenon = 0,
		_platform_dx9,

		k_platform_count
	};

	struct s_stream_source
	{
		IDirect3DVertexBuffer9* data;
		unsigned int offset;
		unsigned int stride;
	};
	static_assert(sizeof(s_stream_source) == 0xC);

	static void get_fullscreen_render_pixel_bounds(rectangle2d* resolution);
	static void get_fullscreen_render_title_safe_pixel_bounds(rectangle2d* resolution);
	static void __fastcall get_display_pixel_bounds(rectangle2d* display_pixel_bounds);
	static void __fastcall get_display_title_safe_pixel_bounds(rectangle2d* resolution);

	static void __fastcall restore_last_viewport();
	static void __fastcall set_depth_stencil_surface(e_surface depth_stencil);

	static rectangle2d* get_last_viewport();
	static bool rasterizer_thread_owns_device();
	static void draw_debug_line(const real_point3d& point1, const real_point3d& point2, pixel32 color0, pixel32 color1);
	static void draw_debug_line2d(const real_point3d& point1, const real_point3d& point2, pixel32 color0, pixel32 color1);
	static void draw_debug_line_list2d_explicit(const rasterizer_vertex_debug* points, long primitive_count);
	static void draw_debug_line_list_explicit(const rasterizer_vertex_debug* points, long line_count);
	static void draw_debug_polygon(const rasterizer_vertex_debug* vertices, long primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type);
	static void __fastcall set_z_buffer_mode(e_z_buffer_mode mode);
	static void set_sampler_filter_mode(long sampler_index, e_sampler_filter_mode sampler_filter_mode);
	static void __fastcall set_sampler_filter_mode_custom_device_no_cache(IDirect3DDevice9Ex* device, long sampler_index, e_sampler_filter_mode sampler_filter_mode);
	static void __fastcall set_cull_mode(e_cull_mode cull_mode);
	static void __fastcall set_color_write_enable(long target_index, long enable);
	static void set_alpha_blend_mode(e_alpha_blend_mode alpha_blend_mode);
	static bool set_vertex_declaration(IDirect3DVertexDeclaration9*);
	static bool __fastcall set_vertex_shader(const c_rasterizer_vertex_shader* vertex_shader, e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, e_entry_point entry_point);
	static bool __fastcall set_pixel_shader(const c_rasterizer_pixel_shader* pixel_shader, e_entry_point entry_point);
	static void __fastcall draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type primitive_type, ulong primitive_count, const void* stream_data, ulong stride);
	static void set_indices(IDirect3DIndexBuffer9* index_buffer);
	static void __fastcall set_pixel_shader_constant(long constant_index, long count, const real_vector4d* constants);
	static void __fastcall set_alpha_blend_mode_custom_device_no_cache(IDirect3DDevice9Ex* device, e_alpha_blend_mode alpha_blend_mode);

	static s_rasterizer_render_globals& render_globals;
	static rectangle2d& g_last_viewport;
	static IDirect3DVertexDeclaration9*& g_current_vertex_declaration;
	static IDirect3DDevice9Ex*& g_device;
	static IDirect3DIndexBuffer9*& g_current_index_buffer;
	static long(&x_last_sampler_filter_modes)[20];
	static long g_render_force_anisotropic_level; // optimised out of ms29
	static _D3DRENDERSTATETYPE x_last_render_state_types[4]; // optimised out of ms29
	static long(&x_last_render_state_value)[4];
	static e_alpha_blend_mode& g_current_alpha_blend_mode;
};

struct s_global_bitmaps
{
	long flags;
	c_typed_tag_reference<BITMAP_TAG> ref;
};
static_assert(sizeof(s_global_bitmaps) == 0x14);

struct s_texture_references_block
{
	c_typed_tag_reference<BITMAP_TAG> ref;
};
static_assert(sizeof(s_texture_references_block) == sizeof(s_tag_reference));

class c_rasterizer_globals
{
public:
	static tag const k_group_tag = RASTERIZER_GLOBALS_TAG;

	enum e_explicit_shader
	{
		_shader_hack_au2_rank_tray = -3,
		_shader_hack_au2_playlist_rating = -2,

		_shader_debug = 0,
		_shader_debug2d,
		_shader_copy_scaled,
		_shader_bloom_spike_vertical,
		_shader_bloom_spike_horizontal,
		_shader_bloom_downsample_4x4_calc_bloom,
		_shader_bloom_downsample_4x4,
		_shader_bloom_rgbe_to_rgb,
		_shader_bloom_update_persistence,
		_shader_bloom_add_downsampled,
		_shader_bloom_add,
		_shader_bloom_blur_kernel_11_horizontal,
		_shader_bloom_blur_kernel_11_vertical,
		_shader_cubemap_blur_phi,
		_shader_cubemap_blur_theta,
		_shader_cubemap_clamp,
		_shader_cubemap_divide,
		_shader_misc_write_depth,
		_shader_postprocess_final_composite,
		_shader_sky_dome_simple,
		_shader_transparent,
		_shader_shield_meter,
		_shader_legacy_meter,
		_shader_overhead_map_geometry,
		_shader_legacy_hud_bitmap,
		_shader_bloom_blend3,
		_shader_particle_update,
		_shader_particle_spawn,
		_shader_screenshot_combine,
		_shader_downsample_2x2,
		_shader_rotate_2d,
		_shader_bspline_resample,
		_shader_bloom_downsample_4x4_bloom_DOF,
		_shader_postprocess_final_composite_DOF,
		_shader_convolve_kernel_5,
		_shader_exposure_downsample,
		_shader_yuv_to_rgb,
		_shader_displacement,
		_shader_screenshot_display,
		_shader_downsample_4x4_block,
		_shader_crop,
		_shader_screenshot_combine_DOF,
		_shader_gamma_correct,
		_shader_contrail_spawn,
		_shader_contrail_update,
		_shader_8x8_dither_pattern,
		_shader_lens_flare_render,
		_shader_decorator_render_default,
		_shader_bloom_downsample_4x4_block_bloom,
		_shader_bloom_downsample_4x4_gaussian,
		_shader_apply_color_matrix,
		_shader_copy,
		_shader_shadow_geometry,
		_shader_shadow_apply,
		_shader_gradient,
		_shader_alpha_test,
		_shader_patchy_fog,
		_shader_light_volume_update,
		_shader_water_ripple,
		_shader_double_gradient,
		_shader_sniper_scope,
		_shader_shield_impact,
		_shader_player_emblem_world,
		_shader_player_emblem_screen,
		_shader_implicit_hill,
		_shader_chud_overlay_blend,
		_shader_bloom_add_alpha1,
		_shader_bloom_downsample_4x4_block_bloom_LDR,
		_shader_ldr_hdr_depth_restore,
		_shader_beam_update,
		_shader_decorator_render_no_wind,
		_shader_decorator_render_static,
		_shader_decorator_render_sun,
		_shader_decorator_render_wavy,
		_shader_postprocess_final_composite_zoom,
		_shader_postprocess_final_composite_debug,
		_shader_shadow_apply_point,
		_shader_shadow_apply_bilinear,
		_shader_shadow_apply_fancy,
		_shader_shadow_apply_faster,
		_shader_shadow_apply_misc,
		_shader_displacement_motion_blur,
		_shader_decorator_render_shaded,
		_shader_screenshot_memexport,
		_shader_bloom_downsample_4x4_gaussian_bloom_LDR,
		_shader_bloom_downsample_4x4_gaussian_bloom,
		_shader_bloom_downsample_4x4_gaussian_bloom_new,
		_shader_bloom_curve,
		_shader_custom_gamma_correct,
		_shader_pixel_copy,
		_shader_decorator_render_edit,
		_shader_hdr_retrieve,
		_shader_smirnov,
		_shader_fxaa,
		_shader_unknown94,
		_shader_sniper_scope_stencil_pc,
		_shader_ssao,
		_shader_ssao_blur,
		_shader_unknown98,
		_shader_lightshafts,
		_shader_radial_blur,
		_shader_unknown101,
		_shader_unknown102,
		_shader_unknown103,
		_shader_unknown104,
		_shader_unknown105,
		_shader_unknown106,
		_shader_unknown107,
		_shader_hud_camera_nightvision,
		_shader_unknown109,

		k_explicit_shader_count
	};

	struct s_explicit_shader
	{
		c_typed_tag_reference<VERTEX_SHADER_TAG> vertex_shader;
		c_typed_tag_reference<PIXEL_SHADER_TAG> pixel_shader;
	};
	static_assert(sizeof(s_explicit_shader) == 0x20);

	const s_explicit_shader* get_explicit_shaders(long index);
	const s_tag_reference* get_explicit_pixel_shader_ref(long index);
	const s_tag_reference* get_explicit_vertex_shader_ref(long index);

	protected:
		// default bitmaps
		//	0:  ............... default 2D texture (white)
		//	1:  ............... default normal map (no bump)
		//	2:  ............... default cube map (error pattern)
		//	3:  ............... color bars
		//	4:  ............... black 2D texture
		//	5:  ............... transparent black 2D texture
		//	6:  ............... gray 2D texture
		//	7:  ............... auto exposure weight
		//	8:  ............... auto exposure weight (4x3)
		//	9:  ............... stencil dither pattern
		//	10: ............... smooth noise warp
		//	11: ............... ripple pattern
		c_typed_tag_block<s_global_bitmaps> m_default_textures_refs;

		// material textures
		//	0: ............... SH Glossy CT CC0236
		//	1: ............... SH Glossy CT C78D78
		//	2: ............... SH Glossy CT DD0236
		//	3: ............... TBD
		//	4: ............... TBD
		//	5: ............... TBD
		c_typed_tag_block<s_texture_references_block> m_material_textures_refs;

		// default shader vertex shader
		//	Renderer uses this shader when a vertex shader is invalid
		c_typed_tag_reference<VERTEX_SHADER_TAG> m_default_vertex_shader_ref;

		// default pixel shader
		//	Renderer uses this shader when a pixel shader is invalid
		c_typed_tag_reference<PIXEL_SHADER_TAG> m_default_pixel_shader_ref;

		// debug override shader
		//	When this shader is set, it overrides all shaders coming from geometry.
		//	However, if you leave a field as default in this shader, that field will not override the corresponding field in user shaders.
		//	For example, to render everything with a white albedo texture, select a white albedo texture in this shader
		//	and leave everything as defaults.

		// explicit shaders
		//	0: ............... debug
		//	1: ............... debug2d
		//	2: ............... copy w/scale
		//	3: ............... bloom: spike blur (vertical)
		//	4: ............... bloom: spike blur (horizontal)
		//	5: ............... bloom: downsample 4x4 (with bloom curve)
		//	6: ............... bloom: downsample 4x4
		//	7: ............... bloom: copy RGBE -> RGB
		//	8: ............... bloom: update persistence
		//	9: ............... bloom: add downsampled
		//	10: .............. bloom: add
		//	11: .............. bloom: kernel blur (11 horizontal)
		//	12: .............. bloom: kernel blur (11 vertical)
		//	13: .............. cubemap: blur (phi)
		//	14: .............. cubemap: blur (theta)
		//	15: .............. cubemap: clamp
		//	16: .............. cubemap: divide
		//	17: .............. write depth
		//	18: .............. final composite
		//	19: .............. sky dome (simple)
		//	20: .............. transparent
		//	21: .............. (legacy) shield meter
		//	22: .............. (legacy) general meter
		//	23: .............. overhead map geometry
		//	24: .............. (legacy) hud bitmap
		//	25: .............. bloom: blend 3 textures
		//	26: .............. particle update
		//	27: .............. particle spawn
		//	28: .............. screenshot combine
		//	29: .............. downsample 2x2
		//	30: .............. rotate 2d
		//	31: .............. bspline resample
		//	32: .............. bloom; downsample 4x4 (with bloom curve + DOF)
		//	33: .............. final composite (with DOF)
		//	34: .............. convolve 5-tap kernel
		//	35: .............. exposure downsample
		//	36: .............. yuv to rgb
		//	37: .............. displacement
		//	38: .............. screenshot display
		//	39: .............. downsample 4x4 block
		//	40: .............. crop
		//	41: .............. screenshot combine (with DOF)
		//	42: .............. gamma correction
		//	43: .............. contrail spawn
		//	44: .............. contrail update
		//	45: .............. stencil 8x8 dither pattern
		//	46: .............. lens flare render
		//	47: .............. decorator render
		//	48: .............. bloom: NEW downsample 4x4 block (with bloom curve)
		//	49: .............. bloom: NEW downsample 4x4 gaussian
		//	50: .............. apply color matrix
		//	51: .............. copy
		//	52: .............. shadow geometry
		//	53: .............. shadow apply
		//	54: .............. gradient
		//	55: .............. alpha test
		//	56: .............. patchy fog
		//	57: .............. light volume
		//	58: .............. water ripple
		//	59: .............. double gradient
		//	60: .............. sniper scope
		//	61: .............. shield impact
		//	62: .............. player emblem-world
		//	63: .............. player emblem-screen
		//	64: .............. implicit hill
		//	65: .............. chud overlay blend
		//	66: .............. bloom add alpha1
		//	67: .............. bloom: NEW downsample 4x4 block (with bloom curve) LDR
		//	68: .............. restore LDR, HDR, and depth to EDRAM
		//	69: .............. beam
		//	70: .............. decorators no wind
		//	71: .............. decorators static
		//	72: .............. decorators sun only
		//	73: .............. decorators wavy
		//	74: .............. final composite zoom
		//	75: .............. final composite debug
		//	76: .............. 
		//	77: .............. 
		//	78: .............. 
		//	79: .............. 
		//	80: .............. 
		//	81: .............. displacement with motion blur
		//	82: .............. 
		//	83: .............. screenshot memexport
		//	84: .............. bloom: NEW downsample 4x4 gauss with curve LDR
		//	85: .............. bloom_downsample_4x4_gaussian_bloom
		//	86: .............. bloom_downsample_4x4_gaussian_bloom new
		//	87: .............. bloom_curve
		//	88: .............. custom gamma correct
		//	89: .............. pixel copy
		//	90: .............. decorators edit
		//	91: .............. hdr retrieve
		//	92: .............. smirnov test
		//	93: .............. fxaa
		//	94: .............. 
		//	95: .............. sniper scope stencil pc
		//	96: .............. ssao
		//	97: .............. ssao_blur
		//	98: .............. 
		//	99: .............. lightshafts
		//	100: ............. radial_blur
		//	101: ............. 
		//	102: ............. 
		//	103: ............. 
		//	104: ............. 
		//	105: ............. 
		//	106: ............. 
		//	107: ............. 
		//	108: ............. hud night vision
		//	109: ............. 
		c_typed_tag_block<s_explicit_shader> m_explicit_shader_refs;

		// atmosphere lookup tables
		//	0: ............... 1D Neta Table
		//	1: ............... TBD
		//	2: ............... TBD
		//	3: ............... TBD
		//	4: ............... TBD
		//	5: ............... TBD
		c_typed_tag_block<s_texture_references_block> m_atmosphere_textures;

		// runtime
		ulong m_max_vs_gprs;
		ulong m_max_ps_gprs;

		c_typed_tag_reference<BITMAP_TAG> m_active_camo_distort_texture;
		c_typed_tag_reference<PERFORMANCE_THROTTLES_TAG> m_default_performance_throttles;
		c_typed_tag_reference<SHIELD_IMPACT_TAG> m_shield_impact_parameters;
		c_typed_tag_reference<VISION_MODE_TAG> m_vision_mode_parameters;

		// Motion blur parameters
		//	max blur:............  max amount to blur, as a percentage of the screen
		//	blur scale:..........  scales blur for a given velocity
		//	center falloff:....... reduces motion blur around the screen center (larger values give smaller reduction regions...)
		//	expected dt:.......... expected time per tick, in seconds (used to correct motion blur in fast/slow frames)

		ulong m_motion_blur_taps;
		real m_motion_blur_max_x;
		real m_motion_blur_max_y;
		real m_motion_blur_scale_x;
		real m_motion_blur_scale_y;
		real m_motion_blur_center_falloff;
		real m_motion_blur_expected_dt;

		// $TODO: these are new to HO, investigate what they are for
		ulong unknown1;
		ulong unknown2;
};
static_assert(sizeof(c_rasterizer_globals) == 0xBC);

extern bool rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader shader_type);