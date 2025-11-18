// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__BATTERY_MONITOR_HPP_
#define QRB_ROS_SYSTEM_MONITOR__BATTERY_MONITOR_HPP_

#include "qrb_ros_system_monitor/monitor_node.hpp"
#include "std_msgs/msg/float32.hpp"

namespace qrb_ros_system_monitor
{

class BatteryMonitor : public MonitorNode
{
public:
  explicit BatteryMonitor(const rclcpp::NodeOptions & options);

protected:
  void on_timer();

private:
  void get_battery_info(std_msgs::msg::Float32 & info);
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string power_capacity_sysfs_path_{ "" };
};

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__BATTERY_MONITOR_HPP_