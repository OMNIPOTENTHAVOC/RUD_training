// Analog pin connected to MQ135
const int senPin = A0;

// Constants for CO2 estimation
float RLOAD = 10.0; // Sensor load resistance (10kΩ)
float RZERO = 76.63; // Calibration resistance in fresh air (Ohms)
float CO2Curve[3] = {2.602, 0.31, -0.58}; // CO2 curve values for MQ135 (from datasheet)

// Func to calculate resistance of the sensor
float calculateResistance(int rawADC) {
  return ((1023.0 * RLOAD) / rawADC) - RLOAD;
}

// Func to estimate CO2 ppm using log
float getCO2Concentration(float resistance) {
  float ratio = resistance / RZERO;
  return pow(10, ((log10(ratio) - CO2Curve[1]) / CO2Curve[2]) + CO2Curve[0]);
}

void setup() {
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the value from MQ135 (0-1023)
  int sensorValue = analogRead(senPin);
  
  //Sensor resistance
  float resistance = calculateResistance(sensorValue);

  // Estimate CO2 concentration in ppm
  float co2ppm = getCO2Concentration(resistance);

  // Display the sensor value, resistance, and estimated CO2 conc.
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Sensor Resistance: ");
  Serial.print(resistance);
  Serial.print(" Ohms | Estimated CO2: ");
  Serial.print(co2ppm);
  Serial.println(" ppm");

  // 2 sec gaps b/w readings
  delay(2000);
}