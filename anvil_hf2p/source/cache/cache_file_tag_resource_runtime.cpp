#include "cache_file_tag_resource_runtime.h"

//REFERENCE_DECLARE(0xEED978, c_cache_file_tag_resource_runtime_manager_allocation, g_resource_runtime_manager);

void __fastcall tag_resources_lock_game(long& locked)
{
	INVOKE(0x7E7E0, tag_resources_lock_game, locked);
	//if (g_resource_runtime_manager.get()->locked_for_game_UGLY())
	//{
	//	return 0;
	//}
	//
	//g_resource_runtime_manager.get()->lock_for_game();
	//return 1;
}

void __fastcall tag_resources_unlock_game(long& locked)
{
	INVOKE(0x7E830, tag_resources_unlock_game, locked);
}

//void c_cache_file_tag_resource_runtime_manager::lock_for_game()
//{
//	if (m_cache_file_resource_gestalt)
//	{
//		c_tag_resource_thread_access::lock_for_current_thread(&m_threaded_tag_resource_cache.c_tag_resource_thread_access);
//	}
//}
//
//bool c_cache_file_tag_resource_runtime_manager::locked_for_game_UGLY() const
//{
//	if (m_cache_file_resource_gestalt)
//	{
//		return c_tag_resource_thread_access::current_thread_has_access(&m_threaded_tag_resource_cache.c_tag_resource_thread_access);
//	}
//	return false;
//}
