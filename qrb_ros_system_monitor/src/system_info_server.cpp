// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/system_info_server.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "qrb_ros_system_monitor/filesystem_info.hpp"
#include "qrb_ros_system_monitor/utils.hpp"

namespace qrb_ros_system_monitor
{

SystemInfoServer::SystemInfoServer(const rclcpp::NodeOptions & options)
  : Node("system_info_server", options)
{
  RCLCPP_INFO(this->get_logger(), "System info server start");
  srv_ = this->create_service<qrb_ros_system_monitor_interfaces::srv::SystemInfo>(
      "system_info_server", std::bind(&SystemInfoServer::system_info, this, std::placeholders::_1,
                                std::placeholders::_2));
}

void SystemInfoServer::system_info(
    const std::shared_ptr<qrb_ros_system_monitor_interfaces::srv::SystemInfo::Request> request,
    std::shared_ptr<qrb_ros_system_monitor_interfaces::srv::SystemInfo::Response> response)
{
  (void)request;
  get_system_info(*response);
}

void SystemInfoServer::get_system_info(
    qrb_ros_system_monitor_interfaces::srv::SystemInfo::Response & resp)
{
  std::string res = exec_command("nproc");
  std::istringstream iss(res);
  iss >> resp.cpu_count;

  std::string unused;
  res = exec_command("cat /proc/meminfo | grep MemTotal");
  std::istringstream iss_mem(res);
  iss_mem >> unused >> resp.mem_total;
  resp.mem_total /= 1024;

  res = exec_command("cat /proc/meminfo | grep SwapTotal");
  std::istringstream iss_swap(res);
  iss_swap >> unused >> resp.swap_total;
  resp.swap_total /= 1024;

  auto list = get_filesystem_list();
  if (list.empty()) {
    RCLCPP_ERROR(this->get_logger(), "get filesystem list error");
    return;
  }

  // only count local physical disks
  for (const auto & item : list) {
    if (item.type == "tmpfs" || item.type == "overlay" || is_network_filesystem(item.type)) {
      continue;
    }
    resp.disk_total += item.total;
  }
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::SystemInfoServer)