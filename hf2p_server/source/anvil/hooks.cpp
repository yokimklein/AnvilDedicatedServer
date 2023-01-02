#include "hooks.h"
#include "patch\Patch.hpp"

#include "..\networking\session\network_session.h"
#include "..\networking\messages\network_message_handler.h"
#include "..\simulation\simulation_world.h"
#include "..\networking\messages\network_message_gateway.h"
#include "..\networking\logic\network_session_interface.h"
#include "..\networking\network_globals.h"
#include "backend.h"
#include "..\networking\logic\network_life_cycle.h"
#include "..\networking\transport\transport_shim.h"
#include "..\networking\session\network_managed_session.h"
#include "server_tools.h"
#include "..\simulation\simulation_debug_globals.h"

// add back missing message handlers
void __fastcall handle_out_of_band_message_hook(c_network_message_handler* message_handler, void* unused, s_transport_address const* address, e_network_message_type message_type, long message_storage_size, s_network_message const* message)
{
    message_handler->handle_out_of_band_message(address, message_type, message_storage_size, message);
}
void __fastcall handle_channel_message_hook(c_network_message_handler* message_handler, void* unused, c_network_channel* channel, e_network_message_type message_type, long message_storage_size, s_network_message const* stub_message)
{
    s_network_message const* message = (s_network_message const*)(module_base + 0x4FFB090);
    message_handler->handle_channel_message(channel, message_type, message_storage_size, message);
}

// create online squad when calling hf2p_setup_session
bool __fastcall create_local_online_squad(e_network_session_class ignore) // may have thiscall for life cycle
{
    return network_life_cycle_create_local_squad(_network_session_class_online);
}

// print to console whenever a message packet is sent
void __stdcall send_message_hook(void* stream, e_network_message_type message_type, long message_storage_size)
{
    FUNCTION_DEF(0x387A0, void, __stdcall, send_message, void* stream, e_network_message_type message_type, long message_storage_size);
    c_network_message_gateway* message_gateway = nullptr;
    c_network_session* session = g_network_session_manager->session[0];
    if (session != nullptr)
        message_gateway = session->m_message_gateway;

    if (message_gateway != nullptr)
        printf("SEND: %s\n", message_gateway->m_message_type_collection->get_message_type_name(message_type));
    return send_message(stream, message_type, message_storage_size);
}

// add back missing host code, process_pending_joins & check_to_send_membership_update
void __fastcall session_idle_hook(c_network_session* session)
{
    session->idle();
}

// I couldn't directly hook peer_request_properties_update without experiencing access violations, so this will do
// add back set_peer_address & set_peer_properties to peer_request_properties_update
void __fastcall network_session_update_peer_properties_hook(c_network_session* session, s_network_session_peer* peer)
{
    network_session_update_peer_properties(session, peer);
}

// add debug print back to before life cycle end is called in c_gui_location_manager::update
void __fastcall network_life_cycle_end_hook()
{
    printf("Resetting network location.  If you got here and didn't just issue a console command, this is a bug.\n");
    network_life_cycle_end();
}

// contrail gpu freeze fix
__declspec(naked) void contrail_fix_hook()
{
    __asm
    {
        add edx, [0x0068A38A]
        cmp edx, -1
        jg render
        push 0x68A3E3
        retn
        render :
        push 0x68A390
            retn
    }
}

// allow view establishment to progress past the connection phase
void __fastcall update_establishing_view_hook(c_simulation_world* simulation_world, void* unused, c_simulation_view* simulation_view)
{
    simulation_world->update_establishing_view(simulation_view);
}

// hook to prevent the game from adding a player to the host if we're running a dedicated server
void __fastcall peer_request_player_add_hook(c_network_session* session, void* unused, const s_player_identifier* player_identifier, long user_index, long controller_index, s_player_configuration_from_client* configuration_from_client, long voice_settings)
{
    if (!game_is_dedicated_server())
        session->peer_request_player_add(player_identifier, user_index, controller_index, configuration_from_client, voice_settings);
}
// ditto above
bool __fastcall network_session_interface_get_local_user_identifier_hook(s_player_identifier* player_identifier)
{
    bool(__thiscall* network_session_interface_get_local_user_identifier)(s_player_identifier* player_identifier) = reinterpret_cast<decltype(network_session_interface_get_local_user_identifier)>(module_base + 0x3D50);
    return !game_is_dedicated_server() && network_session_interface_get_local_user_identifier(player_identifier);
}

// reimplement network_session_check_properties by calling it at the end of network_session_interface_update_session
void __fastcall network_session_interface_update_session_hook(c_network_session* session)
{
    void(__thiscall* network_session_interface_update_session)(c_network_session* session) = reinterpret_cast<decltype(network_session_interface_update_session)>(module_base + 0x2F410);
    network_session_interface_update_session(session);

    if (session->established() && !session->leaving_session())
    {
        if (session->is_host())
        {
            network_session_check_properties(session);
        }
    }
}

