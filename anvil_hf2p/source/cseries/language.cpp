#include "language.h"
#include <cseries\cseries.h>

e_language __cdecl get_current_language()
{
	return INVOKE(ADDRESS_GET_CURRENT_LANGUAGE, get_current_language);
}