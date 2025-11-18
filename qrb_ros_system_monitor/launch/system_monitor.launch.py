# Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause-Clear

import launch
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    container = ComposableNodeContainer(
        name='qrb_ros_system_monitor_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='qrb_ros_system_monitor',
                plugin='qrb_ros_system_monitor::CpuMonitor',
                name='cpu_monitor'
            ),
        ],
        output='screen',
    )
    return launch.LaunchDescription([container])
