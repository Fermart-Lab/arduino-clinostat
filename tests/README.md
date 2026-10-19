# Software verification

Maintained by JGFermart.

## Controller tests

The host tests cover six groups: endpoint bounds and speed conversion; independent dwell; random cadence and busy-slot handling; repeatable bounded random targets; timer rollover; and invalid configurations. A fake motor isolates controller scheduling from the library's physical step timing.

Run from the repository root with a C++11 compiler:

```sh
c++ -std=c++11 -Wall -Wextra -Werror tests/test_axis_controller.cpp -o controller-tests
./controller-tests
```

On Windows, use `controller-tests.exe` as the output name and run that executable. Keep assertions enabled (do not define `NDEBUG`).

## Arduino UNO builds

Install Arduino AVR Boards **1.8.6** and AccelStepper **1.64.0**. With Arduino CLI:

```sh
arduino-cli core install arduino:avr@1.8.6
arduino-cli lib install "AccelStepper@1.64.0"
arduino-cli compile --fqbn arduino:avr:uno --warnings all Stepperclinostat
```

Compile the remaining combinations by adding one of these options to the last command:

```text
--build-property "compiler.cpp.extra_flags=-DCLINOSTAT_RANDOM_MODE=1"
--build-property "compiler.cpp.extra_flags=-DCLINOSTAT_STEP_DIR=1"
--build-property "compiler.cpp.extra_flags=-DCLINOSTAT_STEP_DIR=1 -DCLINOSTAT_RANDOM_MODE=1"
```

The archived sketch uses Stepper **1.1.3** and can be compiled separately from `archive/Stepperclinostat`.

## Results — 19 October 2026

All six host-test groups passed. All four UNO combinations compiled successfully. The default revision uses 9,210 bytes of flash and 519 bytes of static RAM. Compiler warnings were limited to unused parameters in the Arduino core. The original sketch also compiled successfully with Stepper 1.1.3.

These checks do not validate electrical compatibility, mechanical travel, step accuracy or operation on a physical clinostat.
