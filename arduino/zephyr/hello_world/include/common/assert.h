// Copyright (c) 2023, Henrik Söderlund

#pragma once

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "common/robot_constants.h"

#define ASSERT_PRINT(test) __ASSERT_LOC(test)
#define ASSERT_PRINT_MSG(fmt, ...) __ASSERT_MSG_INFO(fmt, ## __VA_ARGS__)

#define ASSERT_DIE k_panic

#define ACTIVATE_LEDS()                                                                   \
        gpio_pin_configure_dt(&robot::constants::kLEDSpec.led1_gpio, GPIO_OUTPUT_ACTIVE); \
        gpio_pin_configure_dt(&robot::constants::kLEDSpec.led2_gpio, GPIO_OUTPUT_ACTIVE); \
        gpio_pin_configure_dt(&robot::constants::kLEDSpec.led3_gpio, GPIO_OUTPUT_ACTIVE);

#define ASSERT(cond)            \
        do {                    \
          if (!(cond)) {        \
            ASSERT_PRINT(cond); \
            ACTIVATE_LEDS();    \
            ASSERT_DIE();       \
          }                     \
        } while (0)

#define ASSERT_MSG(cond, fmt, ...)                 \
        do {                                       \
          if (!(cond)) {                           \
            ASSERT_PRINT(cond);                    \
            ASSERT_PRINT_MSG(fmt, ## __VA_ARGS__); \
            ACTIVATE_LEDS();                       \
            ASSERT_DIE();                          \
          }                                        \
        } while (0)
