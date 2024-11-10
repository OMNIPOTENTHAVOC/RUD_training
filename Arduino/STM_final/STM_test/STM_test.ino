void setup() {
    Serial.begin(9600);  // Start serial communication at 9600 baud
    delay(1000);         // Small delay to ensure serial is initialized
}

void loop() {
    Serial.println("Hello, STM32F401CCU Black Pill!");
    delay(1000);  // 1-second delay between messages
}
