//
// Created by bandera on 19.03.16.
//

#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

#include "StargazerTypes.h"

inline bool readConfig(std::string cfgfile, camera_params_t &camera_intrinsics,
                       landmark_map_t &landmarks) {
  YAML::Node config = YAML::LoadFile(cfgfile);

  if (config["CameraIntrinsics"]) {
    camera_intrinsics[(int)INTRINSICS::f] =
        config["CameraIntrinsics"]["f"].as<double>();
    camera_intrinsics[(int)INTRINSICS::u0] =
        config["CameraIntrinsics"]["u0"].as<double>();
    camera_intrinsics[(int)INTRINSICS::v0] =
        config["CameraIntrinsics"]["v0"].as<double>();
    camera_intrinsics[(int)INTRINSICS::alpha] =
        config["CameraIntrinsics"]["alpha"].as<double>();
    camera_intrinsics[(int)INTRINSICS::beta] =
        config["CameraIntrinsics"]["beta"].as<double>();
    camera_intrinsics[(int)INTRINSICS::theta] =
        config["CameraIntrinsics"]["theta"].as<double>();
  } else {
    std::cerr << "Config file is missing camera_intrinsics!" << std::endl;
    return false;
  }

  if (config["Landmarks"]) {
    for (int i = 0; i < config["Landmarks"].size(); i++) {
      auto lm = config["Landmarks"][i];
      int id = lm["HexID"].as<int>();
      pose_t lm_pose;
      lm_pose[(int)POSE::X] = lm["x"].as<double>();
      lm_pose[(int)POSE::Y] = lm["y"].as<double>();
      lm_pose[(int)POSE::Z] = lm["z"].as<double>();
      lm_pose[(int)POSE::Rx] = lm["rx"].as<double>();
      lm_pose[(int)POSE::Ry] = lm["ry"].as<double>();
      lm_pose[(int)POSE::Rz] = lm["rz"].as<double>();
      landmarks[id] = Landmark(id);
      landmarks[id].pose = lm_pose;
    }
  } else {
    std::cerr << "Config file is missing landmarks!" << std::endl;
    return false;
  }

  return true;
}

inline bool writeConfig(std::string cfgfile,
                        const camera_params_t &camera_intrinsics,
                        const landmark_map_t &landmarks) {
  std::ofstream fout(cfgfile);

  fout << "CameraIntrinsics:\n";
  fout << " f: " << camera_intrinsics[(int)INTRINSICS::f] << "\n";
  fout << " u0: " << camera_intrinsics[(int)INTRINSICS::u0] << "\n";
  fout << " v0: " << camera_intrinsics[(int)INTRINSICS::v0] << "\n";
  fout << " alpha: " << camera_intrinsics[(int)INTRINSICS::alpha] << "\n";
  fout << " beta: " << camera_intrinsics[(int)INTRINSICS::beta] << "\n";
  fout << " theta: " << camera_intrinsics[(int)INTRINSICS::theta] << "\n";
  fout << "\n";
  fout << "Landmarks:\n";

  for (auto &entry : landmarks) {
    fout << " - {";
    fout << " HexID: "
         << "0x" << std::setfill('0') << std::setw(4) << std::hex << entry.first
         << std::setfill(' ');
    fout << ", x: " << std::setw(8) << entry.second.pose[(int)POSE::X];
    fout << ", y: " << std::setw(8) << entry.second.pose[(int)POSE::Y];
    fout << ", z: " << std::setw(8) << entry.second.pose[(int)POSE::Z];
    fout << ", rx: " << std::setw(8) << entry.second.pose[(int)POSE::Rx];
    fout << ", ry: " << std::setw(8) << entry.second.pose[(int)POSE::Ry];
    fout << ", rz: " << std::setw(8) << entry.second.pose[(int)POSE::Rz];
    fout << " }\n";
  }
  return true;
}