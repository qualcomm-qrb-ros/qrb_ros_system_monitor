// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/disk_monitor.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "qrb_ros_system_monitor/utils.hpp"

using namespace std::chrono_literals;

namespace qrb_ros_system_monitor
{

DiskMonitor::DiskMonitor(const rclcpp::NodeOptions & options) : MonitorNode("disk_monitor", options)
{
  RCLCPP_INFO(this->get_logger(), "DISK Monitor start");
  pub_ = this->create_publisher<qrb_ros_system_monitor_interfaces::msg::DiskInfo>("disk", 10);
  timer_ =
      create_wall_timer(std::chrono::seconds(interval_), std::bind(&DiskMonitor::on_timer, this));
}

void DiskMonitor::on_timer()
{
  auto msg = std::make_unique<qrb_ros_system_monitor_interfaces::msg::DiskInfo>();
  get_disk_info(*msg);
  pub_->publish(std::move(msg));
}

void DiskMonitor::get_disk_info(qrb_ros_system_monitor_interfaces::msg::DiskInfo & info)
{
  std::string result = exec_command("df -BM -T");
  if (result.empty()) {
    RCLCPP_ERROR(this->get_logger(), "exec df -BM error");
    return;
  }

  std::istringstream iss(result);
  std::string s_filesystem, s_type, s_blocks, s_used, s_avaliable, s_use_rate, s_mount_on;
  if (!(iss >> s_filesystem >> s_type >> s_blocks >> s_used >> s_avaliable >> s_use_rate >>
          s_mount_on >> s_mount_on)) {
    RCLCPP_ERROR(this->get_logger(), "read exec stream error");
    return;
  }
  qrb_ros_system_monitor_interfaces::msg::FileSystem fs;
  std::string unused;
  while (iss >> fs.fs >> fs.type >> fs.total >> unused >> fs.used >> unused >> fs.avail >> unused >>
         fs.use_rate >> unused >> fs.mount) {
    info.fslist.emplace_back(fs);
  }
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::DiskMonitor)