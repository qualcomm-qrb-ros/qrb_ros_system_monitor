// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__UTILS_HPP_
#define QRB_ROS_SYSTEM_MONITOR__UTILS_HPP_

#include <array>
#include <memory>
#include <string>

std::string exec_command(const std::string & cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) {
    return "";
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

#endif  // QRB_ROS_SYSTEM_MONITOR__UTILS_HPP_