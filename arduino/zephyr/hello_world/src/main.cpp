/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pinctrl.h>

#include "common/robot_constants.h"
#include "service_agents/stepper_agent.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
constexpr struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void) {
  int ret;

  robot::stepper::StepperAgent stepper_agent{ robot::constants::kRobotStepperSpec };

  gpio_pin_configure_dt(&kLEDSpec.led1_gpio, GPIO_OUTPUT_ACTIVE);

  stepper_agent.setMicroStep(8);
  printk("Micro Step is %d\n", stepper_agent.getMicroStep());

  if (!gpio_is_ready_dt(&led)) {
    return 0;
  }

  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

  if (ret < 0) {
    return 0;
  }

  while (1) {
    printk("Hello World! %s\n",  CONFIG_BOARD);
    printk("Micro Step is %d\n", stepper_agent.getMicroStep());
    ret = gpio_pin_toggle_dt(&led);

    gpio_pin_toggle_dt(&kLEDSpec.led1_gpio);

    if (ret < 0) {
      return 0;
    }
    k_msleep(SLEEP_TIME_MS);
  }

  return 0;
}
