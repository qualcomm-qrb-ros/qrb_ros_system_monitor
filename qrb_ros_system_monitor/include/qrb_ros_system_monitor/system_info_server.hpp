// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__SYSTEM_INFO_SERVER_HPP_
#define QRB_ROS_SYSTEM_MONITOR__SYSTEM_INFO_SERVER_HPP_

#include "qrb_ros_system_monitor_interfaces/srv/system_info.hpp"
#include "rclcpp/rclcpp.hpp"

namespace qrb_ros_system_monitor
{

class SystemInfoServer : public rclcpp::Node
{
public:
  explicit SystemInfoServer(const rclcpp::NodeOptions & options);

protected:
  void system_info(
      const std::shared_ptr<qrb_ros_system_monitor_interfaces::srv::SystemInfo::Request> request,
      std::shared_ptr<qrb_ros_system_monitor_interfaces::srv::SystemInfo::Response> response);

private:
  void get_system_info(qrb_ros_system_monitor_interfaces::srv::SystemInfo::Response & info);
  rclcpp::Service<qrb_ros_system_monitor_interfaces::srv::SystemInfo>::SharedPtr srv_;
};

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__SYSTEM_INFO_SERVER_HPP_