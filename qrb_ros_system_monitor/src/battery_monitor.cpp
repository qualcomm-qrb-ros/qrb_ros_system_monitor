// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/battery_monitor.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

using namespace std::chrono_literals;

// clang-format off
const std::string POWER_CAPACITY_SYSFS_PATHS[] = {
  "/sys/class/power_supply/battery/capacity",
  "/sys/class/power_supply/qcom-battmgr-bat/capacity",
};
// clang-format on

namespace qrb_ros_system_monitor
{

BatteryMonitor::BatteryMonitor(const rclcpp::NodeOptions & options)
  : MonitorNode("battery_monitor", options)
{
  RCLCPP_INFO(this->get_logger(), "Battery Monitor start");

  bool sysfs_path_exist = false;
  for (auto const & sysfs_path : POWER_CAPACITY_SYSFS_PATHS) {
    std::ifstream battery_capacity_file(sysfs_path);
    if (battery_capacity_file.is_open()) {
      power_capacity_sysfs_path_ = sysfs_path;
      sysfs_path_exist = true;
      break;
    }
  }
  if (!sysfs_path_exist) {
    RCLCPP_WARN(this->get_logger(), "no power capacity sysfs node found");
  } else {
    pub_ = this->create_publisher<std_msgs::msg::Float32>("battery", 10);
    timer_ = create_wall_timer(
        std::chrono::seconds(interval_), std::bind(&BatteryMonitor::on_timer, this));
  }
}

void BatteryMonitor::on_timer()
{
  auto msg = std::make_unique<std_msgs::msg::Float32>();
  get_battery_info(*msg);
  pub_->publish(std::move(msg));
}

void BatteryMonitor::get_battery_info(std_msgs::msg::Float32 & info)
{
  std::string line;

  std::ifstream battery_file(power_capacity_sysfs_path_);
  if (!battery_file.is_open()) {
    RCLCPP_ERROR(this->get_logger(), "open %s error", power_capacity_sysfs_path_.c_str());
    return;
  }
  line.clear();
  std::getline(battery_file, line);
  battery_file.close();

  std::istringstream battery_iss(line);
  battery_iss >> info.data;
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::BatteryMonitor)