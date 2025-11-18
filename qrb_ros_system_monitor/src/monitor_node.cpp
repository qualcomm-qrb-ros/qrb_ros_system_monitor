// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/monitor_node.hpp"

namespace qrb_ros_system_monitor
{
MonitorNode::MonitorNode(const std::string & node_name, const rclcpp::NodeOptions & options)
  : Node(node_name, options)
{
  interval_ = this->declare_parameter<int>("interval", 5);
}
}  // namespace qrb_ros_system_monitor