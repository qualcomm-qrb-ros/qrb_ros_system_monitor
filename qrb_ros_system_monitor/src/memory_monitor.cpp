// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/memory_monitor.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

using namespace std::chrono_literals;

namespace qrb_ros_system_monitor
{

MemoryMonitor::MemoryMonitor(const rclcpp::NodeOptions & options)
  : MonitorNode("memory_monitor", options)
{
  RCLCPP_INFO(this->get_logger(), "Memory Monitor start");
  pub_ = this->create_publisher<qrb_ros_system_monitor_interfaces::msg::MemInfo>("memory", 10);
  timer_ =
      create_wall_timer(std::chrono::seconds(interval_), std::bind(&MemoryMonitor::on_timer, this));
}

void MemoryMonitor::on_timer()
{
  auto msg = std::make_unique<qrb_ros_system_monitor_interfaces::msg::MemInfo>();
  get_memory_info(*msg);
  pub_->publish(std::move(msg));
}

void MemoryMonitor::get_memory_info(qrb_ros_system_monitor_interfaces::msg::MemInfo & info)
{
  std::ifstream file("/proc/meminfo");
  if (!file.is_open()) {
    RCLCPP_ERROR(this->get_logger(), "open /proc/meminfo error");
    return;
  }
  std::string line;
  std::stringstream ss;
  while (getline(file, line)) {
    ss << line << "\n";
  }
  file.close();

  std::istringstream iss(ss.str());
  std::string field;
  uint64_t value;
  std::string unit;
  while (iss >> field >> value >> unit) {
    if (field == "MemTotal:") {
      info.mem_total = value;
    } else if (field == "MemFree:") {
      info.mem_free = value;
    } else if (field == "MemAvailable:") {
      info.mem_available = value;
    } else if (field == "Buffers:") {
      info.buffers = value;
    } else if (field == "Cached:") {
      info.cached = value;
    } else if (field == "SwapTotal:") {
      info.swap_total = value;
    } else if (field == "SwapFree:") {
      info.swap_free = value;
    }
  }
}
}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::MemoryMonitor)