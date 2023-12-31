// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "balancing_robot_msgs/msg/can_message.hpp"

class MinimalSubscriber : public rclcpp::Node {
 public:
  MinimalSubscriber() : Node("minimal_subscriber") {
    subscription_ = create_subscription<balancing_robot_msgs::msg::CANMessage>(
      "can_data", 10,
      [this] (balancing_robot_msgs::msg::CANMessage::UniquePtr msg) {
      RCLCPP_INFO(get_logger(), "I heard: '%s'",
                  balancing_robot_msgs::msg::to_yaml(*msg).c_str());
    });
  }

 private:
  rclcpp::Subscription<balancing_robot_msgs::msg::CANMessage>::SharedPtr
    subscription_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
