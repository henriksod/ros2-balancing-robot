// Copyright (c) 2023, Henrik Söderlund

#include <iostream>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
#include "std_msgs/msg/int32.hpp"

class MinimalSubscriber : public rclcpp::Node {
 public:
  MinimalSubscriber() : Node("minimal_subscriber") {
    RCLCPP_INFO(get_logger(), "I am alive");
    rclcpp::RosoutQoS rosout_qos;
    subscription_ = create_subscription<std_msgs::msg::Int32>(
      "std_msgs_msg_Int32", rosout_qos, [this](std_msgs::msg::Int32::UniquePtr msg) {
      RCLCPP_INFO(get_logger(), "I heard: '%d'", msg->data);
    });
  }

 private:
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
