#include <HX711.h>

#define FR_DT_PIN 2
#define FR_SCK_PIN 9
#define BR_DT_PIN 3
#define BR_SCK_PIN 10

HX711 sFR, sBR;

const float CALIBRATIONS[4] = {954.76, -291.17, 396.39, 985.72}; 

const int buzzerPin = 6;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  sFR.begin(FR_DT_PIN, FR_SCK_PIN);
  sBR.begin(BR_DT_PIN, BR_SCK_PIN);
  //sFL.begin(FL_DT_PIN, FL_SCK_PIN);
  sFR.tare();
  sBR.tare();
  //sFL.tare();
  sFR.set_scale(CALIBRATIONS[0]);
  sBR.set_scale(CALIBRATIONS[1]);
  //sFL.set_scale(CALIBRATIONS[2]);
}

void loop()
{
  if (sFR.is_ready() && sBR.is_ready())
  {
    long rFR = fabs(sFR.get_units());
    long rBR = fabs(sBR.get_units());
    //long rFL = sFL.get_units();
    //Serial.print(rFR); Serial.print(" "); Serial.println(rBR); //Serial.print(" "); //Serial.println(rFL);

    float total = rFR + rBR;
    float leftNorm = rBR / total;  float rightNorm = rFR / total;
    Serial.print(leftNorm); Serial.print(" "); Serial.println(rightNorm);
    float SI = fabs(rightNorm - leftNorm) * 100.0f; 
    float CoPx = (300 * 0.5f) * fabs(rightNorm - leftNorm);

    if (SI > 60 && CoPx > 60)
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
    Serial.println(CoPx);
  }
}