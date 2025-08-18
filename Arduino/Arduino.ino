// Imports
#include "HX711.h"

// Init and HX711 to Arduino wiring
HX711 Scale1, Scale2, Scale3, Scale4;

uint8_t LOADCELL_DOUT_PINS[4] = {2, 3, 4, 5};
uint8_t LOADCELL_SCK_PINS[4] = {9, 10, 11, 12};

//const float calibration_factors[4] = {};

// Buzzer pin (passive buzzer)
const int buzzerPin = A0;

// SI thresholds
const float SI_SOFT_LIMIT = 10;  // for a mild warning
const float SI_HARD_LIMIT = 20;  // for a strong warning



void setup() 
{
  Serial.begin(9600);
  calibrateCell (197, 0);
  calibrateCell (197, 1);
  calibrateCell (197, 2);
  calibrateCell (197, 3);

  Scale1.begin(LOADCELL_DOUT_PINS[0], LOADCELL_SCK_PINS[0]);
  Scale2.begin(LOADCELL_DOUT_PINS[1], LOADCELL_SCK_PINS[1]);
  Scale3.begin(LOADCELL_DOUT_PINS[2], LOADCELL_SCK_PINS[2]);
  Scale4.begin(LOADCELL_DOUT_PINS[3], LOADCELL_SCK_PINS[3]);

  pinMode(buzzerPin, OUTPUT); 


  // set the calibration factor and reset the scale to 0 using the tare() function
  // Scale1.set_scale(); Scale1.tare();
  // Scale2.set_scale(); Scale2.tare();
  // Scale3.set_scale(); Scale3.tare();
  // Scale4.set_scale(); Scale4.tare();        

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