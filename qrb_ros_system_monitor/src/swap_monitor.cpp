// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/swap_monitor.hpp"

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

SwapMonitor::SwapMonitor(const rclcpp::NodeOptions & options) : MonitorNode("swap_monitor", options)
{
  RCLCPP_INFO(this->get_logger(), "Swap Monitor start");
  pub_ = this->create_publisher<qrb_ros_system_monitor_interfaces::msg::SwapInfo>("swap", 10);
  timer_ =
      create_wall_timer(std::chrono::seconds(interval_), std::bind(&SwapMonitor::on_timer, this));
}

void SwapMonitor::on_timer()
{
  auto msg = std::make_unique<qrb_ros_system_monitor_interfaces::msg::SwapInfo>();
  get_swap_info(*msg);
  pub_->publish(std::move(msg));
}

void SwapMonitor::get_swap_info(qrb_ros_system_monitor_interfaces::msg::SwapInfo & info)
{
  std::ifstream file("/proc/swaps");
  if (!file.is_open()) {
    RCLCPP_ERROR(this->get_logger(), "open /proc/swaps error");
    return;
  }
  std::string line;
  std::stringstream ss;
  while (getline(file, line)) {
    ss << line << "\n";
  }
  file.close();

  std::istringstream iss(ss.str());
  std::string s_file_name, s_type, s_size, s_used, s_priority;
  iss >> s_file_name >> s_type >> s_size >> s_used >> s_priority;

  qrb_ros_system_monitor_interfaces::msg::SwapItem item;
  while (iss >> item.file_name >> item.type >> item.size >> item.used >> item.priority) {
    info.swaplist.emplace_back(item);
  }
}
}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::SwapMonitor)