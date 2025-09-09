#include <iostream>
#include <windows.h>
#include <anvil\server_tools.h>
#include <config\version.h>
#include <anvil\hooks\hooks.h>
#include <anvil\config.h>
#include <main\main.h>
#include <cseries\cseries_events.h>

void anvil_initialize()
{
    printf("base address: %p\n\n", base_address<void*>());
    anvil_patches_apply();
    anvil_hooks_apply();
    anvil_load_configuration();
}

BOOL WINAPI HandlerRoutine(DWORD ctrlType)
{
    // call engine shutdown when closing console
    switch (ctrlType)
    {
        case CTRL_CLOSE_EVENT:
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        {
            // call the engine to exit and hang the thread until the main executable shuts down
            main_exit_game();
            WaitForSingleObject(GetCurrentProcess(), INFINITE);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    static FILE* f;
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
#ifdef EVENTS_ENABLED
            c_console::initialize(version_get_full_string());
            SetConsoleCtrlHandler(HandlerRoutine, TRUE);
            setlocale(LC_ALL, "");
#endif
            SetProcessDPIAware(); // disable dpi scaling
            DisableThreadLibraryCalls(hModule); // disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH (reduces the working set size)
            anvil_initialize();
            return true;
        }
        case DLL_PROCESS_DETACH:
        {
            c_console::dispose();
            return FreeLibrary(hModule);
        }
        case DLL_THREAD_ATTACH:
        {
            return true;
        }
        case DLL_THREAD_DETACH:
        {
            return true;
        }
    }
    return TRUE;
}

// needed for PE bear import table
extern "C" __declspec(dllexport) void null_export(void)
{
}