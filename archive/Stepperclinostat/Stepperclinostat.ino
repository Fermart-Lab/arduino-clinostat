// Include the Stepper library
#include <Stepper.h>

// Define the number of steps per revolution for each motor
#define STEPS_PER_REV_1 200
#define STEPS_PER_REV_2 800

// Define the pins for the first motor
#define DIR_PIN_1 6
#define PUL_PIN_1 7

// Define the pins for the second motor
#define DIR_PIN_2 8
#define PUL_PIN_2 9

// Create Stepper objects for each motor
Stepper motor1(STEPS_PER_REV_1, DIR_PIN_1, PUL_PIN_1);
Stepper motor2(STEPS_PER_REV_2, DIR_PIN_2, PUL_PIN_2);

// Define the speed and angle variables for each motor
int speed1 = 60; // RPM
int speed2 = 40; // Degrees per minute
int angle1 = 0; // Current angle of the first motor in steps
int angle2 = 0; // Current angle of the second motor in steps
int dir1 = 0-1; // Direction of the second motor (1 or -1)
int dir2 = 1; // Direction of the second motor (1 or -1)

void setup() {
  // Set the speed for each motor
  motor1.setSpeed(speed1);
  motor2.setSpeed(speed2);
  Serial.begin(9600);
}

void loop() {
  // Rotate the first motor continuously in one direction
  motor1.step(dir1);
  Serial.print(angle1);
  Serial.print('-');
  angle1 = (angle1 + 1) % STEPS_PER_REV_1; // Update the current angle
  
  // Rotate the second motor back and forth by 40 degrees
  if (angle2 == 0 || angle2 == STEPS_PER_REV_2 / 9) { // Check if the motor reached the limit angles
    dir2 = -dir2; // Reverse the direction
    delay(1000); // Wait for one second before changing direction
  }
  motor2.step(dir2); // Step in the current direction
  Serial.print(angle2);
  Serial.println();
  angle2 = (angle2 + dir2 + STEPS_PER_REV_2) % STEPS_PER_REV_2; // Update the current angle
}