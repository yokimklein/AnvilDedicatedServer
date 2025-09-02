#include "hooks_effect_system.h"
#include <anvil\hooks\hooks.h>
#include <cseries\cseries.h>
#include <effects\particle_states.h>
#include <xmmintrin.h>

// TODO: put this in a file with all the gpu_storage types :)
struct s_gpu_storage_2x16un
{
	static unsigned long pack_no_stream(__m128 data)
	{
		__m128 scaled_vec = _mm_sub_ps(_mm_mul_ps(data, _mm_set1_ps(65535.0f)), _mm_set1_ps(32767.0f));

		// skip clamping + intrinsic packing
		unsigned long packed_value = 0;
		*(((short*)&packed_value) + 0) = (short)scaled_vec.m128_f32[0];
		*(((short*)&packed_value) + 1) = (short)scaled_vec.m128_f32[1];
		return packed_value;
	}
};

void __fastcall write_particle_state_hook(s_particle_state* STATE, unsigned long index)
{
	// call original function
	INVOKE(0x590560, write_particle_state_hook, STATE, index);

	REFERENCE_DECLARE(0x242B628, unsigned int, g_particle_state_write_buffer);
	REFERENCE_DECLARE(0x2423E24, unsigned int, g_particle_state_write_buffer_stride);

	// write to anm2
	unsigned long* p = (unsigned long*)(g_particle_state_write_buffer + index * g_particle_state_write_buffer_stride + 0x40); // 0x40 = anm2
	*p = s_gpu_storage_2x16un::pack_no_stream(_mm_setr_ps(STATE->m_black_point, STATE->m_palette_v, 0.0f, 0.0f));
}

void anvil_hooks_effect_system_apply()
{
	// -- fix particle alpha_black_point and palette_v
	hook::call(0x59151B, write_particle_state_hook);
	hook::call(0x591628, write_particle_state_hook);
}