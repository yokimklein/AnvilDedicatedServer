#include "font_package_cache.h"
#include "cseries\cseries.h"
#include "text\font_loading.h"
#include "algorithms\binary_search.h"

#pragma runtime_checks("", off)
e_font_package_status __fastcall font_package_get(long package_index, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count> flags, ulong character_key, const s_font_package** out_package)
{
    // character_key was optimised away
    e_font_package_status status = DECLFUNC(0x16A370, e_font_package_status, __fastcall, long, void*, c_flags<e_font_cache_flags, ulong, k_font_cache_flag_count>, const s_font_package**)(package_index, NULL, flags, out_package);
    __asm add esp, 8; // Fix usercall & cleanup stack
    return status;
}
#pragma runtime_checks("", restore)

const s_font_character* __fastcall font_package_get_character(const s_font_package* font_package, ulong character_key)
{
    return INVOKE(0x16B9C0, font_package_get_character, font_package, character_key);
}

long __cdecl package_table_search_function(void const* a1, void const* a2, const void* a3)
{
    return INVOKE(0x16B980, package_table_search_function, a1, a2, a3);
}

long font_package_table_find_character(const s_font_package_file_header* package_header, ulong character_key)
{
    return (long)binary_search_elements(
        (void*)character_key,
        package_header + package_header->package_table_offset,
        package_header->package_table_count,
        8,
        package_table_search_function,
        0);
}

void __fastcall font_package_cache_new()
{
    INVOKE(0x16A050, font_package_cache_new);
}
