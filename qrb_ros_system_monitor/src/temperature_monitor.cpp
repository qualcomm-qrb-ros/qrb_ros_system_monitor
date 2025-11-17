// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/temperature_monitor.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

using namespace std::chrono_literals;

#define TEMPERATURE_SYSFS_PATH "/sys/class/thermal/thermal_zone0/temp"

namespace qrb_ros_system_monitor
{

TemperatureMonitor::TemperatureMonitor(const rclcpp::NodeOptions & options)
  : MonitorNode("temperature_monitor", options)
{
  RCLCPP_INFO(this->get_logger(), "Temperature Monitor start");
  pub_ = this->create_publisher<std_msgs::msg::Float32>("temperature", 10);
  timer_ = create_wall_timer(
      std::chrono::seconds(interval_), std::bind(&TemperatureMonitor::on_timer, this));
}

void TemperatureMonitor::on_timer()
{
  auto msg = std::make_unique<std_msgs::msg::Float32>();
  get_temperature_info(*msg);
  pub_->publish(std::move(msg));
}

void TemperatureMonitor::get_temperature_info(std_msgs::msg::Float32 & info)
{
  std::ifstream file(TEMPERATURE_SYSFS_PATH);
  if (!file.is_open()) {
    RCLCPP_ERROR(this->get_logger(), "open %s error", TEMPERATURE_SYSFS_PATH);
    return;
  }
  std::string line;
  getline(file, line);
  file.close();

  std::istringstream iss(line);
  float temp = 0;
  iss >> temp;
  info.data = temp / 1000;
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::TemperatureMonitor)