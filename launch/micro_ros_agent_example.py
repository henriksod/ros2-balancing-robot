# Copyright 2018 Open Source Robotics Foundation, Inc.

#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""Launch a talker and a listener."""
import launch
import launch_ros.actions
import yaml


def generate_launch_description():
    with open("arduino_config_data.yaml", "r") as f:
        config = yaml.safe_load(f)
    """Launch a talker and a listener."""
    return launch.LaunchDescription(
        [
            launch_ros.actions.Node(
                executable="external/com_github_micro_ros_micro_ros_agent/micro_ros_agent",
                output="screen",
                name="micro_ros_agent",
                arguments=[
                    "serial",
                    "--dev",
                    config["serial"],
                ],  # "--baudrate", config["baud_rate"]
            ),
            launch_ros.actions.Node(
                executable="nodes/micro_ros_listener_example/micro_ros_listener_example_task",
                output="screen",
                name="micro_ros_listener_example_task",
            ),
        ]
    )
