#include "rasterizer\rasterizer.h"
#include <d3d9.h>

_D3DRENDERSTATETYPE c_rasterizer::x_last_render_state_types[4] = { D3DRS_COLORWRITEENABLE, D3DRS_COLORWRITEENABLE1, D3DRS_COLORWRITEENABLE2, D3DRS_COLORWRITEENABLE3 };

void c_rasterizer::set_indices(IDirect3DIndexBuffer9* index_buffer)
{
	if (index_buffer != g_current_index_buffer)
	{
		g_current_index_buffer = index_buffer;
		c_rasterizer::g_device->SetIndices(index_buffer);
	}
}

void __fastcall c_rasterizer::set_sampler_filter_mode_custom_device_no_cache(IDirect3DDevice9Ex* device, long sampler_index, e_sampler_filter_mode sampler_filter_mode)
{
	INVOKE(0x25BFF0, c_rasterizer::set_sampler_filter_mode_custom_device_no_cache, device, sampler_index, sampler_filter_mode);
}

void c_rasterizer::set_sampler_filter_mode(long sampler_index, e_sampler_filter_mode sampler_filter_mode)
{
	if (c_rasterizer::x_last_sampler_filter_modes[sampler_index] != sampler_filter_mode)
	{
		if (sampler_index < 20)
		{
			c_rasterizer::x_last_sampler_filter_modes[sampler_index] = sampler_filter_mode;
		}

		c_rasterizer::set_sampler_filter_mode_custom_device_no_cache(c_rasterizer::g_device, sampler_index, sampler_filter_mode);
	}

	if (c_rasterizer::g_render_force_anisotropic_level != -1)
	{
		if (sampler_index >= 16)
		{
			sampler_index += 0xF1;
		}

		c_rasterizer::g_device->SetSamplerState(sampler_index, D3DSAMP_MINFILTER, 3);
		c_rasterizer::g_device->SetSamplerState(sampler_index, D3DSAMP_MAGFILTER, 2);
		c_rasterizer::g_device->SetSamplerState(sampler_index, D3DSAMP_MIPFILTER, 2);
		c_rasterizer::g_device->SetSamplerState(sampler_index, D3DSAMP_MAXANISOTROPY, c_rasterizer::g_render_force_anisotropic_level);
	}
}

void c_rasterizer::set_color_write_enable(long target_index, long enable)
{
	if (enable == x_last_render_state_value[target_index])
	{
		return;
	}

	x_last_render_state_value[target_index] = enable;

	c_rasterizer::g_device->SetRenderState(x_last_render_state_types[target_index], enable);
}
