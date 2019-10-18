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
#define COLOR_THRESHOLD 20

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
  colorSensor.setTint(S11059_TINT2);

  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  if (!colorSensor.start()) {
    Serial.println("start failed");
  }

  //  printer.feed(2);
  printer.setDefault();
}

void readDataFromSensor(char *r_Data, int size) {
  for (int i = 0; i < size; i++) r_Data[i] = -1;

  int writed = 0;
  int beforePat = -1;
  int redCount = 0;
  while (1) {
    colorSensor.delay();
    if (colorSensor.update()) {
      int colors[3] = {colorSensor.getRed(), colorSensor.getGreen(), colorSensor.getBlue()};
      int minVal = min(colors[0], min(colors[1], colors[2]));
      for (int i = 0; i < 3; i++) colors[i] = max(colors[i] - minVal - COLOR_THRESHOLD, 0);

      int pat = -1; // [-1: undefined, black, red, green, blue]
      if (colors[0] == 0 && colors[1] == 0 && colors[2] == 0) pat = 0;
      else if (colors[0] > colors[1] && colors[0] > colors[2]) pat = 1;
      else if (colors[1] > colors[0] && colors[1] > colors[2]) pat = 2;
      else if (colors[2] > colors[0] && colors[2] > colors[1]) pat = 3;
      
      if (pat != -1 && beforePat != pat) {
        beforePat = pat;
        if (pat == 2) {
          r_Data[writed] = 1;
          writed++;
        } else if (pat == 3) {
          r_Data[writed] = 0;
          writed++;
        } else if (pat == 1) {
          if (redCount == 1) break;
          redCount++;
        }
        if (writed == size) break;
      }
    } else {
      Serial.println("update failed");
      break;
    }
  }
}

char* transformData(char *r_Data, char *res, int resSize) {
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

  Serial.println("TData: " + t_Data);

  t_Data.toCharArray(res, resSize);
  return res;
}

void loop() {
  while (digitalRead(SW));
  digitalWrite(LED, 1);

  char r_Data[256];
  readDataFromSensor(r_Data, 256);

  digitalWrite(LED, 0);
  char res[12];
  transformData(r_Data, res, 12);

  printer.printBarcode(res, CODE128);

  digitalWrite(LED, 0);
}
