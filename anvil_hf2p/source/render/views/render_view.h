#pragma once
#include <input\input_windows.h>
#include "render\render_cameras.h"
#include <rasterizer\rasterizer.h>
#include <render\render_game_state.h>
#include <camera\observer.h>
#include <render\render_patchy_fog.h>
#include <visibility\visibility_collection.h>

__interface IDirect3DSurface9;
struct light_geometry;
struct s_observer_result;

class c_view
{
public:
	c_view() :
		m_rasterizer_camera(),
		m_rasterizer_projection(),
		m_render_camera(),
		m_render_projection()
	{
	}

	virtual void render() {};
	virtual long render_setup() {};
	virtual long compute_visibility() {};
	virtual long render_submit_visibility() {};

	//static void __cdecl abort_current_view_stack();
	//static void __cdecl begin(c_view* view);
	//static void __cdecl sub_A28A40();
	//static void __cdecl sub_A28A90();
	//static void __cdecl end();
	//static long __cdecl get_current_stack_level();
	static c_view* top();

	const render_camera* get_render_camera() const;
	render_camera* get_render_camera_modifiable();
	
	const render_camera* get_rasterizer_camera() const;
	render_camera* get_rasterizer_camera_modifiable();
	
	const render_projection* get_rasterizer_projection() const;
	render_projection* get_rasterizer_projection_modifiable();
	
	const render_projection* get_render_projection() const;
	render_projection* get_render_projection_modifiable();

protected:
	static long& g_view_stack_top;
	static c_view* (&g_view_stack)[4];

	render_camera m_rasterizer_camera;
	render_projection m_rasterizer_projection;
	render_camera m_render_camera;
	render_projection m_render_projection;
};
static_assert(sizeof(c_view) == 0x294);

class c_ui_view : public c_view
{
public:
	c_ui_view() :
		c_view(),
		m_is_screenshot(),
		m_render_surface()
	{
		//__vftable = reinterpret_cast<decltype(__vftable)>(base_address(0x0));
	}

	//void __thiscall render_();

	//void setup_camera(const s_observer_result* result, c_rasterizer::e_surface surface);

	bool m_is_screenshot;
	c_rasterizer::e_surface m_render_surface;
};
static_assert(sizeof(c_ui_view) == sizeof(c_view) + 0x8);

class c_fullscreen_view : public c_view
{
public:
	c_fullscreen_view() :
		c_view()
	{
		//__vftable = reinterpret_cast<decltype(__vftable)>(base_address(0x0));
	}

	//void __thiscall render_();
	//static void render_debug_stuff_while_loading();

	//void setup_camera(const s_observer_result* observer);
	//void render_blank_frame(const real_rgb_color* background_color);
};
static_assert(sizeof(c_fullscreen_view) == sizeof(c_view));

class c_world_view : public c_view
{
public:
	//void get_starting_cluster(s_cluster_reference* starting_cluster);

	//protected:
	bool m_using_stored_cluster;
	byte pad;
	s_cluster_reference m_stored_cluster;
};
static_assert(sizeof(c_world_view) == sizeof(c_view) + 0x4);
static_assert(0x0 == OFFSETOF(c_world_view, m_using_stored_cluster) - sizeof(c_view));
static_assert(0x1 == OFFSETOF(c_world_view, pad) - sizeof(c_view));
static_assert(0x2 == OFFSETOF(c_world_view, m_stored_cluster) - sizeof(c_view));

class c_lights_view : public c_world_view
{
public:
	//void submit_simple_light_draw_list_to_shader() const;
	//void build_simple_light_draw_list(long a1);
	//void clear_simple_light_draw_list(long a1);
	//void render(long user_index, long player_index, IDirect3DSurface9* a3, IDirect3DSurface9* a4, IDirect3DSurface9* a5);

	static long& g_gel_bitmap_index;
	static real& g_render_light_intensity;
	static ulong& g_debug_clip_planes;

protected:
	struct s_simple_light
	{
		real_point3d m_position;
		real m_light_source_size;
		real_vector3d m_inv_direction;
		real m_sphere;
		real_rgb_color m_color;
		real m_cone_smooth;
		real m_distance_scale;
		real m_cone_scale;
		real m_distance_offset;
		real m_cone_offset;
		real m_bounding_radius2;
		real m_padding[3];
	};
	static_assert(sizeof(s_simple_light) == 0x50);

	IDirect3DSurface9* m_ldr_surface;
	IDirect3DSurface9* m_hdr_surface;
	IDirect3DSurface9* m_depth_surface;
	visibility_projection visibility_projections[6];
	short projection_count;
	real texture_projection_matrix[4][4];
	bool light_orthogonal;
	real light_near_width;
	real light_near_height;
	real light_near_depth;
	real light_far_depth;
	long shadow_res_x;
	long shadow_res_y;
	long m_user_index;
	long m_simple_light_count;
	s_simple_light m_local_light;
	s_simple_light m_simple_lights[8];
	real m_light_intensity_scale;
};
static_assert(sizeof(c_lights_view) == sizeof(c_world_view) + 0xFD8);

class c_lightmap_shadows_view : public c_world_view
{
public:
	enum e_shadow_mode
	{
		_shadow_mode_point = 0,
		_shadow_mode_box2x2,
		_shadow_mode_bilinear3x3,
		_shadow_mode_bilinear4x4,
		_shadow_mode_old,

