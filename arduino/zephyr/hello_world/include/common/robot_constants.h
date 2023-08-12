// Copyright (c) 2023, Henrik Söderlund

#pragma once

// #include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

namespace robot {
namespace constants {
constexpr struct RobotStepperSpec {
  struct gpio_dt_spec mot_enable_gpio;
  struct gpio_dt_spec mot_ustep1_gpio;
  struct gpio_dt_spec mot_ustep2_gpio;
  struct gpio_dt_spec mot_ustep3_gpio;
} kRobotStepperSpec = {
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_enable_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_ustep1_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_ustep2_gpios),
  GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, mot_ustep3_gpios),
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
