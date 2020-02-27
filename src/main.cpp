
#include "config.h"
#include <Arduino.h>
#include "SimpleSleep.h"
// #define _TASK_TIMECRITICAL      // Enable monitoring scheduling overruns
//#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
// #define _TASK_LTS_POINTER       // Compile with support for local task storage pointer
// #define _TASK_PRIORITY          // Support for layered scheduling priority
// #define _TASK_MICRO_RES         // Support for microsecond resolution
// #define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 and ESP32 ONLY)
// #define _TASK_DEBUG             // Make all methods and variables public for debug purposes
// #define _TASK_INLINE            // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
// #define _TASK_TIMEOUT           // Support for overall task timeout
// #define _TASK_OO_CALLBACKS      // Support for dynamic callback method binding
#include <TaskScheduler.h>

#include "LoraR02.h"
#include "sensor.h"
#include "btnfunc.h"
#include "lora_task.h"

Scheduler runner;
Sensor sensor;
LoraR02 lorar02;
BtnFunc btnFunc;
_ReceiveData receiveData;
LoraTask task;
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

    runner.allowSleep(true);

    runner.enableAll(true);
    // lora_runner.enableAll(true);
    LED_OFF
    _PL("running....")
}
//unsigned long previousMillis = 0;
void loop()
{
    runner.execute();

    btnFunc.loop();
}