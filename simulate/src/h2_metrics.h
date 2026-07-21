#pragma once

#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>

#include <mujoco/mujoco.h>

class H2ContactMetrics
{
public:
  void initialize(const std::string& output_dir)
  {
    if (output_dir.empty()) return;
    std::filesystem::create_directories(output_dir);
    output_.open(std::filesystem::path(output_dir) / "contacts.csv", std::ios::trunc);
    output_ << "time,body1,body2,distance,normal_force,severe,classification\n";
  }

  void record(const mjModel* model, const mjData* data)
  {
    if (!output_.is_open()) return;
    for (int i = 0; i < data->ncon; ++i)
    {
      const mjContact& contact = data->contact[i];
      const int body1_id = model->geom_bodyid[contact.geom1];
      const int body2_id = model->geom_bodyid[contact.geom2];
      const char* body1_name = mj_id2name(model, mjOBJ_BODY, body1_id);
      const char* body2_name = mj_id2name(model, mjOBJ_BODY, body2_id);
      const std::string body1 = body1_name ? body1_name : "world";
      const std::string body2 = body2_name ? body2_name : "world";
      const std::string classification = classify(body1, body2);
      mjtNum force[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      mj_contactForce(model, data, i, force);
      const double normal_force = std::abs(force[0]);
      const bool severe = classification != "foot_contact" && normal_force >= 50.0;
      output_ << data->time << ',' << body1 << ',' << body2 << ','
              << contact.dist << ',' << normal_force << ',' << severe << ','
              << classification << '\n';
    }
  }

private:
  static bool is_foot(const std::string& body)
  {
    return body == "left_ankle_pitch_link" || body == "right_ankle_pitch_link";
  }

  static bool is_torso(const std::string& body)
  {
    return body == "pelvis" || body == "torso_link";
  }

  static std::string classify(const std::string& body1, const std::string& body2)
  {
    if (is_torso(body1) || is_torso(body2)) return "torso_contact";
    if (is_foot(body1) || is_foot(body2)) return "foot_contact";
    if (body1 == "world" || body2 == "world") return "non_foot_ground_contact";
    return "self_contact";
  }

  std::ofstream output_;
};
