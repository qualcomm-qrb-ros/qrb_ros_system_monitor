// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__DISK_MONITOR_HPP_
#define QRB_ROS_SYSTEM_MONITOR__DISK_MONITOR_HPP_

#include "qrb_ros_system_monitor/monitor_node.hpp"
#include "qrb_ros_system_monitor_interfaces/msg/disk_info.hpp"

namespace qrb_ros_system_monitor
{

class DiskMonitor : public MonitorNode
{
public:
  explicit DiskMonitor(const rclcpp::NodeOptions & options);

protected:
  void on_timer();

private:
  void get_disk_info(qrb_ros_system_monitor_interfaces::msg::DiskInfo & info);
  rclcpp::Publisher<qrb_ros_system_monitor_interfaces::msg::DiskInfo>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__DISK_MONITOR_HPP_