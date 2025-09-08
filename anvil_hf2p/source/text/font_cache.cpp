#include "font_cache.h"
#include "memory\tls.h"
#include "game\game.h"
#include "multithreading\synchronization.h"
#include <stdlib.h>
#include "text\font_loading.h"
#include "text\font_package_cache.h"
#include "text\font_fallback.h"

REFERENCE_DECLARE(0x4067900, s_font_cache_globals, g_internal_font_cache_globals);

c_font_cache_mt_safe::c_font_cache_mt_safe()
	: c_font_cache_base()
	, m_locked()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(base_address(0xD80A3C));
}

c_font_cache_mt_safe::~c_font_cache_mt_safe()
{
}

c_font_cache_scope_lock::c_font_cache_scope_lock()
	: m_locked(false)
{
	TLS_DATA_GET_VALUE_REFERENCE(g_font_cache_globals);
	if (!g_font_cache_globals)
	{
		if (game_is_multithreaded())
		{
			internal_critical_section_enter(k_crit_section_font_cache);
		}

		m_locked = true;
		g_font_cache_globals = &g_internal_font_cache_globals;
	}
}

c_font_cache_scope_lock::~c_font_cache_scope_lock()
{
	if (m_locked)
	{
		TLS_DATA_GET_VALUE_REFERENCE(g_font_cache_globals);
		g_font_cache_globals = NULL;
		if (game_is_multithreaded())
		{
			internal_critical_section_leave(k_crit_section_font_cache);
		}
		m_locked = false;
	}
}

static c_font_cache_base::c_font_cache_base__vftable vftable = { NULL, NULL, NULL, NULL, NULL, NULL};

c_font_cache_base::c_font_cache_base() :
	__vftable(reinterpret_cast<decltype(__vftable)>(&vftable))
{
}

void __fastcall font_cache_new()
{
	INVOKE(0x16A880, font_cache_new);
}

e_character_status font_cache_retrieve_character(ulong character_key, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count> flags, const s_font_character** out_character, const void** out_pixel_data)
{
	if (word(character_key >> 16) == (word)_font_index_fallback && fallback_font_get_character(e_utf32(character_key & 0xFFFF), out_character, out_pixel_data))
	{
		return _character_status_ready;
	}
	if (font_in_emergency_mode())
	{
		return _character_status_invalid;
	}
	const s_font_package_file_header* package_header = font_get_package_header_internal();
	if (package_header == NULL)
	{
		return _character_status_invalid;
	}
	// This is failing
	long package_index = font_package_table_find_character(package_header, character_key);
	if (package_index == NONE)
	{
		return _character_status_invalid;
	}
	const s_font_package* package = NULL;
	e_font_package_status status = font_package_get(package_index, flags, character_key, &package);
	if (status != _font_package_ready)
	{
		return status == _font_package_unavailable ? _character_status_invalid : _character_status_loading;
	}
	const s_font_character* character = font_package_get_character(package, character_key);
	if (character == NULL)
	{
		return _character_status_invalid;
	}
	*out_character = character;
	*out_pixel_data = &character[1];
	return _character_status_ready;
}
