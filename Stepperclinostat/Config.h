// Maintainer: JGFermart. These are configurable examples, not recovered run settings.
#ifndef CLINOSTAT_CONFIG_H
#define CLINOSTAT_CONFIG_H

#include "AxisController.h"

// 0 retains the original two-pin phase interface; 1 selects STEP/DIR drivers.
#ifndef CLINOSTAT_STEP_DIR
#define CLINOSTAT_STEP_DIR 0
#endif

// 0 sweeps between endpoints; 1 chooses random targets at scheduled intervals.
#ifndef CLINOSTAT_RANDOM_MODE
#define CLINOSTAT_RANDOM_MODE 0
#endif

static_assert(CLINOSTAT_STEP_DIR == 0 || CLINOSTAT_STEP_DIR == 1,
              "CLINOSTAT_STEP_DIR must be 0 or 1");
static_assert(CLINOSTAT_RANDOM_MODE == 0 || CLINOSTAT_RANDOM_MODE == 1,
              "CLINOSTAT_RANDOM_MODE must be 0 or 1");

constexpr uint8_t DIR_PIN_1 = 6;
constexpr uint8_t PUL_PIN_1 = 7;
constexpr uint8_t DIR_PIN_2 = 8;
constexpr uint8_t PUL_PIN_2 = 9;
constexpr unsigned int STEP_PULSE_WIDTH_US = 5;

constexpr clinostat::MotionMode DEFAULT_MODE = CLINOSTAT_RANDOM_MODE
    ? clinostat::MotionMode::Random : clinostat::MotionMode::Sweep;

// steps/revolution, lower/upper steps, maximum RPM, acceleration (steps/s^2),
// endpoint dwell (ms), random target interval (ms), motion mode.
constexpr clinostat::AxisConfig AXIS_1 = {
    200, 0, 200, 60.0f, 200.0f, 0, 5000, DEFAULT_MODE};
constexpr clinostat::AxisConfig AXIS_2 = {
    800, 0, 89, 40.0f, 533.3333f, 0, 5000, DEFAULT_MODE};

constexpr uint32_t RANDOM_SEED_1 = 1;
constexpr uint32_t RANDOM_SEED_2 = 2;
constexpr uint32_t TELEMETRY_INTERVAL_MS = 500;

#endif
