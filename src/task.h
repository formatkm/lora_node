#ifndef __task_h
#define __task_h

#include "Arduino.h"
#include <TaskSchedulerDeclarations.h>

#include "config.h"
#include "LoraR02.h"
#include "sensor.h"
//extern LoRa lora;
extern Scheduler runner;
extern LoraR02 lorar02;
extern Sensor sensor;

void loop_send_cb()
{
    sensor.getBMP();
    String content = String(sensor.getLight());
    content += "," + String(sensor.getBattery());
    content += "," + String(sensor.getTemp());
    content += "," + String(sensor.getPressure());
    lorar02.send(0xff, content);
}
Task loop_send_task(TASK_SECOND * 2, TASK_FOREVER, &loop_send_cb, &runner);

#endif