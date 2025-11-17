// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__MONITOR_NODE_HPP_
#define QRB_ROS_SYSTEM_MONITOR__MONITOR_NODE_HPP_

#include "rclcpp/rclcpp.hpp"

namespace qrb_ros_system_monitor
{

class MonitorNode : public rclcpp::Node
{
public:
  explicit MonitorNode(const std::string & node_name, const rclcpp::NodeOptions & options);

protected:
  void on_timer();
  rclcpp::TimerBase::SharedPtr timer_;
  uint32_t interval_;
};

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__MONITOR_NODE_HPP_