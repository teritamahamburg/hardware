#include "Adafruit_Thermal.h"

#include "SoftwareSerial.h"
#define TX_PIN 12
#define RX_PIN 11

SoftwareSerial printSerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&printSerial);

#include <Wire.h>
#include <S11059.h>

int color_R = 0, color_G = 0, color_B = 0;

S11059 colorSensor;

void setup() {
  printSerial.begin(9600);
  printer.begin();
  Serial.begin(9600);
  Wire.begin();
  digitalWrite(SDA, 0);
  digitalWrite(SCL, 0);
  colorSensor.setMode(S11059_MODE_FIXED);
  colorSensor.setGain(S11059_GAIN_HIGH);
  colorSensor.setTint(S11059_TINT3);

  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  if (!colorSensor.start()) {
    Serial.println("start failed");
  }
  printer.feed(2);
  printer.setDefault(); // Restore printer to defaults
}

void loop() {
  colorSensor.delay();
  color_R = colorSensor.getRed();
  color_G = colorSensor.getGreen();
  color_B = colorSensor.getBlue();

  colorSensor.update();
  printer.print(F("R: "));
  printer.print(color_R);
  printer.print(F(" G: "));
  printer.print(color_G);
  printer.print(F(" B: "));
  printer.print(color_B);
  printer.println();
  delay(2000);
}