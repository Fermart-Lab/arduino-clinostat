# Arduino clinostat control

Arduino control firmware for the Fermart Lab two-axis clinostat used in biomaterial molding research.

The device repositions a mold while nickel-doped chitosan solidifies, helping the shrinking material remain in contact with the mold. See [the research article](https://doi.org/10.1038/s41467-026-69037-4), particularly Figure 3 and Methods → Construction of clinostat.

**Watch the apparatus:** [YouTube demonstration](https://www.youtube.com/watch?v=_n05wxybXdQ) · [Publisher’s Supplementary Movie 3](https://media.springernature.com/original/springer-static/esm/art%3A10.1038%2Fs41467-026-69037-4/MediaObjects/41467_2026_69037_MOESM5_ESM.mp4).


The current revision uses AccelStepper for independent axis control, with endpoint sweeps and scheduled random targets. The unchanged original sketch is preserved in the [original-sketch tag](https://github.com/Fermart-Lab/arduino-clinostat/tree/original-sketch) and the [archive](archive/Stepperclinostat/Stepperclinostat.ino).

## Files

| File | Purpose |
| --- | --- |
| [Stepperclinostat/Stepperclinostat.ino](Stepperclinostat/Stepperclinostat.ino) | Current AccelStepper sketch |
| [Stepperclinostat/Config.h](Stepperclinostat/Config.h) | Motion settings and interface selection |
| [archive/Stepperclinostat/Stepperclinostat.ino](archive/Stepperclinostat/Stepperclinostat.ino) | Original sketch, unchanged |
| [docs/firmware.md](docs/firmware.md) | Motion modes, parameters and serial output |
| [docs/hardware.md](docs/hardware.md) | Hardware and electrical interface selection |
| [docs/research.md](docs/research.md) | Article, supplementary information and video guide |
| [docs/provenance.md](docs/provenance.md) | Source identity, publication relationship and validation scope |
| [docs/original-firmware.md](docs/original-firmware.md) | Original sketch behavior |
| [tests/README.md](tests/README.md) | Software checks and build commands |
| [CHANGELOG.md](CHANGELOG.md) | Revision history |
| [CITATION.cff](CITATION.cff) | Citation metadata for GitHub |
| [LICENSE](LICENSE) | MIT license for the repository’s code and documentation |

## Open and verify the sketch

1. Install the [Arduino IDE](https://docs.arduino.cc/software/ide/). For an Arduino UNO, install the **Arduino AVR Boards** package through Boards Manager.
2. Install **AccelStepper by Mike McCauley**, version **1.64**, through Library Manager. See the [official library documentation](https://www.airspayce.com/mikem/arduino/AccelStepper/). Only the archived sketch requires Stepper.
3. Open `Stepperclinostat/Stepperclinostat.ino`. Keep `Config.h` and `AxisController.h` in the same folder as the sketch.
4. Select **Arduino Uno** and use **Verify** to compile. Record the IDE, board-package and library versions used.
5. Before uploading to a connected instrument, select the matching electrical interface and confirm motion settings using the [hardware guide](docs/hardware.md). Uploading or resetting starts the loop automatically; there is no start/stop command.

Compilation has been verified for Arduino UNO with Arduino AVR Boards 1.8.6 and AccelStepper 1.64.0 in both motion modes and both interface configurations. Six groups of controller tests pass. No motor operation or electrical validation was performed.

## Configuration

Edit `Stepperclinostat/Config.h` before compilation. The default mode sweeps each axis between two positions. Set `CLINOSTAT_RANDOM_MODE` to `1` for random targets at scheduled intervals. Each axis has its own range, speed, acceleration and timing settings. These are configurable examples, not recovered settings from the published experiments. See the [firmware guide](docs/firmware.md).

## Cite the research

Akshayakumar Kompa and Javier G. Fernandez. **Stronger when wet: Aquatically robust chitinous objects via zero-waste coordination with metal ions.** *Nature Communications* **17**, 1397 (2026). [doi:10.1038/s41467-026-69037-4](https://doi.org/10.1038/s41467-026-69037-4).

When reporting work based on this code, also identify the repository revision and any changes to the sketch or hardware. The article DOI identifies the research paper, not a version of this software.

## License

The code and original documentation in this repository use the [MIT license](LICENSE). External libraries and linked publication materials retain their own licenses; they are not included in this repository.
