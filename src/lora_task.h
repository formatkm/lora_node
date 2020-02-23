#ifndef __LORA_TASK_H
#define __LORA_TASK_H

#include "Arduino.h"
#include <TaskSchedulerDeclarations.h>
#include "LoraR02.h"
#include "config.h"

class LoraTask
{
public:
    void begin();
    void received(_ReceiveData data);
    void send(byte destination, String outgoing);
    static void show_led_cb();          // 点亮LED2秒
    static void loop_lora_sending_cb(); //list发送任务
    static void recv_packet_cb();

private:
};
#endif