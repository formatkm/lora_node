#ifndef __LORA_TASK_H
#define __LORA_TASK_H

#include "Arduino.h"
#include <TaskSchedulerDeclarations.h>
#include "LoraR02.h"
#include "config.h"

class LoraTask
{
public:
    void received(_ReceiveData data);
    static void show_led_cb(); // 点亮LED2秒
    static void lora_sendping_cb();
    static void recv_packet_cb();

private:
};
#endif