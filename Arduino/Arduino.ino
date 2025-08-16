// Imports
#include "HX711.h"

// Init and HX711 to Arduino wiring
HX711 Scale1;
HX711 Scale2;
HX711 Scale3;
HX711 Scale4;

const int LOADCELL1_DOUT_PIN = 2;
const int LOADCELL2_DOUT_PIN = 3;

const int LOADCELL3_DOUT_PIN = 4;
const int LOADCELL4_DOUT_PIN = 5;

const int LOADCELL1_SCK_PIN = 9;
const int LOADCELL2_SCK_PIN = 10;

const int LOADCELL3_SCK_PIN = 11;
const int LOADCELL4_SCK_PIN = 12;

// const float calibration_factor1 = ;
// const float calibration_factor2 = ;
// const float calibration_factor3 = ;
// const float calibration_factor4 = ;

const float imbalanceRatio = 0.1; // variable

// Buzzer pin (passive buzzer)
const int buzzerPin = A0;


void setup() 
{
  Serial.begin(9600);
  Scale1.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
  Scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);
  Scale3.begin(LOADCELL3_DOUT_PIN, LOADCELL3_SCK_PIN);
  Scale4.begin(LOADCELL4_DOUT_PIN, LOADCELL4_SCK_PIN);

  pinMode(buzzerPin, OUTPUT); 

  // set the calibration factor and reset the scale to 0 using the tare() function
  Scale1.set_scale(); 
  Scale1.tare();

  Scale2.set_scale();
  Scale2.tare();

  Scale3.set_scale();
  Scale3.tare();

  Scale4.set_scale();
  Scale4.tare();        
}

void loop() 
{
  // Read values (raw units) on an average of 10 reads
  long w1 = Scale1.get_units(10);
  long w2 = Scale2.get_units(10);
  long w3 = Scale3.get_units(10);
  long w4 = -Scale4.get_units(10);  // inverted the 4th load cell

  long totalWeight = w1 + w2 + w3 + w4;

  long leftSide = w2 + w3; long rightSide = w1 + w4;

  if (abs(leftSide - rightSide) >= totalWeight * imbalanceRatio)
  {
    tone(A0, 200);
  }
  else
  {
    noTone(A0);
  }

  Serial.print("W1: "); Serial.print(w1);
  Serial.print(" W2: "); Serial.print(w2);
  Serial.print(" W3: "); Serial.print(w3);
  Serial.print(" W4: "); Serial.println(w4);    
}