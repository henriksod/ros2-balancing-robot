// Copyright (c) 2023, Henrik Söderlund

#include "service_agents/stepper_agent.h"

namespace robot {
namespace stepper {
using robot::constants::RobotStepperSpec;

struct StepperAgent::Impl {
  explicit Impl(const RobotStepperSpec& stepper_spec)
    : stepper_spec(stepper_spec) {}

  void initialize() {
    gpio_pin_configure_dt(&stepper_spec.mot_enable_gpio, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&stepper_spec.mot_ustep1_gpio, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&stepper_spec.mot_ustep2_gpio, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&stepper_spec.mot_ustep3_gpio, GPIO_OUTPUT_INACTIVE);
  }

  void setMicroStep(uint8_t u_step) {
    this->u_step = u_step;

    // input:                     1 2 4 8 16 32
    // uStep table corresponds to 0 1 2 3 4  5  in binary on uStep pins
    // So, we need to take the log2 of input
    uint8_t u_step_pow = 0;

    while (u_step >>= 1) u_step_pow++;

    gpio_pin_set_raw(stepper_spec.mot_ustep1_gpio.port,
                     stepper_spec.mot_ustep1_gpio.pin,
                     u_step_pow & 0x01);
    gpio_pin_set_raw(stepper_spec.mot_ustep2_gpio.port,
                     stepper_spec.mot_ustep2_gpio.pin,
                     u_step_pow & 0x02);
    gpio_pin_set_raw(stepper_spec.mot_ustep3_gpio.port,
                     stepper_spec.mot_ustep3_gpio.pin,
                     u_step_pow & 0x04);
  }

  const RobotStepperSpec& stepper_spec;
  uint8_t                 u_step = 1;
};

StepperAgent::StepperAgent(const RobotStepperSpec& stepper_spec)
  : pimpl(new Impl(stepper_spec)) {
  pimpl->initialize();
}

StepperAgent::~StepperAgent() = default;

uint8_t StepperAgent::getMicroStep() {
  return pimpl->u_step;
}

void StepperAgent::setMicroStep(uint8_t u_step) {
  pimpl->setMicroStep(u_step);
}
}  // namespace stepper
}  // namespace robot
