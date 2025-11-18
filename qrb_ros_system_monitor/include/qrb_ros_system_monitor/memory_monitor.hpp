// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__MEMORY_MONITOR_HPP_
#define QRB_ROS_SYSTEM_MONITOR__MEMORY_MONITOR_HPP_

#include "qrb_ros_system_monitor/monitor_node.hpp"
#include "qrb_ros_system_monitor_interfaces/msg/mem_info.hpp"

namespace qrb_ros_system_monitor
{

class MemoryMonitor : public MonitorNode
{
public:
  explicit MemoryMonitor(const rclcpp::NodeOptions & options);

protected:
  void on_timer();

private:
  void get_memory_info(qrb_ros_system_monitor_interfaces::msg::MemInfo & info);
  rclcpp::Publisher<qrb_ros_system_monitor_interfaces::msg::MemInfo>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__MEMORY_MONITOR_HPP_