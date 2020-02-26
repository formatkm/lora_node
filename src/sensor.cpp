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
    digitalWrite(LIGHTSENSOR_PIN, HIGH);
    delay(5);
    unsigned int sensorValue = analogRead(A2);
    //_PL("light:" + String(sensorValue));

    digitalWrite(LIGHTSENSOR_PIN, LOW);
    return sensorValue;
}

float Sensor::getBattery()
{

    float sensorValue = Read_Volts();
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
/**************
 * 以外部电压为基准测量1.1v，反过来计算出外部电压 
 **/
float Sensor::Read_Volts(void)
{
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    if (ADMUX != ADMUX_VCCWRT1V1)
    {
        ADMUX = ADMUX_VCCWRT1V1;

        // Bandgap reference start-up time: max 70us
        // Wait for Vref to settle.
        delayMicroseconds(350);
    }

    // Start conversion and wait for it to finish.
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC))
    {
    };

    // Result is now stored in ADC.

    // Calculate Vcc (in V)
    float vcc = 1.1 * 1024.0 / ADC;

    return vcc;
}
