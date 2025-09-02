#include "crates.h"

bool crate_will_self_destruct(datum_index crate_index)
{
	crate_datum* crate = (crate_datum*)object_get_and_verify_type(crate_index, _object_mask_crate);
	return crate->crate.self_destruct != -1;
}