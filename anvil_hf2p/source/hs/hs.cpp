#include "hs.h"
#include "cseries\cseries.h"
#include "hs\hs_function.h"

const hs_function_definition* hs_function_get(short function_index)
{
	ASSERT(function_index >= 0 && function_index < hs_function_table_count);
	return hs_function_table[function_index];
}