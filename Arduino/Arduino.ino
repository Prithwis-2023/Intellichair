// Imports
#include "HX711.h"

/*
TARE done. Make sure nothing is on the sensor now.
When ready, place the known weight onto the cell and press ENTER in the Serial Monitor...
-125.30
Calibrating Cell: 2
TARE done. Make sure nothing is on the sensor now.
When ready, place the known weight onto the cell and press ENTER in the Serial Monitor...
173.22
Calibrating Cell: 3
TARE done. Make sure nothing is on the sensor now.
When ready, place the known weight onto the cell and press ENTER in the Serial Monitor...
187.26
Calibrating Cell: 4
TARE done. Make sure nothing is on the sensor now.
When ready, place the known weight onto the cell and press ENTER in the Serial Monitor...
-196.06
*/

// Init and HX711 to Arduino wiring
HX711 Scale1, Scale2, Scale3, Scale4;

const uint8_t LOADCELL_DOUT_PINS[4] = {2, 3, 4, 5};
const uint8_t LOADCELL_SCK_PINS[4] = {9, 10, 11, 12};
const float CALIBRATION[4] = {-125.30, 173.22, 187.26, -196.06};


// Buzzer pin (passive buzzer)
const int buzzerPin = A0;

// SI thresholds
const float SI_SOFT_LIMIT = 10;  // for a mild warning
const float SI_HARD_LIMIT = 20;  // for a strong warning



void setup() 
{
  Serial.begin(9600);
  // Serial.println(calibrateCell (1520, 0));
  // Serial.println(calibrateCell (1520, 1));
  // Serial.println(calibrateCell (1520, 2));
  // Serial.println(calibrateCell (1520, 3));

  Scale1.begin(LOADCELL_DOUT_PINS[0], LOADCELL_SCK_PINS[0]);
  Scale2.begin(LOADCELL_DOUT_PINS[1], LOADCELL_SCK_PINS[1]);
  Scale3.begin(LOADCELL_DOUT_PINS[2], LOADCELL_SCK_PINS[2]);
  Scale4.begin(LOADCELL_DOUT_PINS[3], LOADCELL_SCK_PINS[3]);

  pinMode(buzzerPin, OUTPUT); 


  // set the calibration factor and reset the scale to 0 using the tare() function
  Scale1.set_scale(CALIBRATION[0]); Scale1.tare();
  Scale2.set_scale(CALIBRATION[1]); Scale2.tare();
  Scale3.set_scale(CALIBRATION[2]); Scale3.tare();
  Scale4.set_scale(CALIBRATION[3]); Scale4.tare();        

}

void loop() 
{
  // // Read values (raw units) on an average of 10 reads
  // long w1 = Scale1.get_units(10);
  // long w2 = Scale2.get_units(10);
  // long w3 = Scale3.get_units(10);
  // long w4 = -Scale4.get_units(10);  // inverted the 4th load cell

  // float total = w1 + w2 + w3 + w4;

  // float leftSide = w1 + w4; float rightSide = w2 + w3;

  // // ----- Symmetry Index (SI) (left-right) - absolute percent
  // // SI = 2 * (X_d - X_nd) / (|X_d| + |X_nd|) * 100
  // // we'll compute absolute percent (0..inf) as a positive number
  // float SI_lr = 0.0;
  // if (fabs(leftSide) + fabs(rightSide) > 1e-6) 
  // {
  //   SI_lr = fabs(2.0 * (rightSide - leftSide) / (fabs(rightSide) + fabs(leftSide)) * 100.0);
  // }

  // if (SI_lr >= SI_HARD_LIMIT)
  // {
  //   //Serial.println(SI_lr);
  // }
  // else 
  // {
  //   noTone(buzzerPin);
  // }

  // // Serial.print("W1: "); Serial.print(w1);
  // // Serial.print(" W2: "); Serial.print(w2);
  // // Serial.print(" W3: "); Serial.print(w3);
  // // Serial.print(" W4: "); Serial.println(w4);    
}