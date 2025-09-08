#include "render_visibility.h"
#include "visibility\visibility_collection.h"

bool render_sphere_visible(const real_point3d* center, real radius)
{
    return visibility_volume_test_sphere(&get_global_camera_collection()->m_input->region.projections[0].volume, center, radius);
}
