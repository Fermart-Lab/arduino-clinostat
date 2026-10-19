# Arduino clinostat control

Archived Arduino control sketch for the Fermart Lab two-axis clinostat used in biomaterial molding research.

The device repositions a mold while nickel-doped chitosan solidifies, helping the shrinking material remain in contact with the mold. See [the research article](https://doi.org/10.1038/s41467-026-69037-4), particularly Figure 3 and Methods → Construction of clinostat.

**Watch the apparatus:** [YouTube demonstration](https://www.youtube.com/watch?v=_n05wxybXdQ) · [Publisher’s Supplementary Movie 3](https://media.springernature.com/original/springer-static/esm/art%3A10.1038%2Fs41467-026-69037-4/MediaObjects/41467_2026_69037_MOESM5_ESM.mp4).


## Files

| File | Purpose |
| --- | --- |
| [Stepperclinostat/Stepperclinostat.ino](Stepperclinostat/Stepperclinostat.ino) | Original sketch, unchanged |
| [docs/firmware.md](docs/firmware.md) | Parameters, actual control flow and serial output |
| [docs/hardware.md](docs/hardware.md) | Known hardware, missing specifications and bench verification |
| [docs/research.md](docs/research.md) | Article, supplementary information and video guide |
| [docs/provenance.md](docs/provenance.md) | Source identity, publication relationship and validation scope |
| [CITATION.cff](CITATION.cff) | Citation metadata for GitHub |
| [LICENSE](LICENSE) | MIT license for the repository’s code and documentation |

## Open and verify the sketch

1. Install the [Arduino IDE](https://docs.arduino.cc/software/ide/). For an Arduino UNO, install the **Arduino AVR Boards** package through Boards Manager.
2. Install **Stepper by Arduino** through Library Manager if it is not already available. The archived sketch does not require AccelStepper. See the [official Stepper library](https://docs.arduino.cc/libraries/stepper/).
3. Open `Stepperclinostat/Stepperclinostat.ino`. Keep the sketch inside its matching `Stepperclinostat` folder.
4. Select **Arduino Uno** and use **Verify** to compile. Record the IDE, board-package and library versions used.
5. Before uploading to a connected instrument, complete the interface and motion checks in [hardware and commissioning](docs/hardware.md). Uploading or resetting starts the loop automatically; there is no start/stop command.

Compilation has been verified for Arduino UNO with Arduino AVR Boards 1.8.6 and Stepper 1.1.3. No motor operation or electrical validation was performed.

## Configuration

Parameters are edited in the sketch before compilation. Defaults are 200 and 800 configured steps per revolution, speed arguments 60 and 40, control pins 6/7 and 8/9, and serial output at 9600 baud. These are source-code values, not measured frame speeds or confirmed driver settings. The [firmware guide](docs/firmware.md) explains the units and reversal behavior.

## Cite the research

Akshayakumar Kompa and Javier G. Fernandez. **Stronger when wet: Aquatically robust chitinous objects via zero-waste coordination with metal ions.** *Nature Communications* **17**, 1397 (2026). [doi:10.1038/s41467-026-69037-4](https://doi.org/10.1038/s41467-026-69037-4).

When reporting work based on this code, also identify the repository revision and any changes to the sketch or hardware. The article DOI identifies the research paper, not a version of this software.

## License

The code and original documentation in this repository use the [MIT license](LICENSE). External libraries and linked publication materials retain their own licenses; they are not included in this repository.
