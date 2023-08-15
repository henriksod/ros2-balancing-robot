// Copyright (c) 2023, Henrik Söderlund

#pragma once

#include "common/common.h"
#include "common/robot_constants.h"
#include "drivers/stepper_controller.h"

namespace robot {
namespace agent {
namespace stepper {
class StepperAgent {
  /// This badge prohibits the constructor of this class from being called
  /// via the external API. It forces the user to use the create function
  /// to instantiate an instance of this class wrapped in a unique_ptr.
  struct InternalConstructorBadge {
    InternalConstructorBadge() = default;
  };

 public:
  StepperAgent(
    std::string name,
    const robot::constants::RobotStepperSpec& stepper_spec,
    const robot::driver::stepper::StepperController& stepper_controller_left,
    const robot::driver::stepper::StepperController& stepper_controller_right,
    InternalConstructorBadge                         internal);
  ~StepperAgent() noexcept;

  StepperAgent(const StepperAgent& other)                         = delete;
  StepperAgent&                       operator=(StepperAgent rhs) = delete;

  /// Primary function for instantiation of this class
  static std::unique_ptr<StepperAgent>create(
    std::string name,
    const robot::constants::RobotStepperSpec& stepper_spec,
    const robot::driver::stepper::StepperController& stepper_controller_left,
    const robot::driver::stepper::StepperController& stepper_controller_right);

  /// Get the name of this stepper agent
  std::string getName() const;

  /// Get the current microstep
  uint8_t     getMicroStep() const;

  /// Set the microstep
  void        setMicroStep(uint8_t u_step);

 private:
  class Impl;
  std::unique_ptr<Impl>_pimpl;
};
}  // namespace stepper
}  // namespace agent
}  // namespace robot
