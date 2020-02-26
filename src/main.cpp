#include <Arduino.h>
#include "SimpleSleep.h"
#include <TaskScheduler.h>
#include "config.h"
#include "LoraR02.h"
#include "sensor.h"
#include "btnfunc.h"

Scheduler lora_runner;
Scheduler runner;
Sensor sensor;
LoraR02 lorar02;
BtnFunc btnFunc;
_ReceiveData receiveData;
#include "task.h"
void setup()
{

    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }
    Serial.println("booting....");
#ifdef DESP8266
    ESP8266WiFiClass::preinitWiFiOff();
#endif
    pinMode(LED_PIN, OUTPUT);
    // 光敏电阻供电pin
    pinMode(LIGHTSENSOR_PIN, OUTPUT);
    digitalWrite(LIGHTSENSOR_PIN, LOW);

    analogReference(DEFAULT); //调用板载1.1V基准源
    btnFunc.begin();
    lorar02.begin();

    runner.enableAll(true);
    // lora_runner.enableAll(true);
    LED_OFF
    _PL("running....")
}
//unsigned long previousMillis = 0;
void loop()
{

    lora_runner.execute();
    runner.execute();
    btnFunc.loop();
}