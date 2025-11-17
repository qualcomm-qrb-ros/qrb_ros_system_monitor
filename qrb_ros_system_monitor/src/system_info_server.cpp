// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/system_info_server.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

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

  std::string result = exec_command("df -BM -l -x tmpfs -x overlay -x devtmpfs");
  if (result.empty()) {
    RCLCPP_ERROR(this->get_logger(), "exec df error");
    return;
  }

  std::istringstream iss_disk(result);
  std::string s_filesystem, s_blocks, s_used, s_avaliable, s_use_rate, s_mount_on;
  if (!(iss_disk >> s_filesystem >> s_blocks >> s_used >> s_avaliable >> s_use_rate >> s_mount_on >>
          unused)) {
    RCLCPP_ERROR(this->get_logger(), "read exec stream error");
    return;
  }
  uint64_t disk_size_m;
  while (iss_disk >> unused >> disk_size_m >> unused >> unused >> unused >> unused >> unused >>
         unused >> unused >> unused) {
    resp.disk_total += disk_size_m;
  }
}

}  // namespace qrb_ros_system_monitor

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(qrb_ros_system_monitor::SystemInfoServer)