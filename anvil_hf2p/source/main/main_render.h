#pragma once

enum e_main_pregame_frame;
void __fastcall game_engine_render_frame_watermarks(bool pregame);
void __fastcall game_engine_render_frame_watermarks_anvil(bool pregame);
void __fastcall main_render_pregame(e_main_pregame_frame pregame_frame_type, const char* pregame_frame_text);