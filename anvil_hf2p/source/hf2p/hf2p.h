#pragma once

struct s_transport_secure_identifier;

void hf2p_initialize();
void game_startup_main();
void game_startup_client();
void game_startup_dedicated_server();
void game_startup_bot_client();
void game_startup_sapien();
bool __fastcall hf2p_session_invalid(s_transport_secure_identifier* session_id);