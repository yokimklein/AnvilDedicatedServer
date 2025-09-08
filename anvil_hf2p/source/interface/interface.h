#pragma once
#include <cseries\cseries.h>

enum e_font_id;
enum e_text_style;
enum e_text_justification;
class c_draw_string;
union real_argb_color;
extern long interface_get_tag_index(short interface_tag_index);
extern real_argb_color* interface_get_real_argb_color(short interface_tag_index, short color_block_index, real_argb_color* color_out);
extern void interface_set_bitmap_text_draw_mode(c_draw_string* draw_string, e_font_id interface_font, e_text_style style, e_text_justification justification, ulong flags, short color_table_index, short color_index);