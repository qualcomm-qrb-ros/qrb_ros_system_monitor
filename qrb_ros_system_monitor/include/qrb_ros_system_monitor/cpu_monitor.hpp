// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__CPU_MONITOR_HPP_
#define QRB_ROS_SYSTEM_MONITOR__CPU_MONITOR_HPP_

#include "qrb_ros_system_monitor/monitor_node.hpp"
#include "qrb_ros_system_monitor_interfaces/msg/cpu_info.hpp"

namespace qrb_ros_system_monitor
{

class CpuMonitor : public MonitorNode
{
public:
  explicit CpuMonitor(const rclcpp::NodeOptions & options);

protected:
  void on_timer();

private:
  void get_cpu_info(qrb_ros_system_monitor_interfaces::msg::CpuInfo & info);
  rclcpp::Publisher<qrb_ros_system_monitor_interfaces::msg::CpuInfo>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::shared_ptr<qrb_ros_system_monitor_interfaces::msg::CpuInfo> last_;
};

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__CPU_MONITOR_HPP_