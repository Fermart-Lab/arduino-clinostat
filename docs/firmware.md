# Firmware behavior

AccelStepper revision by [JGFermart](https://github.com/JGFermart). The [original sketch and its behavior](original-firmware.md) are preserved separately.

## Motion modes

Both axes are serviced on every loop through AccelStepper's `run()` function. Acceleration and deceleration apply to each move.

- **Sweep:** alternate between `lowerSteps` and `upperSteps`. An optional endpoint dwell affects only that axis. The default dwell is zero; velocity still reaches zero at each reversal.
- **Random:** select an initial target at startup, then select new targets on a fixed schedule. If an axis is still moving when a slot arrives, skip that slot and finish its current move. Missed slots do not cause a burst of commands. A target can equal the previous target.

Set `CLINOSTAT_RANDOM_MODE` to `0` for sweep or `1` for random in `Config.h`. To use different modes on the two axes, replace each configuration's `DEFAULT_MODE` with `clinostat::MotionMode::Sweep` or `clinostat::MotionMode::Random`.

Random targets use separate reproducible pseudorandom sequences. The default seeds are 1 and 2. Resetting repeats the sequence for each axis; timing and skipped slots affect when targets are issued. Change the seeds to use different sequences.

## Configuration

Settings are in [Config.h](../Stepperclinostat/Config.h). Values below are examples for this revision, not the published experiment's recorded parameters.

| Setting | Axis 1 | Axis 2 | Units / meaning |
| --- | --- | --- | --- |
| `stepsPerRevolution` | 200 | 800 | Commanded steps per configured revolution |
| `lowerSteps` | 0 | 0 | Lower absolute target relative to startup |
| `upperSteps` | 200 | 89 | Upper absolute target relative to startup |
| `maximumRpm` | 60 | 40 | Maximum configured RPM |
| `accelerationStepsPerSecond2` | 200 | 533.3333 | Steps/s² |
| `endpointDwellMs` | 0 | 0 | Sweep endpoint pause, milliseconds |
| `randomIntervalMs` | 5000 | 5000 | Random target slots, milliseconds |

Maximum speed is converted to steps/s using `maximumRpm * stepsPerRevolution / 60`, giving 200 and approximately 533.33 steps/s. Short movements may not reach this maximum. In the configured angular model, the default ranges are 360° and 40.05°; physical frame angles depend on microstepping and transmission calibration. Targets do not wrap modulo a revolution.

Startup defines the current position as zero without homing or feedback. Both limits must include zero, be ordered, and lie within −1,000,000 to +1,000,000 steps. Speed, acceleration and steps per revolution must be positive; speed and acceleration must be finite. Timing values must be below 2³¹ milliseconds, and the random interval must be nonzero. Invalid motion settings leave both outputs disabled and print an error.

Choose the electrical interface as described in the [hardware guide](hardware.md). The default is the two-pin phase interface. Motors must be connected through suitable drivers, not directly to Arduino pins.

## Serial output

At **9600 baud**, records contain `milliseconds,axis1_steps,axis2_steps`, with signed commanded positions. The nominal record interval is 500 ms. Logging is skipped when the transmit buffer lacks space so that it does not wait for serial transmission.

Positions are software step counts, not encoder measurements. Motion starts after setup; there is no serial start/stop command. Opening a serial connection can reset an UNO and restart the program.

## Verification

See [test results and reproduction commands](../tests/README.md). Software checks cover scheduling and target bounds; no motor operation or electrical validation has been performed. Library semantics are documented in the [AccelStepper reference](https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html).
