#pragma once

enum e_restricted_memory_region
{
	k_game_state_header_region = 0,
	k_game_state_update_region,
	k_game_state_render_region,
	k_game_state_shared_region,
	k_global_render_data_region,

	k_total_restricted_memory_regions,

	k_game_state_shared_region_mirror0 = k_game_state_shared_region + 1,
	k_game_state_shared_region_mirror1 = k_game_state_shared_region + 2,
};

bool restricted_region_locked_for_current_thread(long index);

extern long restricted_region_add_member(long index, const char* name, const char* type, unsigned int allocation, long alignment_bits, void(__cdecl* tls_update_callback)(void*), void(__cdecl* tls_pre_overwrite_fixup_callback)(void*), void(__cdecl* tls_post_copy_fixup_callback)(void*));
extern void* restricted_region_get_member_address(long index, long member_index);