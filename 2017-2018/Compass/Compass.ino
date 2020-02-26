/*
    Reads the direction your compass sensor is pointing in degrees
    The sensor is pointing North when the X axis marking is pointing North

    Place the sensor on a flat surface and avoid anything magnetic that
    may skew the results.
*/


#include <Wire.h>
#include "HMC5883Llib.h"


int xCal = 156;
int yCal = 210;


int dataPin = 2;
int triggerPin = 3;
int LEDPin = A0;
int safeFrequency = 100;   // microseconds
uint16_t bitGaurd = 1;
bool trigger = true;
bool sending = true;

uint16_t compassDir = 0;

Magnetometer mag;
bool fail;

int8_t readHeadingDegHack(double * heading)
{
    int16_t x, y, z;

    int8_t ret = mag.readRaw(&x, &y, &z);

//    Serial.print("Y: ");
//    Serial.println(y);

     x+=xCal;
     y+=yCal;

    double tan_heading = atan2(y, x);
    // Correct for when signs are reversed.
    if(tan_heading < 0)
        tan_heading += 2 * M_PI;

    // convert to degrees
    *heading = tan_heading * 180 / 3.141592; 

    return 0;
}


void sendBit(int myBit)
{
  digitalWrite(dataPin, myBit);
  trigger = !trigger;
  digitalWrite(triggerPin, trigger);
  analogWrite(LEDPin, sending * 255);
}

void sendNumber(int32_t fullNumber)
{
  for (int i = 15; i >= 0; i--)
  {
    uint32_t temp = fullNumber;   // store value in temp
    temp = temp >> i;             // shift to get correct bit
    temp = temp & bitGaurd;       // ignore everything except last bit

    sendBit(temp);
    delayMicroseconds(safeFrequency);
  }

  fullNumber = ((fullNumber + 13) * -9); // checksum

  for (int i = 15; i >= 0; i--)
  {
    uint32_t temp = fullNumber;   // store value in temp
    temp = temp >> i;             // shift to get correct bit
    temp = temp & bitGaurd;       // ignore everything except last bit

    sendBit(temp);
    delayMicroseconds(safeFrequency);
  }

  /*for (int i = 0; i<16; i++)
    {
    sendBit(1);
    delayMicroseconds(safeFrequency);
    }*/
  sending = !sending;
}

void setup()
{

  pinMode(dataPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  Serial.begin(57600);

  if (mag.begin() != 0)
  {
    Serial.println("Error connecting to Magnetometer");
    fail = true;
    return;
  }

  // set the amount of gain - Use the most sensitive
  // for reading the earths magnetic field
  //
  // MSB/Gauss   Field Range
  // 1370     +- 0.88 Ga
  // 1090     +- 1.3 Ga
  // 820      +- 1.9 Ga
  // 660      +- 2.5 Ga
  // 440      +- 4.0 Ga
  // 390      +- 4.7 Ga
  // 330      +- 5.6 Ga
  // 230      +- 8.1 Ga
  mag.setGain(HMC5833L_GAIN_1370);
}

void loop()
{
  // don't bother reading if we failed to connect
  if (fail)
    return;

  // reads the heading in degrees using the X and Y axis
  double heading;
  int8_t ret = readHeadingDegHack(&heading);
  
  switch (ret)
  {
    case HMC5833L_ERROR_GAINOVERFLOW:
      Serial.println("Gain Overflow");
      return;
    case 0:
      // success
      break;
    default:
      Serial.println("Failed to read Magnetometer");
      sending = 0;
      return;
  }

  // print them out
 

  Serial.print("Heading: ");
  Serial.println(heading);


  compassDir = floor(heading * 10);
  for (int i = 0; i < 1; i++)
    sendNumber(compassDir);
  //sendNumber(1234);
  //delay(3);
}
