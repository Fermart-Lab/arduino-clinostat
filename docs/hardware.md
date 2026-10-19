# Hardware and commissioning

## What is documented

The article’s [Construction of clinostat](https://www.nature.com/articles/s41467-026-69037-4#Sec17) describes an aluminum-profile structure, two perpendicular axes driven by stepper motors, and an Arduino UNO. It names AccelStepper and describes positional or random movement.

The [demonstration](https://www.youtube.com/watch?v=_n05wxybXdQ) provides a visual reference for the assembled apparatus. Motor numbering in the sketch has not been mapped to the physical frame and mold axes.

## Pin assignments in the source

| Software object | First control pin | Second control pin |
| --- | --- | --- |
| `motor1` | Arduino digital 6, named `DIR_PIN_1` | Arduino digital 7, named `PUL_PIN_1` |
| `motor2` | Arduino digital 8, named `DIR_PIN_2` | Arduino digital 9, named `PUL_PIN_2` |


## Select the interface

`CLINOSTAT_STEP_DIR` in `Config.h` selects the AccelStepper interface:

| Value | Interface | Pin interpretation |
| --- | --- | --- |
| `0` (default) | `FULL2WIRE` | Two phase-control signals per motor, retaining the original interface type and pin order |
| `1` | `DRIVER` | STEP on 7/9 and DIR on 6/8; AccelStepper receives STEP first, then DIR |

The original names `DIR_PIN` and `PUL_PIN` do not establish the installed driver's protocol. Confirm the driver model, input voltage levels, grounding, pulse timing, microstep settings and transmission before selecting a mode. Use suitable motor drivers; Arduino pins do not power motor windings. STEP/DIR mode uses a configurable minimum pulse width of 5 µs; match it to the driver specification.

Check direction and travel at low speed with the apparatus unloaded. Calibrate steps per frame revolution and choose endpoints within the mechanical and cable limits. The firmware has no homing, limit-switch or encoder input.
