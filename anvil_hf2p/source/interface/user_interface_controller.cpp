#include "user_interface_controller.h"
#include "cseries\cseries.h"

short __fastcall user_interface_controller_get_signed_in_controller_count()
{
    return INVOKE(0x3AD970, user_interface_controller_get_signed_in_controller_count);
}
