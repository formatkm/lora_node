#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include "sensor.h"
#include "config.h"

Adafruit_BMP085 bmp;
void Sensor::getBMP()
{
    if (bmp.begin())
    {
        temp = bmp.readTemperature();
        pressure = bmp.readPressure();
        altitude = bmp.readAltitude();
        // _PL("temp:" + String(temp));
        // _PL("press:" + String(pressure));
        // _PL("altitude:" + String(altitude));
    }
    else
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
}
int16_t Sensor::getLight()
{
    unsigned int sensorValue = analogRead(A2);
    //_PL("light:" + String(sensorValue));
    return sensorValue;
}

int16_t Sensor::getBattery()
{
    unsigned int sensorValue = analogRead(A1);
    //_PL("battery:" + String(sensorValue));
    return sensorValue;
}

int32_t Sensor::getPressure()
{
    return pressure;
}
float Sensor::getTemp()
{
    return temp;
}
float Sensor::getAltitude()
{
    return altitude;
}