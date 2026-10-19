// Maintainer: JGFermart. Distributed under the repository's MIT license.
#ifndef CLINOSTAT_AXIS_CONTROLLER_H
#define CLINOSTAT_AXIS_CONTROLLER_H

#include <stdint.h>
#include <math.h>

namespace clinostat {

enum class MotionMode : uint8_t { Sweep, Random };

struct AxisConfig {
  int32_t stepsPerRevolution;
  int32_t lowerSteps;
  int32_t upperSteps;
  float maximumRpm;
  float accelerationStepsPerSecond2;
  uint32_t endpointDwellMs;
  uint32_t randomIntervalMs;
  MotionMode mode;
};

// Each axis owns its schedule. Motor must provide the AccelStepper interface.
template <typename Motor>
class AxisController {
 public:
  AxisController(Motor& motor, const AxisConfig& config, uint32_t seed)
      : motor_(motor), config_(config), randomState_(seed ? seed : 1),
        ready_(false), dwelling_(false), arrivedAt_(0), lastSlotAt_(0) {}

  bool begin(uint32_t now) {
    ready_ = false;
    if (!validConfiguration()) return false;
    motor_.setMaxSpeed(config_.maximumRpm * config_.stepsPerRevolution / 60.0f);
    motor_.setAcceleration(config_.accelerationStepsPerSecond2);
    // Zero is the physical starting orientation, not a measured home position.
    motor_.setCurrentPosition(0);
    lastSlotAt_ = now;
    dwelling_ = false;
    ready_ = true;
    motor_.moveTo(config_.mode == MotionMode::Sweep
                      ? config_.upperSteps : randomTarget());
    return true;
  }

  void update(uint32_t now) {
    if (!ready_) return;
    motor_.run();

    if (config_.mode == MotionMode::Random) {
      const uint32_t elapsed = now - lastSlotAt_;
      if (elapsed >= config_.randomIntervalMs) {
        // Keep a fixed cadence, skip missed slots and never interrupt a move.
        lastSlotAt_ += elapsed - elapsed % config_.randomIntervalMs;
        if (motor_.distanceToGo() == 0) motor_.moveTo(randomTarget());
      }
      return;
    }

    if (motor_.distanceToGo() != 0) return;
    if (!dwelling_) {
      arrivedAt_ = now;
      dwelling_ = true;
    }
    if (static_cast<uint32_t>(now - arrivedAt_) >= config_.endpointDwellMs) {
      const int32_t next = motor_.targetPosition() == config_.upperSteps
                              ? config_.lowerSteps : config_.upperSteps;
      motor_.moveTo(next);
      dwelling_ = false;
    }
  }

 private:
  bool validConfiguration() const {
    const float speed = config_.maximumRpm * config_.stepsPerRevolution / 60.0f;
    return config_.stepsPerRevolution > 0 &&
           config_.lowerSteps >= -1000000 && config_.upperSteps <= 1000000 &&
           config_.lowerSteps < config_.upperSteps &&
           config_.lowerSteps <= 0 && config_.upperSteps >= 0 &&
           isfinite(speed) && speed > 0 &&
           isfinite(config_.accelerationStepsPerSecond2) &&
           config_.accelerationStepsPerSecond2 > 0 &&
           config_.endpointDwellMs < 0x80000000UL &&
           (config_.mode == MotionMode::Sweep ||
            (config_.mode == MotionMode::Random && config_.randomIntervalMs > 0 &&
             config_.randomIntervalMs < 0x80000000UL));
  }

  int32_t randomTarget() {
    // A separate reproducible pseudorandom sequence for each axis.
    randomState_ ^= randomState_ << 13;
    randomState_ ^= randomState_ >> 17;
    randomState_ ^= randomState_ << 5;
    const uint32_t count = static_cast<uint32_t>(
        config_.upperSteps - config_.lowerSteps + 1);
    return config_.lowerSteps + static_cast<int32_t>(randomState_ % count);
  }

  Motor& motor_;
  const AxisConfig config_;
  uint32_t randomState_;
  bool ready_;
  bool dwelling_;
  uint32_t arrivedAt_;
  uint32_t lastSlotAt_;
};

}  // namespace clinostat
#endif
