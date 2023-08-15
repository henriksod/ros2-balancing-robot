// Copyright (c) 2023, Henrik Söderlund

#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>

#include <string>

#include "common/time.h"
#include "common/common.h"
#include "common/robot_constants.h"

namespace robot {
namespace driver {
namespace stepper {
/// The maximum absolute speed that a stepper motor can go
constexpr float kMaxAbsSpeed{ 3U };

constexpr uint64_t kMaxRevsPerSec{ 1000U };

/// Number of micro steps possible
constexpr size_t kNumMicroSteps{ 6U };

/// How many steps per revolution for each microstep
constexpr uint16_t kMicroStepsPerRevolution[kNumMicroSteps] =
{ 200U, 400U, 800U, 1600U, 3200U, 6400U };

/// The maximum frequency of the stepper motor
constexpr time::Frequency kMaxFrequency[kNumMicroSteps] = {
  { kMaxRevsPerSec *kMicroStepsPerRevolution[0] },
  { kMaxRevsPerSec *kMicroStepsPerRevolution[1] },
  { kMaxRevsPerSec *kMicroStepsPerRevolution[2] },
  { kMaxRevsPerSec *kMicroStepsPerRevolution[3] },
  { kMaxRevsPerSec *kMicroStepsPerRevolution[4] },
  { kMaxRevsPerSec *kMicroStepsPerRevolution[5] },
};

/// The minimum frequency of the stepper motor (required so that the
/// timer can still tick)
constexpr time::Frequency kMinFrequency{ 20U };

/**
 * @brief Class that is used to control a stepper motor.\n
 **/
class StepperController {
  /// This badge prohibits the constructor of this class from being called
  /// via the external API. It forces the user to use the create function
  /// to instantiate an instance of this class wrapped in a unique_ptr.
  struct InternalConstructorBadge {
    InternalConstructorBadge() = default;
  };

 public:
  StepperController(
    const std::string& name,
    const robot::constants::RobotStepperSpec& stepper_spec,
    const robot::constants::RobotStepperControlSpec& stepper_control_spec,
    InternalConstructorBadge                         internal);

  ~StepperController() noexcept;

  StepperController(const StepperController& other)                         = delete;
  StepperController&                       operator=(StepperController rhs) = delete;

  /// Primary function for instantiation of this class
  static std::unique_ptr<StepperController>create(
    const std::string& name,
    const robot::constants::RobotStepperSpec& stepper_spec,
    const robot::constants::RobotStepperControlSpec& stepper_control_spec);

  /// Get the name of this stepper driver
  std::string getName() const;

  /// Set the speed of the stepper motor, at a specified micro step setting (1, 2, 4, 8, ...)
  void        setSpeed(float   speed,
                       uint8_t micro_step);

  /// Get the current speed of the stepper motor
  float              getSpeed() const;

  /// Override the step counter of the stepper motor
  void               setStep(int32_t step);

  /// Get the current step counter of the stepper motor
  int32_t            getStep() const;

  /// Get the direction of the stepper motor
  uint8_t            getDirection() const;

  /// Get the current frequency of the stepper motor
  time::Frequency    getFrequency() const;

  /// Get timer alarm configuration, used by timer interrupt function
  counter_alarm_cfg* getAlarmConfig() const;

  /// Get the stepper control specification
  const robot::constants::RobotStepperControlSpec&
  getStepperControlSpec() const;

 private:
  class Impl;
  std::unique_ptr<Impl>_pimpl;
};
}  // namespace stepper
}  // namespace driver
}  // namespace robot
