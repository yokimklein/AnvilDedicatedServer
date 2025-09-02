#include "language.h"
#include <cseries\cseries.h>

e_language __cdecl get_current_language()
{
	return INVOKE(0xB0C00, get_current_language);
}