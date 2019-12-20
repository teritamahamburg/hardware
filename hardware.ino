#include "Adafruit_Thermal.h"

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

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

void audioPace(unsigned int pace) {
  delay(200 * pace);
}

void audioFuro() {
  tone(BUZZ, NOTE_G4); audioPace(1);
  tone(BUZZ, NOTE_F4); audioPace(1);
  tone(BUZZ, NOTE_E4); audioPace(2);

  tone(BUZZ, NOTE_G4); audioPace(1);
  tone(BUZZ, NOTE_C5); audioPace(1);
  tone(BUZZ, NOTE_B4); audioPace(2);

  tone(BUZZ, NOTE_G4); audioPace(1);
  tone(BUZZ, NOTE_D5); audioPace(1);
  tone(BUZZ, NOTE_C5); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(3);
  noTone(BUZZ); audioPace(1);

  tone(BUZZ, NOTE_C5); audioPace(1);
  tone(BUZZ, NOTE_B4); audioPace(1);
  tone(BUZZ, NOTE_A4); audioPace(2);

  tone(BUZZ, NOTE_F5); audioPace(1);
  tone(BUZZ, NOTE_D5); audioPace(1);
  tone(BUZZ, NOTE_C5); audioPace(2);
  tone(BUZZ, NOTE_B4); audioPace(2);

  tone(BUZZ, NOTE_C5); audioPace(1);
  tone(BUZZ, NOTE_G5); audioPace(1);
  tone(BUZZ, NOTE_G5); audioPace(1);
  tone(BUZZ, NOTE_F5); audioPace(1);
  tone(BUZZ, NOTE_E5); audioPace(3);
  noTone(BUZZ); audioPace(1);
}

void audioErr1() { //カンマ複数個
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
}

void audioErr2() { //カンマの後ろに数字がない
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
}

void audioErr3() { //カンマの前後の数字が０
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
  tone(BUZZ, NOTE_E5); audioPace(2);
  noTone(BUZZ); audioPace(2);
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

        // green -> blue
        if (beforePat == 2 && pat == 3) {
          r_Data[writed - 1] = 2;
          Serial.println("2");
          // writed++;
        } else if (beforePat == 0 && pat == 2) {
          r_Data[writed] = 1;
          Serial.println("1");
          writed++;
        } else if (beforePat == 0 && pat == 3) {
          r_Data[writed] = 0;
          Serial.println("0");
          writed++;
        } else if (pat == 1) {
          if (redCount == 1) break;
          redCount++;
        }
        beforePat = pat;
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
    else if (r_Data[i] == 2) {
      i -= 3;
      t_Data += ",";
    }
    else if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 0)  t_Data += "0";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 1)  t_Data += "1";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 1 && r_Data[i + 3] == 0)  t_Data += "2";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 0 && r_Data[i + 2] == 1 && r_Data[i + 3] == 1)  t_Data += "3";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 0 && r_Data[i + 3] == 0)  t_Data += "4";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 0 && r_Data[i + 3] == 1)  t_Data += "5";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 1 && r_Data[i + 3] == 0)  t_Data += "6";
    else if (r_Data[i] == 0 && r_Data[i + 1] == 1 && r_Data[i + 2] == 1 && r_Data[i + 3] == 1)  t_Data += "7";
    else if (r_Data[i] == 1 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 0)  t_Data += "8";
    else if (r_Data[i] == 1 && r_Data[i + 1] == 0 && r_Data[i + 2] == 0 && r_Data[i + 3] == 1)  t_Data += "9";
  }

  t_Data.toCharArray(res, resSize);
  return res;
}

uint8_t balidate(uint8_t r_Data) {
  if (0) {
    
  }
  else  return 0;
}

void loop() {
  while (digitalRead(SW));
  digitalWrite(LED, 1);

  char r_Data[256];
  readDataFromSensor(r_Data, 256);

  digitalWrite(LED, 0);
  char res[12];
  transformData(r_Data, res, 12);

  Serial.println(r_Data);
  Serial.println(res);

  if (!balidate(r_Data)) {
    printer.printBarcode(res, CODE128);
    audioFuro();
  }

  digitalWrite(LED, 0);
}
