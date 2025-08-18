// Imports
#include "HX711.h"

// Init and HX711 to Arduino wiring
HX711 ScaleBR, ScaleBL, ScaleTL, ScaleTR;

const uint8_t LOADCELL_DOUT_PINS[4] = {2, 3, 4, 5};
const uint8_t LOADCELL_SCK_PINS[4] = {9, 10, 11, 12};
const float CALIBRATION[4] = {-125.30, 173.22, 187.26, -196.06};

// Buzzer pin (passive buzzer)
const int buzzerPin = A0;

// SI thresholds
const float SI_SOFT_LIMIT = 10;  // for a mild warning
const float SI_HARD_LIMIT = 20;  // for a strong warning


//loadCalibrationFromEEPROM();

void setup() 
{
  Serial.begin(9600);

  // Serial.println(calibrateCell (1520, 0));
  // Serial.println(calibrateCell (1520, 1));
  // Serial.println(calibrateCell (1520, 2));
  // Serial.println(calibrateCell (1520, 3));

  ScaleBR.begin(LOADCELL_DOUT_PINS[0], LOADCELL_SCK_PINS[0]);
  ScaleBL.begin(LOADCELL_DOUT_PINS[1], LOADCELL_SCK_PINS[1]);
  ScaleTL.begin(LOADCELL_DOUT_PINS[2], LOADCELL_SCK_PINS[2]);
  ScaleTR.begin(LOADCELL_DOUT_PINS[3], LOADCELL_SCK_PINS[3]);

  pinMode(buzzerPin, OUTPUT); 


  // set the calibration factor and reset the scale to 0 using the tare() function
  ScaleBR.set_scale(CALIBRATION[0]); ScaleBR.tare();
  ScaleBL.set_scale(CALIBRATION[1]); ScaleBL.tare();
  ScaleTL.set_scale(CALIBRATION[2]); ScaleTL.tare();
  ScaleTR.set_scale(CALIBRATION[3]); ScaleTR.tare();        

}

void loop() 
{
  float wBR = ScaleBR.get_units(10);
  float wBL = ScaleBL.get_units(10);
  float wTL = ScaleTL.get_units(10);
  float wTR = ScaleTR.get_units(10);

  float leftSide = abs(wTL) + abs(wBL);
  float rightSide = abs(wTR) + abs(wBR);
  float total = abs(wBR) + abs(wBL) + abs(wTL) + abs(wTR);
  float leftNorm = leftSide / total;
  float rightNorm = rightSide / total;
  float SI = (leftNorm - rightNorm) * 100;

  // Debug DEBUG
  Serial.print("BR: "); Serial.print(wBR, 2);
  Serial.print("  BL: "); Serial.print(wBL, 2);
  Serial.print("  TL: "); Serial.print(wTL, 2);
  Serial.print("  TR: "); Serial.print(wTR, 2);
  Serial.print("  Total: "); Serial.print(total, 2);
  Serial.print("  SI: "); Serial.println(SI, 2);

  // Buzzer alerts based on SI thresholds
  if (abs(SI) >= SI_HARD_LIMIT) 
  {
    tone(buzzerPin, 1000);  // strong warning
    //noTone(buzzerPin);
  } 
  else if (abs(SI) >= SI_SOFT_LIMIT) 
  {
    tone(buzzerPin, 200);   // mild warning
    //noTone(buzzerPin);
  } 
  else 
  {
    noTone(buzzerPin);      // safe posture
  }

  delay(500);  // adjust reading interval
}
