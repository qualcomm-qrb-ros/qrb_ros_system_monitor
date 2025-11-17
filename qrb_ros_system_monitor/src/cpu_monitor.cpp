// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/cpu_monitor.hpp"

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

CpuMonitor::CpuMonitor(const rclcpp::NodeOptions & options) : MonitorNode("cpu_monitor", options)
{
  RCLCPP_INFO(this->get_logger(), "CPU Monitor start");
  pub_ = this->create_publisher<qrb_ros_system_monitor_interfaces::msg::CpuInfo>("cpu", 10);
  timer_ =
      create_wall_timer(std::chrono::seconds(interval_), std::bind(&CpuMonitor::on_timer, this));
}

void CpuMonitor::on_timer()
{
  auto msg = std::make_unique<qrb_ros_system_monitor_interfaces::msg::CpuInfo>();
  get_cpu_info(*msg);
  pub_->publish(std::move(msg));
}

void CpuMonitor::get_cpu_info(qrb_ros_system_monitor_interfaces::msg::CpuInfo & info)
{
  std::ifstream file("/proc/stat");
  if (!file.is_open()) {
    RCLCPP_ERROR(this->get_logger(), "open /proc/stat error");
    return;
  }
  std::string line;
  getline(file, line);
  file.close();

  std::istringstream iss(line);
  std::string s_cpu;

  iss >> s_cpu >> info.user >> info.nice >> info.system >> info.idle >> info.iowait >> info.irq >>
      info.softirq >> info.steal >> info.guest >> info.guest_nice;

  if (last_ != nullptr) {
    uint64_t dt_idle = (info.idle + info.iowait) - (last_->idle + last_->iowait);
    uint64_t dt_total = (info.user + info.nice + info.system + info.idle + info.iowait + info.irq +
                            info.softirq + info.steal) -
                        (last_->user + last_->nice + last_->system + last_->idle + last_->iowait +
                            last_->irq + last_->softirq + last_->steal);
    info.usage = 100.0 * (1 - dt_idle * 1.0f / dt_total);
  }
  last_ = std::make_shared<qrb_ros_system_monitor_interfaces::msg::CpuInfo>(info);
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::CpuMonitor)