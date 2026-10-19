// AccelStepper revision maintained by JGFermart.
// Original source: the original-sketch tag and archive/Stepperclinostat/.
#include <Arduino.h>
#include <AccelStepper.h>
#include "Config.h"

#if CLINOSTAT_STEP_DIR
// AccelStepper DRIVER takes STEP first, then DIR.
AccelStepper motor1(AccelStepper::DRIVER, PUL_PIN_1, DIR_PIN_1, 0, 0, false);
AccelStepper motor2(AccelStepper::DRIVER, PUL_PIN_2, DIR_PIN_2, 0, 0, false);
#else
// Keep the archived sketch's two-phase pin ordering by default.
AccelStepper motor1(AccelStepper::FULL2WIRE, DIR_PIN_1, PUL_PIN_1, 0, 0, false);
AccelStepper motor2(AccelStepper::FULL2WIRE, DIR_PIN_2, PUL_PIN_2, 0, 0, false);
#endif

clinostat::AxisController<AccelStepper> axis1(motor1, AXIS_1, RANDOM_SEED_1);
clinostat::AxisController<AccelStepper> axis2(motor2, AXIS_2, RANDOM_SEED_2);
bool configurationValid = false;
uint32_t lastTelemetryAt = 0;

void setup() {
  Serial.begin(9600);
  const uint32_t now = millis();
  const bool firstValid = axis1.begin(now);
  const bool secondValid = axis2.begin(now);
  configurationValid = firstValid && secondValid;
  if (!configurationValid) {
    Serial.println(F("Invalid motion configuration; outputs disabled."));
    return;
  }
  motor1.setMinPulseWidth(STEP_PULSE_WIDTH_US);
  motor2.setMinPulseWidth(STEP_PULSE_WIDTH_US);
  motor1.enableOutputs();
  motor2.enableOutputs();
  lastTelemetryAt = now;
}

void loop() {
  if (!configurationValid) return;
  const uint32_t now = millis();
  axis1.update(now);
  axis2.update(now);

  // At most 36 bytes per record; skip logging instead of waiting for serial space.
  if (static_cast<uint32_t>(now - lastTelemetryAt) >= TELEMETRY_INTERVAL_MS &&
      Serial.availableForWrite() >= 40) {
    lastTelemetryAt = now;
    Serial.print(now);
    Serial.print(',');
    Serial.print(motor1.currentPosition());
    Serial.print(',');
    Serial.println(motor2.currentPosition());
  }
}
