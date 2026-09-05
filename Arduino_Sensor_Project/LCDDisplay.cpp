#include "LCDDisplay.h"

LCDDisplay::LCDDisplay(uint8_t address, uint8_t columns, uint8_t rows)
  : lcd(address, columns, rows) {
}

void LCDDisplay::begin() {
  lcd.init();
  lcd.backlight();
}

void LCDDisplay::showStartup() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
}

void LCDDisplay::showTemperatureHumidity(float temperature, float humidity) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.print(humidity, 0);
  lcd.print("%");
}

void LCDDisplay::showMotion(bool motion) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Motion:");
  lcd.setCursor(0, 1);

  if (motion) {
    lcd.print("DETECTED");
  } else {
    lcd.print("NONE");
  }
}

void LCDDisplay::showGas(int gasValue, int threshold) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gasValue);

  lcd.setCursor(0, 1);

  if (gasValue > threshold) {
    lcd.print("WARNING!");
  } else {
    lcd.print("Gas Normal");
  }
}
