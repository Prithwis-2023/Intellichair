// Imports
#include "HX711.h"

// HX711 to Arduino wiring
const int LOADCELL1_DOUT_PIN = 2;
const int LOADCELL2_DOUT_PIN = 3;
const int LOADCELL3_DOUT_PIN = 4;
const int LOADCELL4_DOUT_PIN = 5;
const int LOADCELL1_SCK_PIN = 9;
const int LOADCELL2_SCK_PIN = 10;
const int LOADCELL3_SCK_PIN = 11;
const int LOADCELL4_SCK_PIN = 12;

// piezo wiring

//TBD

// Initialization
HX711 Scale;

void setup() 
{
  Serial.begin(9600);
  Scale.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
  Scale.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);
  Scale.begin(LOADCELL3_DOUT_PIN, LOADCELL3_SCK_PIN);
  Scale.begin(LOADCELL4_DOUT_PIN, LOADCELL4_SCK_PIN);
  pinMode(A0, OUTPUT);  
}

void loop() 
{
  digitalWrite(A0, HIGH);  // Turn buzzer ON
  delay(500);
  digitalWrite(A0, LOW);   // Turn buzzer OFF
  delay(500);
}