int __cdecl vsnprintf_s_net_debug_hook(char* DstBuf, size_t SizeInBytes, size_t MaxCount, const char* Format, va_list ArgList)
{
    // original function call
    int result = vsnprintf_s(DstBuf, SizeInBytes, MaxCount, Format, ArgList);

    // check if we're building a URI - we don't want to print these
    if (strcmp(Format, "/%s.svc/%s") != 0)
    {
        printf("[+] %s \n", DstBuf);
    }
    // parse obfuscated URIs - temporarily disabled
    //else if (strcmp(Format, "Request %s") == 0 || strcmp(Format, "Response %s [%d|%d]") == 0)
    //{
    //    char resource_uri[0x100];
    //    memcpy(resource_uri, va_arg(ArgList, char*), 0x100); // occasional access violations here - TODO FIX
    //    backend_deobfuscate_uri(resource_uri, 0x100);
    //
    //    // potentially dangerous, but acceptable given the context?
    //    // these will return random data from memory for request logs
    //    // but they are ignored by the request format in snprintf
    //    long request_status = va_arg(ArgList, long); // more likely to be error code - response code
    //    long response_status = va_arg(ArgList, long);
    //
    //    char deobfuscated_log[0x100];
    //    memcpy(deobfuscated_log, DstBuf, 0x100);
    //    snprintf(deobfuscated_log, 0x100, Format, resource_uri, request_status, response_status);
    //
    //    printf("[+] %s \n", deobfuscated_log);
    //}

    return result;
}

bool __fastcall transport_secure_key_create_hook(s_transport_session_description* session_description)
{
    FUNCTION_DEF(0x3BC0, bool, __fastcall, transport_secure_key_create, s_transport_session_description* session_description);
    bool result = transport_secure_key_create(session_description);
    
    xnet_shim_table_add(&transport_security_globals->address, &session_description->host_address, &session_description->session_id);
    return result;
}

void __fastcall managed_session_delete_session_internal_hook(long managed_session_index, c_managed_session* managed_session)
{
    FUNCTION_DEF(0x28C30, void, __fastcall, managed_session_delete_session_internal, long managed_session_index, c_managed_session* managed_session);

    if ((managed_session->flags & 0x10) != 0 && managed_session->session_class == 1)
        xnet_shim_unregister_inaddr(&transport_security_globals->address);

    managed_session_delete_session_internal(managed_session_index, managed_session);
}

void __fastcall network_session_interface_set_local_name_hook(wchar_t const* machine_name, wchar_t const* session_name)
{
    FUNCTION_DEF(0x2E680, void, __fastcall, network_session_interface_set_local_name, wchar_t const* machine_name, wchar_t const* session_name);
    network_session_interface_set_local_name(k_anvil_machine_name, k_anvil_session_name);
}

void anvil_dedi_apply_patches()
{
    // enable tag edits
    Patch(0x082DB4, { 0xEB }).Apply();
    Patch::NopFill(Pointer::Base(0x083120), 2);
    Patch::NopFill(Pointer::Base(0x083AFC), 2);
    // contrail gpu freeze fix - twister
    Patch(0x1D6B70, { 0xC3 }).Apply();
    // enable netdebug
    g_network_interface_show_latency_and_framerate_metrics_on_chud = true;
    g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;
}

void anvil_dedi_apply_hooks()
{
    // add back missing host code by replacing existing stripped down functions
    Hook(0x25110, handle_out_of_band_message_hook).Apply();
    Hook(0x252F0, handle_channel_message_hook).Apply();
    Hook(0x2A580, network_join_process_joins_from_queue).Apply();
    Hook(0x21AB0, session_idle_hook).Apply();
    Hook(0x2F48E, network_session_update_peer_properties_hook, HookFlags::IsCall).Apply();
    Hook(0x3EEE1F, network_life_cycle_end_hook, HookFlags::IsCall).Apply();
    // set network_life_cycle_create_local_squad call in hf2p_setup_session to create an online session
    Hook(0x3AAF68, create_local_online_squad, HookFlags::IsCall).Apply();
    // allow view_establishment to progress past connection phase to established in update_establishing_view again
    Hook(0x370E0, update_establishing_view_hook).Apply();
    // add back network_session_check_properties
    Hook(0x2AD9E, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    Hook(0x2DC71, network_session_interface_update_session_hook, HookFlags::IsCall).Apply();
    // add/remove the host address & security keys to the xnet shim table on session creation/destruction so we can locate them with one another
    Hook(0x28E32, transport_secure_key_create_hook, HookFlags::IsCall).Apply();
    Hook(0x21342, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Hook(0x28051, managed_session_delete_session_internal_hook, HookFlags::IsCall).Apply();
    Pointer::Base(0x284B8).WriteJump(managed_session_delete_session_internal_hook, HookFlags::None);

    // TODO: hook hf2p_tick and disable everything but the heartbeat service, and reimplement whatever ms23 was doing, do the same for game_startup
    // prevent the game from adding a player to the dedicated host
    Hook(0x2F5AC, peer_request_player_add_hook, HookFlags::IsCall).Apply();
    Hook(0x212CC, network_session_interface_get_local_user_identifier_hook, HookFlags::IsCall).Apply();
    // set peer & session name
    Hook(0x3AA897, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    Hook(0x3AC21A, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    Hook(0x3AC243, network_session_interface_set_local_name_hook, HookFlags::IsCall).Apply();
    
    // output the message type for debugging
    Hook(0x233D4, send_message_hook, HookFlags::IsCall).Apply();
    // hook net_debug_print's vsnprintf_s call to print API logs to the console - temporarily disabled due to crashes
    //Hook(0x55D8BF, vsnprintf_s_net_debug_hook, HookFlags::IsCall).Apply();

    // contrail gpu freeze fix - twister
    Hook(0x28A38A, contrail_fix_hook).Apply();
}