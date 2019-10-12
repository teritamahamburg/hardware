#include "Adafruit_Thermal.h"

#include "SoftwareSerial.h"
#define TX_PIN 12
#define RX_PIN 11

SoftwareSerial printSerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&printSerial);

void setup() {
  printSerial.begin(9600);
  printer.begin();

  printer.justify('C');
  printer.boldOn();
  printer.println(F("550E8400E29B41D4A716446655000000999\n"));
  printer.boldOff();
  printer.justify('L');

  printer.print(F("CODE 39:"));
  printer.printBarcode("00000", CODE39);

  printer.feed(2);
  printer.setDefault(); // Restore printer to defaults
}

void loop() {
}
