// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "qrb_ros_system_monitor/battery_monitor.hpp"
#include "qrb_ros_system_monitor/cpu_monitor.hpp"
#include "qrb_ros_system_monitor/disk_monitor.hpp"
#include "qrb_ros_system_monitor/memory_monitor.hpp"
#include "qrb_ros_system_monitor/swap_monitor.hpp"
#include "qrb_ros_system_monitor/system_info_server.hpp"
#include "qrb_ros_system_monitor/temperature_monitor.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char const * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::NodeOptions options;
  options.use_intra_process_comms(true);

  auto cpu_monitor = std::make_shared<qrb_ros_system_monitor::CpuMonitor>(options);
  exec.add_node(cpu_monitor);

  auto memory_monitor = std::make_shared<qrb_ros_system_monitor::MemoryMonitor>(options);
  exec.add_node(memory_monitor);

  auto temp_monitor = std::make_shared<qrb_ros_system_monitor::TemperatureMonitor>(options);
  exec.add_node(temp_monitor);

  auto disk_monitor = std::make_shared<qrb_ros_system_monitor::DiskMonitor>(options);
  exec.add_node(disk_monitor);

  auto swap_monitor = std::make_shared<qrb_ros_system_monitor::SwapMonitor>(options);
  exec.add_node(swap_monitor);

  auto battery_monitor = std::make_shared<qrb_ros_system_monitor::BatteryMonitor>(options);
  exec.add_node(battery_monitor);

  auto system_info_server = std::make_shared<qrb_ros_system_monitor::SystemInfoServer>(options);
  exec.add_node(system_info_server);

  exec.spin();
  rclcpp::shutdown();

  return 0;
}
