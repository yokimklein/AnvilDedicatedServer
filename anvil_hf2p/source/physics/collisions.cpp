#include "collisions.h"
#include <objects\objects.h>

bool collision_test_vector(s_collision_test_flags flags, bool arg8, const real_point3d* point, const real_vector3d* vector, long first_ignore_object_index, long second_ignore_object_index, long third_ignore_object_index, collision_result* collision)
{
	size_t target = base_address<size_t>(0x1A1F80);
	bool return_value;

	__asm
	{
		push flags.object_flags
		push flags.collision_flags
		push collision
		push third_ignore_object_index
		push second_ignore_object_index
		push first_ignore_object_index
		push vector
		mov edx, point
		movzx ecx, arg8
		mov eax, target
		call eax
		add esp, 28
		mov return_value, al
	}

	return return_value;
}
