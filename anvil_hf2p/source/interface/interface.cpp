#include "interface.h"
#include <game\game_globals.h>
#include <cache\cache_files.h>
#include <bitmaps\color_table_group.h>
#include <scenario\scenario.h>
#include <text\draw_string.h>

long interface_get_tag_index(short interface_tag_index)
{
	ASSERT(interface_tag_index >= 0 && interface_tag_index < NUMBER_OF_INTERFACE_TAGS);
	s_game_globals* game_globals = scenario_get_game_globals();

	// original used TAG_BLOCK_GET_ELEMENT
	if (game_globals->interface_tags.count() > 0)
	{
		return game_globals->interface_tags[0].interface_tags[interface_tag_index].index;
	}
	else
	{
		return NONE;
	}
}

real_argb_color* interface_get_real_argb_color(short interface_tag_index, short color_block_index, real_argb_color* color_out)
{
	long index = interface_get_tag_index(interface_tag_index);
	color_out->blue = 1.0f;
	color_out->green = 1.0f;
	color_out->red = 1.0f;
	color_out->alpha = 1.0f;
	if (index == NONE)
	{
		return color_out;
	}

	color_table* color_table_tag = (color_table*)tag_get(COLOR_TABLE_TAG, index);
	if (color_table_tag && color_table_tag->colors.count() > 0)
	{
		*color_out = color_table_tag->colors[color_block_index % color_table_tag->colors.count()].color;
	}

	return color_out;
}

void interface_set_bitmap_text_draw_mode(c_draw_string* draw_string, e_font_id interface_font, e_text_style style, e_text_justification justification, ulong flags, short color_table_index, short color_index)
{
	real_argb_color color{};
	interface_get_real_argb_color(color_table_index, color_index, &color);

	real_argb_color shadow_color{};
	shadow_color.rgb = *global_real_rgb_black;
	shadow_color.alpha = color.alpha;

	draw_string->set_font(interface_font);
	draw_string->set_style(style);
	draw_string->set_justification(justification);
	draw_string->set_color(&color);
	draw_string->set_shadow_color(&shadow_color);
}
