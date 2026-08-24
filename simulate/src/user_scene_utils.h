#pragma once

#include <algorithm>
#include <cstring>

#include <mujoco/mujoco.h>

namespace support_visualization {

inline int appendUserGeoms(mjvScene& destination, const mjvGeom* user_geoms,
                           int user_count)
{
    if (user_geoms == nullptr || user_count <= 0 || destination.geoms == nullptr) {
        return 0;
    }

    const int available = std::max(0, destination.maxgeom - destination.ngeom);
    const int appended = std::min(user_count, available);
    if (appended > 0) {
        std::memcpy(destination.geoms + destination.ngeom, user_geoms,
                    appended * sizeof(mjvGeom));
        destination.ngeom += appended;
    }
    return appended;
}

}  // namespace support_visualization
