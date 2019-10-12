#include <Wire.h>
#include <S11059.h>

S11059 colorSensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  digitalWrite(SDA, 0);
  digitalWrite(SCL, 0);
  colorSensor.setMode(S11059_MODE_FIXED);
  colorSensor.setGain(S11059_GAIN_HIGH);

  // * S11059_TINT0: 87.5 us
  // * S11059_TINT1: 1.4 ms
  // * S11059_TINT2: 22.4 ms
  // * S11059_TINT3: 179.2 ms
  colorSensor.setTint(S11059_TINT3);

  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  if (!colorSensor.start()) {
    Serial.println("start failed");
  }

}

void loop() {
  colorSensor.delay();

  if (colorSensor.update()) {
    Serial.print(colorSensor.getBlue());
    Serial.print(",");
    Serial.print(colorSensor.getRed());
    Serial.print(",");
    Serial.print(colorSensor.getGreen());
    Serial.print(",");
    Serial.print(colorSensor.getIR());
    Serial.println("");
  } else {
    Serial.println("update failed");
  }
}