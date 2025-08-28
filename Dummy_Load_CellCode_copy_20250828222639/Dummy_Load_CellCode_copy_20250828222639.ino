#include <HX711.h>
#include "Arduino_BMI270_BMM150.h"
#include <ArduinoBLE.h>

#define FR_DT_PIN 2
#define FR_SCK_PIN 9
#define BR_DT_PIN 3
#define BR_SCK_PIN 10
#define SI_THRESHOLD 90

HX711 sFR, sBR;  // Front Right and Back Right Load Cells

const float CALIBRATIONS[4] = {954.76, -291.17, 396.39, 985.72};  // Calibration values for the load cells

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

  sFR.tare();
  sBR.tare();

  sFR.set_scale(CALIBRATIONS[0]);
  sBR.set_scale(CALIBRATIONS[1]);

  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  if (IMU.accelerationAvailable() && sFR.is_ready() && sBR.is_ready())
  {
   /* Load Cell Analysis */ 
    long rFR = fabs(sFR.get_units());
    long rBR = fabs(sBR.get_units());
    float total = rFR + rBR;
    float leftNorm = rBR / total;  float rightNorm = rFR / total;
    Serial.print(leftNorm); Serial.print(" "); Serial.println(rightNorm);  //
    float SI = fabs(rightNorm - leftNorm) * 100.0f; 
    float CoPx = (300 * 0.5f) * fabs(rightNorm - leftNorm);

    /* Accelerometer Analysis */
    IMU.readAcceleration(x, y, z);
    angleY = atan2(y, sqrt(x*x + z*z)) * 180 / PI;
    angleX = atan2(x, sqrt(y * y + z * z)) * 180 / PI;

    /* If the chair is falling or the person is slouching, buzz*/
    if ((SI > SI_THRESHOLD) || (angleY > 5 || angleY <-5) || (angleX > 5 || angleX < -5))
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