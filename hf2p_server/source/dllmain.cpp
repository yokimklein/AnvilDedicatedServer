#include <iostream>
#include <windows.h>
#include <anvil\server_tools.h>

BOOL WINAPI DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    static FILE* f;
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        setlocale(LC_ALL, "");
        freopen_s(&f, "CONOUT$", "w", stdout);
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