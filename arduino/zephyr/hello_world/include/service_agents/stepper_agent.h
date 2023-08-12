// Copyright (c) 2023, Henrik Söderlund

#pragma once

#include <stdint.h>

#include "common/unique_ptr.h"
#include "common/robot_constants.h"

namespace robot {
namespace stepper {
class StepperAgent {
 public:
  explicit StepperAgent(const robot::constants::RobotStepperSpec& stepper_spec);
  ~StepperAgent();

  StepperAgent(const StepperAgent& other)   = delete;
  StepperAgent& operator=(StepperAgent rhs) = delete;

  uint8_t       getMicroStep();
  void          setMicroStep(uint8_t u_step);

 private:
  class Impl;
  std::unique_ptr<Impl>pimpl;
};
}  // namespace stepper
}  // namespace robot