		k_shadow_mode_count,
	};

public:
	//void compute_visibility(long object_index, long forced_shadow_receiver_object_index);
	//static long __cdecl get_shadow_apply_shader(e_shadow_mode shadow_mode);
	//static bool __cdecl object_shadow_visible(long object_index, s_oriented_bounding_box* obb, light_geometry* geometry);
	//void render(long ignore_object_index);
	//void render_ambient(real shadow_alpha, real shadow_resolution, long object_index, long model_index);
	//static void __cdecl set_shadow_generate_mode(e_shadow_mode shadow_mode);
	//void submit_visibility_and_render(real shadow_alpha, real shadow_resolution);

public:
	s_oriented_bounding_box m_projection_bounds;

	static bool& g_debug_shadow_screenspace;
	static bool& g_debug_shadow_bounds;
	static bool& g_debug_shadow_bounds_solid;
	static bool& g_debug_shadow_opaque;
	static bool& g_debug_shadow_histencil;
	static bool& g_debug_shadow_force_hi_res;
	static bool& g_debug_objectspace_stencil_clip;
	static bool& g_debug_force_fancy_shadows;
	static bool& g_debug_force_old_shadows;
};
static_assert(sizeof(c_lightmap_shadows_view) == sizeof(c_world_view) + 0x40);

class c_occlusion_view : public c_lightmap_shadows_view
{
public:
};
static_assert(sizeof(c_occlusion_view) == sizeof(c_lightmap_shadows_view));

class c_reflection_view : public c_view
{
public:
};
static_assert(sizeof(c_reflection_view) == sizeof(c_view));

class c_first_person_view : public c_view
{
public:
	//void __thiscall override_projection(bool squish_close_to_camera);
	//void __thiscall render_albedo(long user_index);

	//protected:

		// c_first_person_camera::update
		// c_first_person_view::override_projection
		// hs_external_globals[242]
	static real& m_fov_scale;

	// c_first_person_view::override_projection
	static real& m_z_far_scale; // from reach, missing `g_enable_first_person_squish && ` in if statement, always `1.0f`

	render_camera* m_default_rasterizer_camera;
};
static_assert(sizeof(c_first_person_view) == sizeof(c_view) + 0x4);

class c_player_view : public c_world_view
{
public:
	static c_player_view*& x_current_player_view;
	static c_player_view(&x_global_player_views)[4];

	static c_player_view* __cdecl get_current()
	{
		return x_current_player_view;
	}

	static c_player_view* __cdecl get_current(long view_index)
	{
		return &x_global_player_views[view_index];
	}

	static void __cdecl set_global_player_view(c_player_view* view)
	{
		// asserts

		x_current_player_view = view;
	}

	long get_player_view_user_index()
	{
		return m_camera_user_data.user_index;
	}

protected:
	//void animate_water();

public:
	//void __thiscall apply_distortions();
	//void create_frame_textures(long player_index);

	//protected:
	//void __thiscall generate_distortions();
	//static void __cdecl generate_distortions_callback(long __formal);

public:
	static void get_player_render_camera_orientation(real_matrix4x3* camera);
	//void __thiscall queue_patchy_fog();
	//void __thiscall render_();
	//bool __thiscall render_albedo();
	//static void __cdecl render_albedo_decals(bool render_object_decals, bool render_structure_decals);
	//void __thiscall render_effects(e_effect_pass effect_pass);
	//void __thiscall render_first_person(bool render_only_transparents);
	//void __thiscall render_first_person_albedo();
	//void __thiscall render_lens_flares();
	//void __thiscall render_lightmap_shadows();
	//static void __cdecl render_misc_transparents();
	//void __thiscall render_static_lighting();
	//void __thiscall render_transparents();
	//void __thiscall render_water();
	//void __thiscall render_weather_occlusion();
	//void restore_to_display_surface();
	//void setup_camera(long player_window_index, long player_window_count, long player_window_arrangement, long user_index, const s_observer_result* observer, bool freeze_render_camera);
	//void __thiscall setup_camera_fx_parameters(real exposure_boost);
	//void __thiscall setup_cinematic_clip_planes();
	//void __thiscall submit_attachments();
	//void __thiscall submit_distortions();
	//void __thiscall submit_occlusion_tests(bool occlusion, bool conditional);

	//static void frame_advance();

	//protected:
	struct s_camera_user_data
	{
		long player_window_index;
		long player_window_count;
		long player_window_arrangement;
		long user_index;
		e_controller_index controller_index;
		c_rasterizer::e_splitscreen_res m_splitscreen_res_index;
		long m_splitscreen_resolve_surface;
	};
	static_assert(sizeof(s_camera_user_data) == 0x1C);

	s_render_game_state::s_player_window* m_window_game_state;
	real m_render_exposure;
	real m_illum_render_scale;
	s_observer_depth_of_field m_observer_depth_of_field;
	c_patchy_fog m_patchy_fog;

	struct
	{
		real_point3d position;
		real_vector3d forward;
		real_vector3d up;
		real game_time;
		real_matrix4x3 view_matrix;
		real projection_matrix[4][4];
	} m_last_frame_motion_blur_state;

	c_first_person_view m_first_person_view;
	c_ui_view m_ui_view;
	c_lights_view m_lights_view;
	c_lightmap_shadows_view m_lightmap_shadows_view;
	c_reflection_view m_reflection_view;
	c_occlusion_view m_occlusion_view;
	rectangle2d m_final_surface_window_rect;
	s_camera_user_data m_camera_user_data;
	bool m_stall_cpu_to_wait_for_gpu;
};
static_assert(sizeof(c_player_view) == sizeof(c_world_view) + 0x2420);

bool render_resources_disabled();
void render_debug_window_render(long user_index);
void __fastcall render_debug_frame_render();