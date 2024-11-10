void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  // Move motor 1 forward at half speed
  Serial.println(127); // Motor 1 forward (half speed)
  delay(2000); // Wait for 2 seconds

  // Stop the motor
  Serial.println(0); // Stop motor 1
  delay(2000); // Wait for 2 seconds

  // Move motor 1 in reverse at half speed
  Serial.println(192); // Motor 1 reverse (half speed)
  delay(2000); // Wait for 2 seconds

  // Stop the motor again
  Serial.println(0); // Stop motor 1
  delay(2000); // Wait for 2 seconds
}

