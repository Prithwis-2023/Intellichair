#include "HX711.h"
#include "Arduino_BMI270_BMM150.h"

#define MINIMUM_TILT 20

float x, y, z;
int angleX = 0;
int angleY = 0;

// HX711 circuit wiring
const int LOADCELL_DOUT_PINS[4] = {2, 3, 4, 5};
const int LOADCELL_SCK_PINS[4] = {9, 10, 11, 12};
const float CALIBRATIONS[4] = {1.0, 1.0, 1.0, 1.0};  // accomplished based on the documentation: github.com/RobTillaart/HX711/tree/master
const int chassisLength = 300;  // TBD
const int SI_THRESHOLD = 15;
const int CoPx_THRESHOLD = 30;  // in mm

const int buzzerPin = 6;  // passive buzzer

HX711 scaleFR, scaleBR, scaleBL, scaleFL; 

void setup() 
{
  Serial.begin(57600);  // suggested baud rate online

  while(!Serial);

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  scaleFR.begin(LOADCELL_DOUT_PINS[0], LOADCELL_SCK_PINS[0]);
  scaleBR.begin(LOADCELL_DOUT_PINS[1], LOADCELL_SCK_PINS[1]);
  scaleBL.begin(LOADCELL_DOUT_PINS[2], LOADCELL_SCK_PINS[2]);
  scaleFL.begin(LOADCELL_DOUT_PINS[3], LOADCELL_SCK_PINS[3]);
  
  // calibrate the scales
  scaleFR.set_scale(CALIBRATIONS[0]);
  scaleBR.set_scale(CALIBRATIONS[1]);
  scaleBL.set_scale(CALIBRATIONS[2]);
  scaleFL.set_scale(CALIBRATIONS[3]);
}

void loop() 
{
  if (IMU.accelerationAvailable() && scaleFR.is_ready() && scaleBR.is_ready() && scaleBL.is_ready() && scaleFL.is_ready()) 
  {
    IMU.readAcceleration(x, y, z);
    angleY = atan2(y, sqrt(x*x + z*z)) * 180 / PI;
    angleX = atan2(x, sqrt(y * y + z * z)) * 180 / PI;

    float readingFR = fabs(scaleFR.get_units(10));
    float readingBR = fabs(scaleBR.get_units(10));
    float readingBL = fabs(scaleBL.get_units(10));
    float readingFL = fabs(scaleFL.get_units(10));
    float total = readingFR + readingBR + readingBL + readingFL;
    
    Serial.print("HX711 reading: ");

    float rightSideNorm = (readingFR + readingBR) / total; 
    float leftSideNorm = (readingBL + readingFL) / total;
    Serial.print(leftSideNorm); Serial.print(" "); Serial.println(rightSideNorm);

    float SI = fabs(rightSideNorm - leftSideNorm) * 100.0f;
    float CoPx = (chassisLength * 0.5f) * fabs(rightSideNorm - leftSideNorm);
    Serial.println(SI);
    Serial.println(CoPx);

    if ((SI >= SI_THRESHOLD && CoPx >= CoPx_THRESHOLD) || (angleY >= MINIMUM_TILT || angleX >= MINIMUM_TILT) || (angleY <= -MINIMUM_TILT || angleX <= -MINIMUM_TILT))
    {
      tone(buzzerPin, 1000);  // strong warning
    }
    else
    {
      noTone(buzzerPin);
    }
  } 
  delay(1000); 
}