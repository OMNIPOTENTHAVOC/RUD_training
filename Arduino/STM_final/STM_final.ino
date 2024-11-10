// Pin configuration for Black Pill STM32F401CCU
const int analogPin = PA10;  // Analog pin where MQ135 AO is connected
const int baudRate = 9600;  // Baud rate for Serial communication

// Constants for MQ135 calculations
const float R0 = 10.0;  // Base resistance of MQ135 in clean air (adjust according to your sensor calibration)
const float VOLTAGE_REF = 5.0;  // The operating voltage of the MQ135 sensor

// Function to convert analog value to sensor resistance (RS)
float MQ135_GetResistance(int analogValue) {
    float voltage = (analogValue * VOLTAGE_REF) / 4095.0;  // Convert ADC value to voltage (12-bit ADC)
    float RS = ((VOLTAGE_REF - voltage) / voltage) * R0;   // Calculate sensor resistance (RS)
    return RS;
}

// Function to calculate CO2 concentration in ppm based on RS
float MQ135_GetCO2(float RS) {
    // Use the equation based on the sensor's datasheet sensitivity curve for CO2
    // Formula for CO2 ppm (this is an approximation, further calibration may be needed):
    // CO2 ppm = 116.6020682 * (RS / R0)^-2.769034857
    float ratio = RS / R0;
    float CO2_ppm = 116.6020682 * pow(ratio, -2.769034857);  // Based on the MQ135 CO2 sensitivity curve
    return CO2_ppm;
}

void setup() {
    // Initialize serial communication at the desired baud rate
    Serial.begin(baudRate);

    // Set analog pin mode (optional, not needed in Arduino-style code)
    pinMode(analogPin, INPUT);
}

void loop() {
    // Read analog value from MQ135 sensor
    int analogValue = analogRead(analogPin);

    // Calculate sensor resistance (RS) based on analog value
    float RS = MQ135_GetResistance(analogValue);

    // Calculate CO2 concentration in ppm
    float CO2_level = MQ135_GetCO2(RS);

    // Print CO2 level to the serial monitor in real-time
    Serial.print("CO2 Level: ");
    Serial.print(CO2_level);
    Serial.println(" ppm");

    // Small delay to simulate real-time but not overwhelm serial monitor
    delay(1000);  // 1-second delay for real-time updates
}
