#include <Arduino.h>
#include <SimpleSleep.h>

#include <TaskScheduler.h>
#include "config.h"
#include "LoraR02.h"
#include "sensor.h"
#include "btnfunc.h"

SimpleSleep Sleep;
Scheduler lora_runner;
Scheduler runner;
Sensor sensor;
LoraR02 lorar02;
BtnFunc btnFunc;
_ReceiveData receiveData;
#include "task.h"
void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    lorar02.begin();
    btnFunc.begin();
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