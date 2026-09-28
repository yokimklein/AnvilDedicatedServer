#include "path.h"

void* __fastcall ai_scratch_allocate()
{
	return INVOKE(0x6A3250, ai_scratch_allocate);
}

void __fastcall ai_scratch_free(void* memory)
{
	INVOKE(0x6A3300, ai_scratch_free, memory);
}

bool ai_point3d_new(const real_point3d* position, datum_index pathfinding_object_index, dword pathfinding_bsp_reference, c_ai_point3d* out_ai_point)
{
	size_t target = base_address<size_t>(0x6BD350);
	bool return_value;

	__asm
	{
		push out_ai_point
		push pathfinding_bsp_reference
		mov edx, pathfinding_object_index
		mov ecx, position
		mov eax, target
		call eax
		add esp, 8
		mov return_value, al
	}

	return return_value;
}

void path_state_new(const path_input* input, path_state* state, const path_error_info* error_info, void* debug, const special_movement* movement, const s_hint_penalty_cache* penalty_cache)
{
	size_t target = base_address<size_t>(0x72C180);

	__asm
	{
		push penalty_cache
		push movement
		push debug
		push error_info
		mov edx, state
		mov ecx, input
		mov eax, target
		call eax
		add esp, 16
	}
}

void __fastcall path_state_destination(path_state* state, const c_path_destination* destination)
{
	INVOKE(0x72C350, path_state_destination, state, destination);
}

bool __fastcall path_state_find(path_state* state)
{
	return INVOKE(0x72C680, path_state_find, state);
}

bool path_state_build_path(datum_index actor_index, path_state* state, path_result* result)
{
	size_t target = base_address<size_t>(0x72B4E0);
	bool return_value;

	__asm
	{
		push result
		mov edx, state
		mov ecx, actor_index
		mov eax, target
		call eax
		add esp, 4
		mov return_value, al
	}

	return return_value;
}

#pragma runtime_checks("", off)
c_sector_ref __fastcall collision_surface_get_sector(long structure_bsp_index, long surface_index, long instanced_geometry_instance_index, datum_index object_index, dword bsp_reference, const real_point3d* point)
{
	c_sector_ref sector_ref = INVOKE(0x6D0AE0, collision_surface_get_sector, structure_bsp_index, surface_index, instanced_geometry_instance_index, object_index, bsp_reference, point);
	__asm add esp, 16; // Cleanup stack
	return sector_ref;
}
#pragma runtime_checks("", restore)

void __fastcall object_get_pathfinding_location(datum_index object_index, c_sector_ref* sector_ref_out, c_ai_point3d* position_out)
{
	size_t target = base_address<size_t>(0x708230);

	__asm // fastcall with its own cleanup?
	{
		push position_out // push
		mov edx, sector_ref_out // first two
		mov ecx, object_index   // first two
		mov eax, target
		call eax
		add esp, 4
	}
}
