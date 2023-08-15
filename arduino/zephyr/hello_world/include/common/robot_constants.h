// Copyright (c) 2023, Henrik Söderlund

#pragma once

// #include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

namespace robot {
namespace constants {
constexpr struct RobotStepperSpec {
  const struct device *timer;
  struct gpio_dt_spec  mot_enable_gpio;
  struct gpio_dt_spec  mot_ustep1_gpio;
  struct gpio_dt_spec  mot_ustep2_gpio;
  struct gpio_dt_spec  mot_ustep3_gpio;
} kRobotStepperSpec = {
  DEVICE_DT_GET(DT_NODELABEL(tc1)),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_enable_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_ustep1_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_ustep2_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_ustep3_gpios),
};

struct RobotStepperControlSpec {
  uint8_t             timer_channel_id;
  struct gpio_dt_spec mot_step_gpio;
  struct gpio_dt_spec mot_dir_gpio;
  bool                reversed;
};

constexpr struct RobotStepperControlSpec kRobotStepperControlLeftSpec = {
  3,
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_step_left_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_dir_left_gpios),
  false,
};

constexpr struct RobotStepperControlSpec kRobotStepperControlRightSpec = {
  4,
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_step_right_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_dir_right_gpios),
  false,
};

constexpr struct LEDSpec {
  struct gpio_dt_spec led1_gpio;
  struct gpio_dt_spec led2_gpio;
  struct gpio_dt_spec led3_gpio;
} kLEDSpec = {
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, led1_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, led2_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, led3_gpios),
};
}  // namespace constants
}  // namespace robot
