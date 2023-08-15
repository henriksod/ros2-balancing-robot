// Copyright (c) 2023, Henrik Söderlund

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/counter.h>

#include "common/assert.h"
#include "common/time.h"
#include "drivers/stepper_controller.h"

namespace robot {
namespace driver {
namespace stepper {
using robot::constants::RobotStepperSpec;
using robot::constants::RobotStepperControlSpec;

constexpr uint64_t kTimerResolution = 8;

static void _stepper_tick_interrupt_callback(
  const struct device *counter_dev,
  uint8_t              chan_id,
  uint32_t             ticks,
  void                *user_data);

struct StepperController::Impl {
  explicit Impl(
    std::string name,
    const RobotStepperSpec& stepper_spec,
    const RobotStepperControlSpec& stepper_control_spec)
    : name(name), stepper_spec(stepper_spec), stepper_control_spec(stepper_control_spec) {}

  ~Impl() = default;

  void initialize() {
    /// Initialize alarm
    alarm_cfg.flags     = 0;
    alarm_cfg.ticks     = counter_us_to_ticks(stepper_spec.timer, time::Microseconds(frequency));
    alarm_cfg.callback  = _stepper_tick_interrupt_callback;
    alarm_cfg.user_data = this;

    ASSERT_MSG(!counter_set_channel_alarm(stepper_spec.timer,
                                          stepper_control_spec.timer_channel_id,
                                          &alarm_cfg),
               "Alarm %s could not be initialized!", stepper_control_spec.timer_channel_id);

    /// Initialize stepper pins
    gpio_pin_configure_dt(&stepper_control_spec.mot_step_gpio, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&stepper_control_spec.mot_dir_gpio,  GPIO_OUTPUT_INACTIVE);
  }

  void setSpeedImpl(float speed, uint8_t micro_step) {
    uint8_t last_direction = direction;
    float   absolute_speed;

    uint8_t u_step_pow = 0;

    while (micro_step >>= 1) u_step_pow++;

    float steps_per_rev =
      static_cast<float>(robot::driver::stepper::kMicroStepsPerRevolution[u_step_pow]);

    // Determine direction by speed
    if (speed > 0) {
      direction = stepper_control_spec.reversed ? -1 : 1;
    } else {
      direction = stepper_control_spec.reversed ? 1 : -1;
    }

    // Clip speed
    absolute_speed = abs(speed);

    if (absolute_speed > robot::driver::stepper::kMaxAbsSpeed) {
      absolute_speed = robot::driver::stepper::kMaxAbsSpeed;
    }

    // Only update dir pin if changed (it doesn't change very often)
    if (direction != last_direction) {
      gpio_pin_set_raw(stepper_control_spec.mot_dir_gpio.port,
                       stepper_control_spec.mot_dir_gpio.pin,
                       direction == 1 ? 1 : 0);
    }

    ASSERT(u_step_pow < robot::driver::stepper::kNumMicroSteps);

    frequency = std::clamp(
      steps_per_rev * absolute_speed * 2,
      static_cast<float>(robot::driver::stepper::kMinFrequency),
      static_cast<float>(robot::driver::stepper::kMaxFrequency[u_step_pow]));
  }

  const std::string        name;
  struct counter_alarm_cfg alarm_cfg;

  const RobotStepperSpec       & stepper_spec;
  const RobotStepperControlSpec& stepper_control_spec;
  time::Frequency                frequency{ robot::driver::stepper::kMaxFrequency[0] };
  float                          speed     = 0;
  uint8_t                        direction = 0;
  int32_t                        step      = 0;

  uint8_t _dir = 0;
};

StepperController::StepperController(
  std::string name,
  const RobotStepperSpec& stepper_spec,
  const RobotStepperControlSpec& stepper_control_spec,
  InternalConstructorBadge       internal)
  : _pimpl(std::make_unique<Impl>(std::move(name), stepper_spec, stepper_control_spec)) {
  _pimpl->initialize();
}

StepperController::~StepperController() noexcept = default;

std::unique_ptr<StepperController>StepperController::create(
  std::string name,
  const robot::constants::RobotStepperSpec& stepper_spec,
  const robot::constants::RobotStepperControlSpec& stepper_control_spec) {
  return std::make_unique<StepperController>(
    name, stepper_spec, stepper_control_spec, InternalConstructorBadge{});
}

std::string StepperController::getName() const {
  return _pimpl->name;
}

void StepperController::setSpeed(float speed, uint8_t micro_step) {
  _pimpl->setSpeedImpl(speed, micro_step);
}

float StepperController::getSpeed() const {
  return _pimpl->speed;
}

void StepperController::setStep(int32_t step) {
  _pimpl->step = step;
}

int32_t StepperController::getStep() const {
  return _pimpl->step;
}

uint8_t StepperController::getDirection() const {
  return _pimpl->direction;
}

time::Frequency StepperController::getFrequency() const {
  return _pimpl->frequency;
}

counter_alarm_cfg * StepperController::getAlarmConfig() const {
  return &_pimpl->alarm_cfg;
}

const robot::constants::RobotStepperControlSpec&
StepperController::getStepperControlSpec() const {
  return _pimpl->stepper_control_spec;
}

static void _stepper_tick_interrupt_callback(
  const struct device *counter_dev, uint8_t chan_id, uint32_t ticks, void *user_data) {
  StepperController *stepper_controller =
    reinterpret_cast<StepperController *>(user_data);
  struct counter_alarm_cfg *config =
    stepper_controller->getAlarmConfig();
  const robot::constants::RobotStepperControlSpec& stepper_control_spec =
    stepper_controller->getStepperControlSpec();

  // Manipulate the stepper pins
  if (abs(stepper_controller->getSpeed()) > 0) {
    int step_pin_state = gpio_pin_get_raw(stepper_control_spec.mot_step_gpio.port,
                                          stepper_control_spec.mot_step_gpio.pin);

    if (!step_pin_state) {
      // Step is made on rising edge
      stepper_controller->setStep(stepper_controller->getStep()
                                  + stepper_controller->getDirection());

      gpio_pin_set_raw(stepper_control_spec.mot_step_gpio.port,
                       stepper_control_spec.mot_step_gpio.pin,
                       1);
    } else {
      gpio_pin_set_raw(stepper_control_spec.mot_step_gpio.port,
                       stepper_control_spec.mot_step_gpio.pin,
                       0);
    }
  }

  // Reset the alarm at a new frequency
  config->ticks = counter_us_to_ticks(counter_dev,
                                      time::Microseconds(stepper_controller->getFrequency()));
  ASSERT_MSG(!counter_set_channel_alarm(counter_dev, stepper_control_spec.timer_channel_id, config),
             "Alarm %s could not be set!", stepper_control_spec.timer_channel_id);
}
}  // namespace stepper
}  // namespace driver
}  // namespace robot
