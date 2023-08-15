// Copyright (c) 2023, Henrik Söderlund

#include <zephyr/drivers/counter.h>

#include "common/assert.h"
#include "service_agents/stepper_agent.h"

namespace robot {
namespace agent {
namespace stepper {
using robot::constants::RobotStepperSpec;
using robot::driver::stepper::StepperController;

struct StepperAgent::Impl {
  Impl(std::string name,
       const RobotStepperSpec& stepper_spec,
       const StepperController& stepper_controller_left,
       const StepperController& stepper_controller_right)
    : name(name), stepper_spec(stepper_spec) {}

  ~Impl() = default;

  void initialize() {
    /// Initialize counter timer
    ASSERT_MSG(!device_is_ready(stepper_spec.timer),
               "Timer device is not ready!");

    ASSERT_MSG(!counter_start(stepper_spec.timer),
               "Could not start counter timer!");

    /// Initialize shared stepper pins
    gpio_pin_configure_dt(&stepper_spec.mot_enable_gpio, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&stepper_spec.mot_ustep1_gpio, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&stepper_spec.mot_ustep2_gpio, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&stepper_spec.mot_ustep3_gpio, GPIO_OUTPUT_INACTIVE);
  }

  void setMicroStepImpl(uint8_t u_step) {
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

  const std::string       name;
  const RobotStepperSpec& stepper_spec;
  uint8_t                 u_step = 1;
};

StepperAgent::StepperAgent(
  std::string name,
  const RobotStepperSpec& stepper_spec,
  const StepperController& stepper_controller_left,
  const StepperController& stepper_controller_right,
  InternalConstructorBadge internal)
  : _pimpl(std::make_unique<Impl>(std::move(name), stepper_spec, stepper_controller_left,
                                  stepper_controller_right)) {
  _pimpl->initialize();
}

StepperAgent::~StepperAgent() noexcept = default;

std::unique_ptr<StepperAgent>StepperAgent::create(
  std::string name,
  const RobotStepperSpec& stepper_spec,
  const StepperController& stepper_controller_left,
  const StepperController& stepper_controller_right) {
  return std::make_unique<StepperAgent>(name,
                                        stepper_spec,
                                        stepper_controller_left,
                                        stepper_controller_right,
                                        InternalConstructorBadge{});
}

std::string StepperAgent::getName() const {
  return _pimpl->name;
}

uint8_t StepperAgent::getMicroStep() const {
  return _pimpl->u_step;
}

void StepperAgent::setMicroStep(uint8_t u_step) {
  _pimpl->setMicroStepImpl(u_step);
}
}  // namespace stepper
}  // namespace agent
}  // namespace robot
