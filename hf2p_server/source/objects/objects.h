#pragma once
#include "..\memory\data.h"

struct c_object_identifier
{
	datum_index unique_id;
	short origin_bsp_index;
	char type;
	char source;
};