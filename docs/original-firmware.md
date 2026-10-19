# Firmware behavior

This guide describes [the archived sketch](../archive/Stepperclinostat/Stepperclinostat.ino) as written. Its comments are retained for provenance, including comments that do not match the implementation.

## Parameters

| Setting | Default | Meaning in this sketch |
| --- | --- | --- |
| `STEPS_PER_REV_1` | `200` | Step count supplied to the first Stepper object |
| `STEPS_PER_REV_2` | `800` | Step count supplied to the second Stepper object |
| `DIR_PIN_1`, `PUL_PIN_1` | `6`, `7` | First object’s two control pins; names do not establish an electrical protocol |
| `DIR_PIN_2`, `PUL_PIN_2` | `8`, `9` | Second object’s two control pins |
| `speed1` | `60` | RPM argument to `motor1.setSpeed()` |
| `speed2` | `40` | RPM argument to `motor2.setSpeed()`, despite the degrees-per-minute comment |
| `dir1` | `0-1`, i.e. `-1` | Constant negative stepping direction for motor 1 |
| `dir2` | `1` | Initial value; reversed before motor 2’s first step |
| `angle1`, `angle2` | `0`, `0` | Software counters initialized at reset, not measured positions |
| Serial rate | `9600` baud | Diagnostic output only |
| Reversal pause | `1000` ms | Blocks the entire loop at either second-axis endpoint |

The [official Stepper implementation](https://github.com/arduino-libraries/Stepper/blob/master/src/Stepper.cpp) defines `setSpeed()` in RPM and implements the two-pin constructor using a four-state phase sequence. It is not a dedicated STEP/DIR pulse generator. `step()` blocks until its requested steps have been issued. These library semantics must be considered when interpreting this sketch’s pin names and speed settings.

## Loop sequence

Each iteration issues one negative step to motor 1, prints its counter and a hyphen, and increments that counter modulo 200. It then checks motor 2’s counter. At either 0 or `800 / 9` (integer value 88), it reverses motor 2’s direction and waits one second. It issues one step to motor 2, prints its old counter, and finally updates that counter modulo 800.

Both motors receive one step call per loop. The calls are sequential, and the shared pause and serial output affect both axes. The configured speed arguments therefore do not demonstrate independent frame speeds of 60 and 40 RPM.

## Second-axis reversal

At startup, `angle2 == 0`, so `dir2` changes from +1 to −1 before any second-axis step. The counter then follows:

```text
0 → 799 → 798 → ... → 88 → 89 → ... → 799 → 0 → 799 → ...
    negative direction       positive direction       negative
```

There are **712 commanded steps per traversal** between the endpoints. In the sketch’s 800-step-per-revolution model, that corresponds to **320.4°**, rather than the shorter 88-step path of **39.6°**. The long traversal repeats; it is not limited to startup. Actual frame travel also depends on the electrical interface, transmission and calibration, none of which are established by these constants alone.


## Serial output

Set Serial Monitor to **9600 baud**. The first logical lines are:

```text
0-0
1-799
2-798
3-797
```

The fields are the two software counters printed before their respective updates. Motor 1’s counter increases while its step direction is negative. Neither field is an encoder reading, and the output contains no timestamps. Opening a serial connection can reset an UNO and restart the sequence.
