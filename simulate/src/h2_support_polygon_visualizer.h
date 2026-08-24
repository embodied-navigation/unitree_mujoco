#pragma once

#include "support_polygon.h"

#include <mujoco/mujoco.h>

#include <algorithm>
#include <array>
#include <string>
#include <unordered_set>
#include <vector>

class H2SupportPolygonVisualizer
{
public:
    void initialize(const mjModel* model, mjvScene& scene)
    {
        foot_geoms_.clear();
        for (int geom = 0; geom < model->ngeom; ++geom)
        {
            const char* name = mj_id2name(model, mjOBJ_GEOM, geom);
            if (!name) continue;
            const std::string value(name);
            if ((value.rfind("left_foot", 0) == 0 ||
                 value.rfind("right_foot", 0) == 0) &&
                value.find("_collision") != std::string::npos)
                foot_geoms_.insert(geom);
        }
        pelvis_body_ = mj_name2id(model, mjOBJ_BODY, "pelvis");
        scene.ngeom = 0;
    }

    std::string update(const mjModel* model, const mjData* data, mjvScene& scene)
    {
        scene.ngeom = 0;
        if (pelvis_body_ < 0 || foot_geoms_.empty()) return "SUPPORT: unavailable";

        std::vector<support_polygon::Point2> contacts;
        double ground_z = 0.0;
        for (int i = 0; i < data->ncon; ++i)
        {
            const auto& contact = data->contact[i];
            const bool geom1_foot = foot_geoms_.count(contact.geom1) != 0;
            const bool geom2_foot = foot_geoms_.count(contact.geom2) != 0;
            if (geom1_foot == geom2_foot) continue;
            const int other_geom = geom1_foot ? contact.geom2 : contact.geom1;
            if (model->geom_bodyid[other_geom] != 0) continue;
            contacts.push_back({contact.pos[0], contact.pos[1]});
            ground_z += contact.pos[2];
        }

        const std::size_t contact_count = contacts.size();
        const auto hull = support_polygon::convexHull(std::move(contacts));
        if (hull.size() < 3) return "SUPPORT: no polygon";
        ground_z /= static_cast<double>(contact_count);

        const mjtNum* com3 = data->subtree_com + 3 * pelvis_body_;
        const support_polygon::Point2 com{com3[0], com3[1]};
        const bool inside = support_polygon::contains(hull, com);
        const std::array<float, 4> status_color = inside
            ? std::array<float, 4>{0.1F, 0.9F, 0.2F, 1.0F}
            : std::array<float, 4>{0.95F, 0.1F, 0.1F, 1.0F};

        for (std::size_t i = 0; i < hull.size(); ++i)
        {
            const auto& a = hull[i];
            const auto& b = hull[(i + 1) % hull.size()];
            addLine(scene, {a.x, a.y, ground_z + 0.01},
                    {b.x, b.y, ground_z + 0.01}, status_color, 0.008);
        }
        addSphere(scene, {com.x, com.y, ground_z + 0.025}, status_color, 0.025);
        addLine(scene, {com.x, com.y, ground_z + 0.025},
                {com3[0], com3[1], com3[2]}, status_color, 0.006);
        return inside ? "COM: INSIDE support polygon" : "COM: OUTSIDE support polygon";
    }

private:
    static void addSphere(mjvScene& scene, const std::array<double, 3>& pos,
                          const std::array<float, 4>& color, double radius)
    {
        if (scene.ngeom >= scene.maxgeom) return;
        const mjtNum size[3]{radius, radius, radius};
        const mjtNum position[3]{pos[0], pos[1], pos[2]};
        const mjtNum matrix[9]{1, 0, 0, 0, 1, 0, 0, 0, 1};
        mjv_initGeom(&scene.geoms[scene.ngeom++], mjGEOM_SPHERE, size,
                     position, matrix, color.data());
    }

    static void addLine(mjvScene& scene, const std::array<double, 3>& from,
                        const std::array<double, 3>& to,
                        const std::array<float, 4>& color, double width)
    {
        if (scene.ngeom >= scene.maxgeom) return;
        const mjtNum size[3]{width, width, width};
        const mjtNum position[3]{0, 0, 0};
        const mjtNum matrix[9]{1, 0, 0, 0, 1, 0, 0, 0, 1};
        auto& geom = scene.geoms[scene.ngeom++];
        mjv_initGeom(&geom, mjGEOM_LINE, size, position, matrix, color.data());
        const mjtNum start[3]{from[0], from[1], from[2]};
        const mjtNum end[3]{to[0], to[1], to[2]};
        mjv_connector(&geom, mjGEOM_LINE, width, start, end);
    }

    std::unordered_set<int> foot_geoms_;
    int pelvis_body_ = -1;
};
