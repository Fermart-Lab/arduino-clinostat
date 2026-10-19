# Hardware and commissioning

## What is documented

The article’s [Construction of clinostat](https://www.nature.com/articles/s41467-026-69037-4#Sec17) describes an aluminum-profile structure, two perpendicular axes driven by stepper motors, and an Arduino UNO. It names AccelStepper and describes positional or random movement.

The [demonstration](https://www.youtube.com/watch?v=_n05wxybXdQ) provides a visual reference for the assembled apparatus. Motor numbering in the sketch has not been mapped to the physical frame and mold axes.

## Pin assignments in the source

| Software object | First control pin | Second control pin |
| --- | --- | --- |
| `motor1` | Arduino digital 6, named `DIR_PIN_1` | Arduino digital 7, named `PUL_PIN_1` |
| `motor2` | Arduino digital 8, named `DIR_PIN_2` | Arduino digital 9, named `PUL_PIN_2` |

