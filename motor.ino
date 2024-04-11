
// Function to run a motor for a specific time
void vibrateMotor(int motorRunTime) {
  digitalWrite(motorPin, HIGH);  // Turn on the motor
  delay(motorRunTime);           // Run the motor for the specified time
  digitalWrite(motorPin, LOW);   // Turn off the motor
}

void stopMotors() {
  digitalWrite(motorPin, LOW);
  delay(10);
}

void runMotorDiagnostic() {
  // Run each motor for 100ms in sequence
  Serial.println("Running motor diagnostic");

  vibrateMotor(100);
  delay(100);
  vibrateMotor(100);
  delay(100);
  vibrateMotor(100);
  delay(100);
  vibrateMotor(100);
}
