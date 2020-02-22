#ifndef _SENSOR_H_
#define _SENSOR_H_

class Sensor
{
public:
    void getBMP();
    int16_t getLight();
    int16_t getBattery();
    int32_t getPressure();
    float getTemp();
    float getAltitude();

private:
    int32_t pressure;
    float temp;
    float altitude;
};

#endif