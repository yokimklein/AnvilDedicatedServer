#include "rasterizer.h"
#include "cseries\cseries.h"
#include "math\integer_math.h"
#include "multithreading\threads.h"
#include "cseries\cseries_events.h"

REFERENCE_DECLARE(ADDRESS_RASTERIZER_RENDER_GLOBALS, s_rasterizer_render_globals, c_rasterizer::render_globals);
REFERENCE_DECLARE(ADDRESS_RASTERIZER_G_LAST_VIEWPORT, rectangle2d, c_rasterizer::g_last_viewport);
REFERENCE_DECLARE(ADDRESS_RASTERIZER_G_CURRENT_VERTEX_DECLARATION, IDirect3DVertexDeclaration9*, c_rasterizer::g_current_vertex_declaration);
REFERENCE_DECLARE(ADDRESS_RASTERIZER_G_DEVICE, IDirect3DDevice9Ex*, c_rasterizer::g_device);
REFERENCE_DECLARE(ADDRESS_RASTERIZER_G_CURRENT_INDEX_BUFFER, IDirect3DIndexBuffer9*, c_rasterizer::g_current_index_buffer);
REFERENCE_DECLARE_ARRAY(ADDRESS_RASTERIZER_X_LAST_SAMPLER_FILTER_MODES, long, c_rasterizer::x_last_sampler_filter_modes, 20);
long c_rasterizer::g_render_force_anisotropic_level = -1;
REFERENCE_DECLARE_ARRAY(ADDRESS_RASTERIZER_X_LAST_RENDER_STATE_VALUE, long, c_rasterizer::x_last_render_state_value, 4);
REFERENCE_DECLARE(ADDRESS_RASTERIZER_G_CURRENT_ALPHA_BLEND_MODE, c_rasterizer::e_alpha_blend_mode, c_rasterizer::g_current_alpha_blend_mode);

void c_rasterizer::get_fullscreen_render_pixel_bounds(rectangle2d* resolution)
{
	resolution->y0 = 0;
	resolution->x0 = 0;
	resolution->x1 = static_cast<short>(c_rasterizer::render_globals.resolution_width);
	resolution->y1 = static_cast<short>(c_rasterizer::render_globals.resolution_height);
}

void c_rasterizer::get_fullscreen_render_title_safe_pixel_bounds(rectangle2d* resolution)
{
	ASSERT(resolution);

	resolution->y0 = 0;
	resolution->x0 = 0;
	resolution->x1 = static_cast<short>(c_rasterizer::render_globals.resolution_width);
	resolution->y1 = static_cast<short>(c_rasterizer::render_globals.resolution_height);
	real x1 = (real)resolution->x1;
	resolution->x0 = static_cast<short>((real)(x1 * 0.05f));
	real y1 = (real)resolution->y1;
	resolution->y0 = static_cast<short>((real)(y1 * 0.05f));
	resolution->x1 = static_cast<short>((real)(x1 * 0.95f));
	resolution->y1 = static_cast<short>((real)(y1 * 0.95f));
}

void __fastcall c_rasterizer::get_display_pixel_bounds(rectangle2d* display_pixel_bounds)
{
	INVOKE(ADDRESS_C_RASTERIZER_GET_DISPLAY_PIXEL_BOUNDS, c_rasterizer::get_display_pixel_bounds, display_pixel_bounds);
}

void __fastcall c_rasterizer::get_display_title_safe_pixel_bounds(rectangle2d* resolution)
{
	INVOKE(ADDRESS_C_RASTERIZER_GET_DISPLAY_TITLE_SAFE_PIXEL_BOUNDS, c_rasterizer::get_display_title_safe_pixel_bounds, resolution);
}

void __fastcall c_rasterizer::restore_last_viewport()
{
	INVOKE(ADDRESS_C_RASTERIZER_RESTORE_LAST_VIEWPORT, c_rasterizer::restore_last_viewport);
}

