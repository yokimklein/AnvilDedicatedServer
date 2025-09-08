#include "rasterizer\rasterizer.h"
#include "cseries\cseries.h"
#include "scenario\scenario.h"
#include "game\game_globals.h"
#include "cache\cache_files.h"
#include "rasterizer\rasterizer_resource_definitions.h"
#include <rasterizer\rasterizer_shader_definitions.h>

void c_rasterizer::draw_debug_line2d(const real_point3d& point1, const real_point3d& point2, pixel32 color0, pixel32 color1)
{
	rasterizer_vertex_debug point_list[2]{};
	point_list[0].position = point1;
	point_list[0].color = color0;
	point_list[1].position = point2;
	point_list[1].color = color1;

	// $TODO: TEST THIS!!!
	rectangle2d* last_viewport = c_rasterizer::get_last_viewport();
	real pixel_width_size = 1.0f / (real)(last_viewport->x1 - last_viewport->x0);
	real pixel_height_size = 1.0f / (real)(last_viewport->y1 - last_viewport->y0);
	real debug_viewport_width = (last_viewport->x0 * 2.0f) * pixel_width_size;
	real debug_viewport_height = (last_viewport->y0 * 2.0f) * pixel_height_size;

	point_list[0].position.x = (((point_list[0].position.x * pixel_width_size) * 2.0f) - 1.0f) - debug_viewport_width;
	point_list[0].position.y = debug_viewport_height - (((point_list[0].position.y * pixel_height_size) * 2.0f) - 1.0f);
	point_list[1].position.x = (((point_list[1].position.x * pixel_width_size) * 2.0f) - 1.0f) - debug_viewport_width;
	point_list[1].position.y = debug_viewport_height - (((point_list[1].position.y * pixel_height_size) * 2.0f) - 1.0f);

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug2d))
	{
		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_off);
		real_vector4d constant_data = { 1.0f, 1.0f, 1.0f, 1.0f };
		set_pixel_shader_constant(1, 1, &constant_data);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, 1, point_list, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void c_rasterizer::draw_debug_line(const real_point3d& point1, const real_point3d& point2, pixel32 color0, pixel32 color1)
{
	rasterizer_vertex_debug point_list[2]{};
	point_list[0].position = point1;
	point_list[0].color = color0;
	point_list[1].position = point2;
	point_list[1].color = color1;

	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug))
	{
		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_read);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, 1, point_list, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void c_rasterizer::draw_debug_polygon(const rasterizer_vertex_debug* vertices, long primitive_count, c_rasterizer_index_buffer::e_primitive_type primitive_type)
{
	if (primitive_count > 0)
	{
		if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug))
		{
			set_cull_mode(_cull_mode_off);
			set_z_buffer_mode(_z_buffer_mode_read);
			set_indices(NULL);
			draw_primitive_up(primitive_type, primitive_count, vertices, sizeof(rasterizer_vertex_debug));
			set_cull_mode(_cull_mode_cw);
		}
	}
}

bool c_rasterizer::set_vertex_declaration(IDirect3DVertexDeclaration9* vertex_declaration)
{
	if (vertex_declaration == g_current_vertex_declaration)
	{
		return true;
	}
	
	g_current_vertex_declaration = vertex_declaration;
	return SUCCEEDED(c_rasterizer::g_device->SetVertexDeclaration(vertex_declaration));
}

bool rasterizer_set_explicit_debug_shader(c_rasterizer_globals::e_explicit_shader shader_type)
{
	s_game_globals* game_globals = scenario_get_game_globals();
	if (!game_globals)
	{
		return false;
	}

	if (game_globals->rasterizer_globals_ref.index == NONE)
	{
		return false;
	}

	c_rasterizer_globals* rasterizer_globals = TAG_GET(RASTERIZER_GLOBALS_TAG, c_rasterizer_globals, game_globals->rasterizer_globals_ref.index);

	const s_tag_reference* default_vertex_shader_ref = rasterizer_globals->get_explicit_vertex_shader_ref(shader_type);
	const s_tag_reference* default_pixel_shader_ref = rasterizer_globals->get_explicit_pixel_shader_ref(shader_type);
	if (default_vertex_shader_ref->index == NONE || default_pixel_shader_ref->index == NONE)
	{
		return false;
	}

	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
	c_vertex_declaration_table::set(_vertex_type_debug, _transfer_vertex_none, _entry_point_default);

	c_rasterizer_vertex_shader* vertex_shader = TAG_GET(VERTEX_SHADER_TAG, c_rasterizer_vertex_shader, default_vertex_shader_ref->index);
	c_rasterizer::set_vertex_shader(vertex_shader, _vertex_type_debug, _transfer_vertex_none, _entry_point_default);

	c_rasterizer_pixel_shader* pixel_shader = TAG_GET(PIXEL_SHADER_TAG, c_rasterizer_pixel_shader, default_pixel_shader_ref->index);
	c_rasterizer::set_pixel_shader(pixel_shader, _entry_point_default);

	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);

	return true;
}

void c_rasterizer::draw_debug_line_list2d_explicit(const rasterizer_vertex_debug* points, long line_count)
{
	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug2d))
	{
		real_vector4d fill_color[1]
		{
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		};

		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_off);
		set_pixel_shader_constant(1, NUMBEROF(fill_color), fill_color);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, line_count, points, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}

void c_rasterizer::draw_debug_line_list_explicit(const rasterizer_vertex_debug* points, long line_count)
{
	if (rasterizer_set_explicit_debug_shader(c_rasterizer_globals::_shader_debug))
	{
		set_cull_mode(_cull_mode_off);
		set_z_buffer_mode(_z_buffer_mode_read);
		set_indices(NULL);
		draw_primitive_up(c_rasterizer_index_buffer::_primitive_type_line_list, line_count, points, sizeof(rasterizer_vertex_debug));
		set_cull_mode(_cull_mode_cw);
	}
}