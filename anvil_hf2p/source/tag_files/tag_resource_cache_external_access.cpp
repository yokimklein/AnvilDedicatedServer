#include "tag_resource_cache_external_access.h"
#include "memory\data.h"

long tag_resource_cache_handle_get_external_handle(datum_index index)
{
    return BUILD_DATUM_INDEX(~DATUM_INDEX_TO_IDENTIFIER(index), DATUM_INDEX_TO_ABSOLUTE_INDEX(index));
}
