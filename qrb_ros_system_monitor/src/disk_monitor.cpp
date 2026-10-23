// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/disk_monitor.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <utility>

#include "qrb_ros_system_monitor/filesystem_info.hpp"

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
  auto list = get_filesystem_list();
  if (list.empty()) {
    RCLCPP_ERROR(this->get_logger(), "get filesystem list error");
    return;
  }

  qrb_ros_system_monitor_interfaces::msg::FileSystem fs;
  for (const auto & item : list) {
    fs.fs = item.fs;
    fs.type = item.type;
    fs.total = item.total;
    fs.used = item.used;
    fs.avail = item.avail;
    fs.use_rate = item.use_rate;
    fs.mount = item.mount;
    info.fslist.emplace_back(fs);
  }
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::DiskMonitor)