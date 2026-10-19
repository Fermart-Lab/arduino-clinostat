// Maintainer: JGFermart. Host tests for scheduling and bounded target selection.
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "../Stepperclinostat/Config.h"

struct FakeMotor {
  int32_t position = 0, target = 0;
  float speed = 0, acceleration = 0;
  unsigned calls = 0, targets = 0;
  bool stalled = false;
  void setMaxSpeed(float value) { speed = value; }
  void setAcceleration(float value) { acceleration = value; }
  void setCurrentPosition(int32_t value) { position = target = value; }
  void moveTo(int32_t value) { target = value; ++targets; }
  int32_t distanceToGo() const { return target - position; }
  int32_t targetPosition() const { return target; }
  void run() {
    ++calls;
    if (!stalled) position += (target > position) - (target < position);
  }
};

using clinostat::AxisConfig;
using clinostat::AxisController;
using clinostat::MotionMode;

void sweep_and_units() {
  FakeMotor first, second;
  AxisController<FakeMotor> a(first, AXIS_1, 1), b(second, AXIS_2, 2);
  assert(a.begin(0) && b.begin(0));
  assert(fabs(first.speed - 200.0f) < 0.01f);
  assert(fabs(second.speed - 533.3333f) < 0.01f);
  assert(first.target == 200 && second.target == 89);
  unsigned reversals = 0;
  int32_t oldTarget = second.target;
  for (uint32_t t = 0; t < 2000; ++t) {
    a.update(t); b.update(t);
    assert(first.position >= 0 && first.position <= 200);
    assert(second.position >= 0 && second.position <= 89);
    if (second.target != oldTarget) { ++reversals; oldTarget = second.target; }
  }
  assert(reversals > 10);
  assert(first.calls == 2000 && second.calls == 2000);
}

void independent_dwell() {
  AxisConfig slow = {200, 0, 2, 10, 100, 1000, 5000, MotionMode::Sweep};
  AxisConfig fast = {200, 0, 20, 10, 100, 0, 5000, MotionMode::Sweep};
  FakeMotor first, second;
  AxisController<FakeMotor> a(first, slow, 1), b(second, fast, 2);
  assert(a.begin(0) && b.begin(0));
  for (uint32_t t = 0; t < 1000; ++t) { a.update(t); b.update(t); }
  assert(first.position == 2 && first.target == 2);
  assert(second.targets > 40 && second.calls == 1000);
  a.update(1000); assert(first.target == 2);
  a.update(1001); assert(first.target == 0);
}

void random_cadence_and_overrun() {
  AxisConfig cfg = {200, 0, 100, 10, 100, 0, 100, MotionMode::Random};
  FakeMotor motor;
  AxisController<FakeMotor> axis(motor, cfg, 123);
  assert(axis.begin(0));
  const int32_t initial = motor.target;
  assert(initial > 0 && initial <= 100);
  motor.stalled = true;
  axis.update(100); axis.update(250);
  assert(motor.targets == 1);  // Busy slots do not retarget a moving axis.
  motor.position = motor.target;
  axis.update(299); assert(motor.targets == 1);
  axis.update(300); assert(motor.targets == 2);
  motor.position = motor.target;
  axis.update(1050); assert(motor.targets == 3);  // No burst after missed calls.
  motor.position = motor.target;
  axis.update(1099); assert(motor.targets == 3);
  axis.update(1100); assert(motor.targets == 4);
}

void repeatable_bounded_random_targets() {
  AxisConfig cfg = {800, -20, 89, 40, 500, 0, 10, MotionMode::Random};
  FakeMotor x, y, z;
  AxisController<FakeMotor> a(x, cfg, 42), b(y, cfg, 42), c(z, cfg, 43);
  assert(a.begin(0) && b.begin(0) && c.begin(0));
  unsigned differentSeeds = 0, changes = 0;
  int32_t previous = x.target;
  for (uint32_t t = 10; t <= 10000; t += 10) {
    x.position = x.target; y.position = y.target; z.position = z.target;
    a.update(t); b.update(t); c.update(t);
    assert(x.target >= -20 && x.target <= 89);
    assert(y.target == x.target);
    if (z.target != x.target) ++differentSeeds;
    if (previous != x.target) ++changes;
    previous = x.target;
  }
  assert(differentSeeds > 900 && changes > 900);
}

void timer_rollover() {
  AxisConfig cfg = {200, 0, 1, 10, 100, 20, 20, MotionMode::Sweep};
  FakeMotor motor;
  AxisController<FakeMotor> axis(motor, cfg, 1);
  const uint32_t start = UINT32_MAX - 10;
  assert(axis.begin(start));
  axis.update(start); assert(motor.target == 1);
  axis.update(8); assert(motor.target == 1);
  axis.update(9); assert(motor.target == 0);

  cfg.mode = MotionMode::Random;
  FakeMotor other;
  AxisController<FakeMotor> randomAxis(other, cfg, 1);
  assert(randomAxis.begin(start));
  other.position = other.target;
  randomAxis.update(8); assert(other.targets == 1);
  randomAxis.update(9); assert(other.targets == 2);
}

void invalid_settings() {
  for (unsigned problem = 0; problem < 9; ++problem) {
    AxisConfig cfg = AXIS_1;
    switch (problem) {
      case 0: cfg.stepsPerRevolution = 0; break;
      case 1: cfg.upperSteps = cfg.lowerSteps; break;
      case 2: cfg.lowerSteps = 1; break;
      case 3: cfg.maximumRpm = 0; break;
      case 4: cfg.maximumRpm = HUGE_VALF; break;
      case 5: cfg.accelerationStepsPerSecond2 = -1; break;
      case 6: cfg.endpointDwellMs = 0x80000000UL; break;
      case 7: cfg.mode = MotionMode::Random; cfg.randomIntervalMs = 0; break;
      case 8: cfg.upperSteps = 1000001; break;
    }
    FakeMotor motor;
    AxisController<FakeMotor> axis(motor, cfg, 1);
    assert(!axis.begin(0));
    axis.update(1000);
    assert(motor.targets == 0 && motor.calls == 0);
  }
}

int main() {
  sweep_and_units();
  independent_dwell();
  random_cadence_and_overrun();
  repeatable_bounded_random_targets();
  timer_rollover();
  invalid_settings();
  puts("All six controller test groups passed.");
}
