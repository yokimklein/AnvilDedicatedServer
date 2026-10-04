#include "user_interface_controller.h"
#include "cseries\cseries.h"

short __fastcall user_interface_controller_get_signed_in_controller_count()
{
    return INVOKE(ADDRESS_USER_INTERFACE_CONTROLLER_GET_SIGNED_IN_CONTROLLER_COUNT, user_interface_controller_get_signed_in_controller_count);
}
