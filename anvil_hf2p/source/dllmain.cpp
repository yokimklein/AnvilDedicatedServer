#include <iostream>
#include <windows.h>
#include <anvil\server_tools.h>
#include <anvil\build_version.h>
#include <anvil\hooks\hooks.h>
#include <anvil\config.h>

void anvil_initialize()
{
    printf("%s\n\n", anvil_build_version_string());
    printf("base address: %p\n\n", base_address<void*>());
    anvil_patches_apply();
    anvil_hooks_apply();
    anvil_load_configuration();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    static FILE* f;
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
#ifdef EVENTS_ENABLED
        // allocate console
        AllocConsole();
        setlocale(LC_ALL, "");
        freopen_s(&f, "CONOUT$", "w", stdout);
#endif
        SetProcessDPIAware(); // disable dpi scaling
        DisableThreadLibraryCalls(hModule); // disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH (reduces the working set size)
        anvil_initialize();
        return true;

    case DLL_PROCESS_DETACH:
        fclose(f);
        FreeConsole();
        return FreeLibrary(hModule);

    case DLL_THREAD_ATTACH:
        return true;

    case DLL_THREAD_DETACH:
        return true;
    }
    return TRUE;
}

// needed for PE bear import table
extern "C" __declspec(dllexport) void null_export(void)
{
}