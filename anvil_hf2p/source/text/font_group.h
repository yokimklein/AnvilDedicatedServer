#pragma once

struct s_font_header;
struct s_kerning_pair;
struct s_font_character;
long font_get_line_height(const s_font_header* header);
void font_header_byteswap(s_font_header* header);
bool font_header_validate(const s_font_header* header);
void font_kerning_pairs_byteswap(s_kerning_pair* kerning_pairs, long kerning_pair_count);
bool font_character_validate(const s_font_character* character);