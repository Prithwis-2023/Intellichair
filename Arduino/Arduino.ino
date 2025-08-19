#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PINS[4] = {2, 3, 4, 5};
const int LOADCELL_SCK_PINS[4] = {9, 10, 11, 12};
const float CALIBRATIONS[4] = {-578312.19, 2911352.00, 1047461.44, 2131984.75};  // accomplished based on the documentation: github.com/RobTillaart/HX711/tree/master
const int chassisLength = 300;  // TBD
const int SI_THRESHOLD = 15;
const int CoPx_THRESHOLD = 30;  // in mm
//const int ledPin = 6;
const int buzzerPin = A0;  // passive buzzer

HX711 scaleTR, scaleBR, scaleBL, scaleTL;  // cells 1 and 2 are right ans cells 3 and 4 are left, with reference to arduino jack

void setup() 
{
  Serial.begin(57600);  // suggested baud rate online

  while(!Serial);
  
  scaleTR.begin(LOADCELL_DOUT_PINS[0], LOADCELL_SCK_PINS[0]);
  scaleBR.begin(LOADCELL_DOUT_PINS[1], LOADCELL_SCK_PINS[1]);
  scaleBL.begin(LOADCELL_DOUT_PINS[2], LOADCELL_SCK_PINS[2]);
  scaleTL.begin(LOADCELL_DOUT_PINS[3], LOADCELL_SCK_PINS[3]);

  pinMode(ledPin, OUTPUT);
  
  // calibrate the scales
  scaleTR.set_scale(CALIBRATIONS[0]);
  scaleBR.set_scale(CALIBRATIONS[1]);
  scaleBL.set_scale(CALIBRATIONS[2]);
  scaleTL.set_scale(CALIBRATIONS[3]);
}

void loop() 
{
  if (scaleTR.is_ready() && scaleBR.is_ready() && scaleBL.is_ready() && scaleTL.is_ready()) 
  {
    float readingTR = fabs(scaleTR.get_units(10));
    float readingBR = fabs(scaleBR.get_units(10));
    float readingBL = fabs(scaleBL.get_units(10));
    float readingTL = fabs(scaleTL.get_units(10));
    float total = readingTR + readingBR + readingBL + readingTL;
    
    Serial.print("HX711 reading: ");

    float rightSideNorm = (readingTR + readingBR) / total; 
    float leftSideNorm = (readingBL + readingTL) / total;
    Serial.print(leftSideNorm); Serial.print(" "); Serial.println(rightSideNorm);

    float SI = fabs(rightSideNorm - leftSideNorm) * 100.0f;
    float CoPx = (chassisLength * 0.5f) * fabs(rightSideNorm - leftSideNorm);
    Serial.println(SI);
    Serial.println(CoPx);

    if (SI >= SI_THRESHOLD && CoPx >= CoPx_THRESHOLD)
    {
      //digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000);  // strong warning
      
    }
    else
    {
      //digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
    }
  } 
  delay(1000); 
}