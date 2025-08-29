#include <HX711.h>
#include "Arduino_BMI270_BMM150.h"
#include <ArduinoBLE.h>

#define FR_DT_PIN 2
#define FR_SCK_PIN 9
#define BR_DT_PIN 3
#define BR_SCK_PIN 10
#define CELL1_DT_PIN 4
#define CELL1_SCK_PIN 11
#define CELL2_DT_PIN 5
#define CELL2_SCK_PIN 12
#define SI_MAX_THRESHOLD 80
#define SI_MIN_THRESHOLD 10

HX711 sFR, sBR, s1, s2;  // Front Right and Back Right Load Cells

const float CALIBRATIONS[4] = {954.76, -291.17, 26.11, 16.60};  // Calibration values for the load cells

const int buzzerPin = 6;

float x, y, z;    // Variables to hold accelerometer data
int angleX = 0;  // Variable to hold calculated angles
int angleY = 0; // Variable to hold calculated angles

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  sFR.begin(FR_DT_PIN, FR_SCK_PIN);
  sBR.begin(BR_DT_PIN, BR_SCK_PIN);
  s1.begin(CELL1_DT_PIN, CELL1_SCK_PIN);
  s2.begin(CELL2_DT_PIN, CELL2_SCK_PIN);
  

  sFR.tare();
  sBR.tare();
  s1.tare();
  s2.tare();

  sFR.set_scale(CALIBRATIONS[0]);
  sBR.set_scale(CALIBRATIONS[1]);
  s1.set_scale(CALIBRATIONS[2]);
  s2.set_scale(CALIBRATIONS[3]);

  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  if (IMU.accelerationAvailable() && sFR.is_ready() && sBR.is_ready() && s1.is_ready() && s2.is_ready())
  {
   /* Load Cell Analysis */ 
    long rFR = fabs(sFR.get_units());
    long rBR = fabs(sBR.get_units());
    long r1 = fabs(s1.get_units());
    long r2 = fabs(s2.get_units());
    float total = rFR + rBR + r1 + r2;
    float leftNorm = (rBR + r1) / total;  float rightNorm = (rFR + r2) / total;
    Serial.print(leftNorm); Serial.print(" "); Serial.println(rightNorm);  //
    float SI = fabs(rightNorm - leftNorm) * 100.0f; 
    Serial.println(SI);
    float CoPx = (300 * 0.5f) * fabs(rightNorm - leftNorm);

    /* Accelerometer Analysis */
    IMU.readAcceleration(x, y, z);
    angleY = atan2(y, sqrt(x*x + z*z)) * 180 / PI;
    angleX = atan2(x, sqrt(y * y + z * z)) * 180 / PI;

    /* If the chair is falling or the person is slouching, buzz*/
    if ((SI > SI_MAX_THRESHOLD) || (SI <= SI_MIN_THRESHOLD) || (angleY > 5 || angleY <-5) || (angleX > 5 || angleX < -5))
    {
      tone(buzzerPin, 500);
      delay(1000);
      noTone(buzzerPin);
      delay(1000);
    }
    else
    {
      noTone(buzzerPin);
    }
  }
}