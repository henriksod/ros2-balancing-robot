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

#include <chrono>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <cstdint>

// #include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "serial/serial.h"
#include "include/crc8.hpp"

#include "balancing_robot_msgs/msg/can_message.hpp"

using balancing_robot_msgs::msg::CANMessage;

namespace balancing_robot {
/* This example creates a subclass of Node and uses a fancy C++11 lambda
 * function to shorten the callback syntax, at the expense of making the
 * code somewhat more difficult to understand at first glance. */

class SerialCANReceiver : public rclcpp::Node {
 public:
  serial::Serial my_serial_;

  /// TODO: remove hardcoded values
  // port, baudrate, timeout in milliseconds
  SerialCANReceiver() : Node("serial_can_receiver"),
    my_serial_{"/dev/ttyACM0", 460800, serial::Timeout::simpleTimeout(1000)} {
    declare_parameter("callback_period_ms", 10);
    auto callback_period_ms = get_parameter("callback_period_ms").as_int();

    RCLCPP_INFO(get_logger(), "Available ports: '%s'",
                enumerate_ports().c_str());

    if (!my_serial_.isOpen()) {
      RCLCPP_ERROR(get_logger(), "ERROR, serial port is not open.");
      return;
    }

    auto timer_callback =
      [this] () {
        auto message = CANMessage();

        size_t num_available   = my_serial_.available();
        size_t prev_buffer_idx = buffer_idx;

        for (; buffer_idx < prev_buffer_idx + num_available;
             buffer_idx++) {
          if (buffer_idx >= buffer_size)
            break;
          buffer[buffer_idx] = my_serial_.read().front();
        }

        if (buffer_idx < buffer_size)
          return;

        buffer_idx = 0;

        std::string result(buffer, buffer_size);
        std::size_t foundAA = result.find(static_cast<char>(0xAA));

        if (foundAA == std::string::npos)  // If not found
          // AA
          return;

        std::size_t foundBB = result.find(static_cast<char>(0xBB),
                                          foundAA);

        if (foundBB == std::string::npos)  // If not found
          // BB
          return;

        // Anything other than 16 is an invalid size
        if (foundBB - foundAA != 16)
          return;

        result = result.substr(foundAA + 1, foundBB - 1);

        std::memcpy(&message.timestamp,
                    result.substr(0, 4).c_str(),
                    sizeof(int32_t));
        std::memcpy(&message.dlc,
                    result.substr(4, 1).c_str(),
                    sizeof(int8_t));
        std::memcpy(&message.arbitration_id,
                    result.substr(5, 4).c_str(),
                    sizeof(int8_t));
        std::string _payload =
          result.substr(9, message.dlc);
        std::copy(_payload.begin(),
                  _payload.end(),
                  message.payload.data());
        message.counter = message.payload[message.dlc - 2];
        message.crc     = message.payload[message.dlc - 1];
        message.invalid = !crc8::check_crc8(message.payload,
                                            message.dlc,
                                            message.crc);
        RCLCPP_DEBUG(get_logger(),
                     "Publishing: '%s'",
                     balancing_robot_msgs::msg::to_yaml(
                       message).c_str());
        publisher_->publish(message);

        // Flush input if we are delayed
        if (my_serial_.available() > 100) {
          RCLCPP_DEBUG(get_logger(),
                       "We are delayed, flush!");
          my_serial_.flushInput();
        }
      };

    publisher_ = create_publisher<CANMessage>("can_data", 10);
    timer_     =
      create_wall_timer(std::chrono::milliseconds(callback_period_ms),
                        timer_callback);
  }

 private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<CANMessage>::SharedPtr publisher_;

  static const size_t buffer_size = 20;
  size_t buffer_idx               = 0;
  char buffer[buffer_size]        = {};

  std::string enumerate_ports() {
    std::vector<serial::PortInfo> devices_found = serial::list_ports();

    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

    std::string devices_list = "";

    while (iter != devices_found.end()) {
      serial::PortInfo device = *iter++;

      devices_list += string_format("(%s, %s, %s)\n",
                                    device.port.c_str(),
                                    device.description.c_str(),
                                    device.hardware_id.c_str());
    }

    return devices_list;
  }

  /// TODO: Move to common header file
  template<typename ... Args>
  std::string string_format(const std::string& format, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;  // Extra

    // space
    // for
    // '\0'

    if (size_s <= 0)
      throw std::runtime_error("Error during formatting.");
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);  // We don't want the
                                                          // '\0' inside
  }

  std::string string_to_hex(const std::string& input) {
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;

    output.reserve(input.length() * 2);

    for (unsigned char c : input) {
      output.push_back(hex_digits[c >> 4]);
      output.push_back(hex_digits[c & 15]);
    }
    return output;
  }
};
}  // namespace balancing_robot

int main(int argc, char *argv[]) {
  std::cerr <<
    "If serial fails and you run WSL, run the following in PowerShell:\n"
            << "usbipd wsl list\n"
            << "usbipd wsl attach --busid BUSID\n";
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<balancing_robot::SerialCANReceiver>());
  rclcpp::shutdown();
  return 0;
}