void __fastcall c_rasterizer::set_depth_stencil_surface(e_surface depth_stencil)
{
	INVOKE(ADDRESS_C_RASTERIZER_SET_DEPTH_STENCIL_SURFACE, c_rasterizer::set_depth_stencil_surface, depth_stencil);
}

rectangle2d* c_rasterizer::get_last_viewport()
{
	return &c_rasterizer::g_last_viewport;
}

bool c_rasterizer::rasterizer_thread_owns_device()
{
	return g_thread_owning_device == get_current_thread_index();
}

const c_rasterizer_globals::s_explicit_shader* c_rasterizer_globals::get_explicit_shaders(long index)
{
	static bool warned = false;
	if (!warned && m_explicit_shader_refs.count() < 100)
	{
		event(_event_error, "Explicit shader count mismatch on access to (%d), TRY GETTING LATEST TAGS AND REBUILDING ANY CACHE FILES",
			index);

		warned = true;
	}

	if (!VALID_INDEX(index, m_explicit_shader_refs.count()))
	{
		event(_event_error, "Explicit shader (%d) doesn't exist in rasterizer_globals - returning NULL, RENDERING MAY BE AFFECTED, CRASHES MAY ENSUE - TRY GETTING LATEST TAGS AND REBUILDING ANY CACHE FILES",
			index);

		return NULL;
	}

	return &m_explicit_shader_refs[index];
}

const s_tag_reference* c_rasterizer_globals::get_explicit_pixel_shader_ref(long index)
{
	const s_explicit_shader* shader = get_explicit_shaders(index);
	ASSERT(shader);
	return &shader->pixel_shader;
}

const s_tag_reference* c_rasterizer_globals::get_explicit_vertex_shader_ref(long index)
{
	const s_explicit_shader* shader = get_explicit_shaders(index);
	ASSERT(shader);
	return &shader->vertex_shader;
}

#pragma runtime_checks("", off)
bool __fastcall c_rasterizer::set_vertex_shader(const c_rasterizer_vertex_shader* vertex_shader, e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, e_entry_point entry_point)
{
	bool return_value = INVOKE(ADDRESS_C_RASTERIZER_SET_VERTEX_SHADER, c_rasterizer::set_vertex_shader, vertex_shader, base_vertex_type, transfer_vertex_type, entry_point);
	__asm add esp, 8; // Fixup stack
	return return_value;

	//if (!c_rasterizer::g_device)
	//	return true;
	//
	//render_method_submit_invalidate_cache();
	//
	//if (vertex_shader)
	//{
	//	IDirect3DVertexShader9* d3d_shader = vertex_shader->get_d3d_shader(base_vertex_type, entry_point, 0);
	//	bool vertex_declaration_set = c_vertex_declaration_table::set(base_vertex_type, transfer_vertex_type, entry_point);
	//
	//	if (d3d_shader == g_current_vertex_shader)
	//		return d3d_shader != NULL && vertex_declaration_set;
	//
	//	g_current_vertex_shader = d3d_shader;
	//	return SUCCEEDED(c_rasterizer::g_device->SetVertexShader(d3d_shader)) && d3d_shader != NULL && vertex_declaration_set;
	//}
	//else if (g_current_vertex_shader)
	//{
	//	g_current_vertex_shader = NULL;
	//	return SUCCEEDED(c_rasterizer::g_device->SetVertexShader(NULL));
	//}
	//
	//return true;
}
#pragma runtime_checks("", restore)

bool __fastcall c_rasterizer::set_pixel_shader(const c_rasterizer_pixel_shader* pixel_shader, e_entry_point entry_point)
{
	return INVOKE(ADDRESS_C_RASTERIZER_SET_PIXEL_SHADER, c_rasterizer::set_pixel_shader, pixel_shader, entry_point);

	//if (!c_rasterizer::g_device)
	//	return true;
	//
	//render_method_submit_invalidate_cache();
	//
	//if (pixel_shader)
	//{
	//	IDirect3DPixelShader9* d3d_shader = pixel_shader->get_d3d_shader(entry_point, 0);
	//	if (d3d_shader == g_current_pixel_shader)
	//		return d3d_shader != NULL;
	//
	//	g_current_pixel_shader = d3d_shader;
	//	return SUCCEEDED(c_rasterizer::g_device->SetPixelShader(d3d_shader)) && d3d_shader != NULL;
	//}
	//else if (g_current_pixel_shader)
	//{
	//	g_current_pixel_shader = NULL;
	//	return SUCCEEDED(c_rasterizer::g_device->SetPixelShader(NULL));
	//}
	//
	//return true;
}

