#include "HX711.h"

HX711 scale;

const uint8_t DOUT_PINS[4] = {2, 3, 4, 5};
const uint8_t SCK_PINS[4]  = {9, 10, 11, 12};

float calibrateCell (float knownWeight, uint8_t index)
{
  Serial.print("Calibrating Cell: "); Serial.println(index + 1);

  scale.begin(DOUT_PINS[index], SCK_PINS[index]);
  
  scale.set_scale(1.0);  // to make sure scale has default scale = 1 so get_units() returns raw counts
  scale.tare();         // sets the current reading to 0
  delay(500);

  
  Serial.println("TARE done. Make sure nothing is on the sensor now.");
  Serial.println("When ready, place the known weight onto the cell and press ENTER in the Serial Monitor...");
  while (!Serial.available()) { delay(100); } // wait for Enter (or any char)
  while (Serial.available()) Serial.read(); // clear input

  const int samples = 50;
  float rawSum = 0;
  for (int i = 0; i < samples; i++)
  {
    rawSum += (float) scale.get_units(5); // average of 5 reads per iteration 
    delay(100);
  }
  float rawAvg = rawSum / samples; // average of raw values

  float calib = rawAvg / knownWeight;

  return calib;

  tone(buzzerPin, 1000);
  delay(100);
  noTone(buzzerPin);
}