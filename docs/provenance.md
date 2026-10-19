# Source provenance

Maintained by [JGFermart](https://github.com/JGFermart).

## Original source

The initial repository version preserves `Stepperclinostat/Stepperclinostat.ino` byte for byte, including its original comments and formatting. No motor-control changes were made in the initial import.

SHA-256:

```text
6068bd3f5efad26b730df26e885c4e4104dc3638301a67ba68107936f0414e93
```

The source is associated with the clinostat described in the [research article](https://doi.org/10.1038/s41467-026-69037-4). 


## AccelStepper revision

Revision by JGFermart, 19 October 2026. The original import is commit [`97d5cf7`](https://github.com/Fermart-Lab/arduino-clinostat/commit/97d5cf79afb40bcd502d303d3154b47479b20972), tagged [`original-sketch`](https://github.com/Fermart-Lab/arduino-clinostat/tree/original-sketch). Its sketch is also retained unchanged in `archive/Stepperclinostat/Stepperclinostat.ino` with the checksum above.

The new controller implements independent AccelStepper motion, endpoint sweeps and scheduled random targets corresponding to the motion options described in the article. This is a subsequent software revision; its configurable example parameters do not establish the settings used in the published experiments. Software validation is described in [tests/README.md](../tests/README.md).