#pragma runtime_checks("", off)
void __fastcall c_rasterizer::draw_primitive_up(c_rasterizer_index_buffer::e_primitive_type primitive_type, ulong primitive_count, const void* stream_data, ulong stride)
{
	INVOKE(ADDRESS_C_RASTERIZER_DRAW_PRIMITIVE_UP, c_rasterizer::draw_primitive_up, primitive_type, primitive_count, stream_data, stride);
	__asm add esp, 8; // Fixup stack
}
#pragma runtime_checks("", restore)

#pragma runtime_checks("", off)
void __fastcall c_rasterizer::set_pixel_shader_constant(long constant_index, long count, const real_vector4d* constants)
{
	INVOKE(ADDRESS_C_RASTERIZER_SET_PIXEL_SHADER_CONSTANT, c_rasterizer::set_pixel_shader_constant, constant_index, count, constants);
	__asm add esp, 4; // Fixup stack
}
#pragma runtime_checks("", restore)

void __fastcall c_rasterizer::set_z_buffer_mode(e_z_buffer_mode mode)
{
	INVOKE(ADDRESS_C_RASTERIZER_SET_Z_BUFFER_MODE, c_rasterizer::set_z_buffer_mode, mode);
}

void __fastcall c_rasterizer::set_cull_mode(e_cull_mode cull_mode)
{
	INVOKE(ADDRESS_C_RASTERIZER_SET_CULL_MODE, c_rasterizer::set_cull_mode, cull_mode);

	//if (cull_mode != g_current_cull_mode)
	//{
	//	g_current_cull_mode = cull_mode;
	//	c_rasterizer::g_device->SetRenderState(D3DRS_CULLMODE, cull_mode);
	//}
}

void c_rasterizer::set_alpha_blend_mode(e_alpha_blend_mode alpha_blend_mode)
{
	if (alpha_blend_mode != g_current_alpha_blend_mode)
	{
		g_current_alpha_blend_mode = alpha_blend_mode;
		set_alpha_blend_mode_custom_device_no_cache(c_rasterizer::g_device, alpha_blend_mode);
	}
}

// optimised function has device argument baked into body as c_rasterizer::g_device, device will be ignored
void __fastcall c_rasterizer::set_alpha_blend_mode_custom_device_no_cache(IDirect3DDevice9Ex* device, e_alpha_blend_mode alpha_blend_mode)
{
	INVOKE(ADDRESS_C_RASTERIZER_SET_ALPHA_BLEND_MODE_CUSTOM_DEVICE_NO_CACHE, c_rasterizer::set_alpha_blend_mode_custom_device_no_cache, NULL, alpha_blend_mode);
}

bool __fastcall c_rasterizer::begin_frame()
{
	return INVOKE(ADDRESS_C_RASTERIZER_BEGIN_FRAME, c_rasterizer::begin_frame);
}

void __fastcall c_rasterizer::setup_targets_simple()
{
	INVOKE(ADDRESS_C_RASTERIZER_SETUP_TARGETS_SIMPLE, c_rasterizer::setup_targets_simple);
}

bool __fastcall c_rasterizer::end_frame()
{
	return INVOKE(ADDRESS_C_RASTERIZER_END_FRAME, c_rasterizer::end_frame);
}

bool rasterizer_get_is_widescreen()
{
	return ((real)c_rasterizer::render_globals.resolution_width / (real)c_rasterizer::render_globals.resolution_height) > 1.5f;
}

bool rasterizer_dump_display_to_bmp(const char* file_name)
{
	// $TODO: !!!
	return true;
}
