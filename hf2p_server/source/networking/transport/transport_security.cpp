#include <windows.h>
#include "transport_security.h"
#include "..\..\dllmain.h"

long transport_secure_address_get(GUID* transport_secure_address)
{
    byte* unknown_struct = (byte*)(module_base + 0x4EBE8F4);

    if (unknown_struct[221] && transport_secure_address)
    {
        // i fucking hate pointers
        memcpy(&transport_secure_address->Data1, &unknown_struct[222], 4);
        memcpy(&transport_secure_address->Data2, &unknown_struct[222 + 4], 2);
        memcpy(&transport_secure_address->Data3, &unknown_struct[222 + 6], 2);
        memcpy(&transport_secure_address->Data4, &unknown_struct[222 + 8], 8);
    }
    else
        return 0;
    return 1;
}