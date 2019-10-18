#include "Adafruit_Thermal.h"

#include "SoftwareSerial.h"
#define TX_PIN 12
#define RX_PIN 11

SoftwareSerial printSerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&printSerial);

#include <Wire.h>
#include <S11059.h>

#define SW 4
#define LED 5
#define BUZZ 3

#define R_th 20
#define G_th 20
#define B_th 20

int color_R = 0, color_G = 0, color_B = 0;
char r_Data[256];
char res[12];

S11059 colorSensor;

void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  printSerial.begin(9600);
  printer.begin();
  Serial.begin(9600);
  Wire.begin();
  digitalWrite(SDA, 0);
  digitalWrite(SCL, 0);
  colorSensor.setMode(S11059_MODE_FIXED);
  colorSensor.setGain(S11059_GAIN_HIGH);
  colorSensor.setTint(S11059_TINT1);

  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  if (!colorSensor.start()) {
    Serial.println("start failed");
  }

  //  printer.feed(2);
  printer.setDefault();
}

void r_data() {
  for (int i = 0; i < 256; i++) r_Data[i] = -1;

  colorSensor.delay();

  while (colorSensor.getRed() > R_th && colorSensor.getGreen() < G_th && colorSensor.getBlue() < B_th);

  for (int i = 0; i < 256; i++) {
    while (colorSensor.getRed() < R_th && colorSensor.getGreen() < G_th && colorSensor.getBlue() < B_th);

    if (colorSensor.getRed() < R_th && colorSensor.getGreen() > G_th && colorSensor.getBlue() < B_th) r_Data[i] = 1;
    while (colorSensor.getRed() < R_th && colorSensor.getGreen() > G_th && colorSensor.getBlue() < B_th);
    if (colorSensor.getRed() < R_th && colorSensor.getGreen() < G_th && colorSensor.getBlue() > B_th) r_Data[i] = 0;
    while (colorSensor.getRed() < R_th && colorSensor.getGreen() < G_th && colorSensor.getBlue() > B_th);
    if (colorSensor.getRed() > R_th && colorSensor.getGreen() < G_th && colorSensor.getBlue() < B_th) break;
  }

}

void t_data() {
  String t_Data = "";

  for (int i = 0; i < 256; i += 4) {
    if (r_Data[i] == -1) break;
    if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 0)  t_Data += "0";
    if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 1)  t_Data += "1";
    if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 1 && r_Data[i + 3] == 0)  t_Data += "2";
    if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 1 && r_Data[i + 3] == 1)  t_Data += "3";
    if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 0 && r_Data[i + 3] == 0)  t_Data += "4";
    if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 0 && r_Data[i + 3] == 1)  t_Data += "5";
    if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 1 && r_Data[i + 3] == 0)  t_Data += "6";
    if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 1 && r_Data[i + 3] == 1)  t_Data += "7";
    if (r_Data[i] == 1 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 0)  t_Data += "8";
    if (r_Data[i] == 1 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 1)  t_Data += "9";
  }

  t_Data.toCharArray(res, 12);
}

void loop() {
  while (digitalRead(SW));
  //  tone(BUZZ, 440);
  digitalWrite(LED, 1);

  r_data();

  digitalWrite(LED, 0);
  t_data();

  printer.printBarcode(res, CODE128);

  //  noTone(BUZZ);
  digitalWrite(LED, 0);

  //  colorSensor.delay();
  //  if (colorSensor.update()) {
  //    Serial.print(colorSensor.getRed());
  //    Serial.print(",");
  //    Serial.print(colorSensor.getGreen());
  //    Serial.print(",");
  //    Serial.print(colorSensor.getBlue());
  //    Serial.print(",");
  //    Serial.print(colorSensor.getIR());
  //    Serial.println("");
  //  } else {
  //    Serial.println("update failed");
  //  }
}
