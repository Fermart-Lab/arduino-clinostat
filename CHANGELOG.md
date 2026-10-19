# Changelog

## AccelStepper revision — 19 October 2026

Changes by JGFermart.

- Replace blocking Stepper calls with independent AccelStepper controllers.
- Add endpoint sweeps, per-axis dwell and scheduled random targets.
- Use bounded absolute targets instead of modulo counters.
- Add explicit phase-control and STEP/DIR interface selection.
- Add timestamped serial positions and controller tests.
- Preserve the original sketch byte for byte in the archive and `original-sketch` tag.

## Original import — 19 October 2026

By JGFermart. Publish the unchanged original sketch and reviewed research documentation.
