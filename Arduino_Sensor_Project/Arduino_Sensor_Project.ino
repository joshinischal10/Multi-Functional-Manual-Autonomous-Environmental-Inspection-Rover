// Arduino_Sensor_Project.ino
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Config.h"
#include "DHTSensor.h"
#include "PIRSensor.h"
#include "GasSensor.h" // Added Gas Sensor header

// Initialize LCD (I2C address 0x27 is standard; change to 0x3F if blank)
LiquidCrystal_I2C lcd(0x27, 16, 2);

DHTSensor dhtSensor(DHT_PIN, DHT_TYPE);
PIRSensor pirSensor(PIR_PIN);
GasSensor gasSensor(GAS_PIN); // Added Gas Sensor instance

unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000; // 2 seconds delay for sensors

void setup() {
    Serial.begin(BAUD_RATE);
    
    // Initialize LCD with backlight
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("System Readying");
    
    dhtSensor.begin();
    pirSensor.begin();
    gasSensor.begin(); // Initialize Gas Sensor
    
    // Warm-up delay to stabilize the PIR sensor baseline
    delay(10000); 
    lcd.clear();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastReadTime >= readInterval) {
        lastReadTime = currentMillis;

        bool dhtSuccess = dhtSensor.read();
        bool motionDetected = pirSensor.isMotionDetected();
        int gasValue = gasSensor.read(); // Read gas sensor raw/ppm value

        float temp = dhtSuccess ? dhtSensor.getTemperature() : 0.0;
        float hum = dhtSuccess ? dhtSensor.getHumidity() : 0.0;

        // Display readings on LCD screen
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temp, 1);
        lcd.print("C H:");
        lcd.print(hum, 1);
        lcd.print("%  ");

        lcd.setCursor(0, 1);
        lcd.print("M:");
        lcd.print(motionDetected ? "YES" : "NO ");
        lcd.print(" G:");
        lcd.print(gasValue);
        lcd.print("   ");

        // Format: TEMP,HUMIDITY,MOTION,GAS
        Serial.print(temp);
        Serial.print(",");
        Serial.print(hum);
        Serial.print(",");
        Serial.print(motionDetected ? "1" : "0");
        Serial.print(",");
        Serial.println(gasValue);
    }
}