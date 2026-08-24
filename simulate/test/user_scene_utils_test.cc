#include "user_scene_utils.h"

#include <cassert>

int main()
{
    mjvGeom destination_geoms[2]{};
    mjvGeom user_geoms[1]{};
    user_geoms[0].type = mjGEOM_SPHERE;

    mjvScene destination{};
    destination.geoms = destination_geoms;
    destination.maxgeom = 2;
    destination.ngeom = 0;

    const int appended = support_visualization::appendUserGeoms(
        destination, user_geoms, 1);
    assert(appended == 1);
    assert(destination.ngeom == 1);
    assert(destination.geoms[0].type == mjGEOM_SPHERE);
}
