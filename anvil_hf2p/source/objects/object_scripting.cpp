#include "object_scripting.h"

void __fastcall object_scripting_clear_all_function_variables(datum_index object_index)
{
	INVOKE(ADDRESS_OBJECT_SCRIPTING_CLEAR_ALL_FUNCTION_VARIABLES, object_scripting_clear_all_function_variables, object_index);
}