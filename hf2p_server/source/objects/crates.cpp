#include "crates.h"

bool crate_will_self_destruct(datum_index crate_index)
{
	s_crate_data* crate = (s_crate_data*)object_get(crate_index);
	return crate->self_destruct != -1;
}