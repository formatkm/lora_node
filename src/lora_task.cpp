
#include "lora_task.h"
extern Scheduler lora_runner;
extern LoraR02 lorar02;

_ReceiveData packetData;
boolean isPing = false;
unsigned long stopMillis = 0;

Task lora_sendping_task(200, TASK_ONCE, LoraTask::lora_sendping_cb, &lora_runner, false);
Task show_led_cb_task(200, TASK_FOREVER, LoraTask::show_led_cb, &lora_runner, true);
Task recv_packet_task(200, TASK_ONCE, LoraTask::recv_packet_cb, &lora_runner, false);
void LoraTask::lora_sendping_cb()
{
    if (isPing)
    {
        isPing = false;
        lorar02.send(packetData.sender, "pong");
    }
}
void LoraTask::show_led_cb()
{
    if (stopMillis != 0)
    {
        if (stopMillis > millis())
        {
            LED_ON
        }
        else
        {
            LED_OFF
            stopMillis = 0;
            lorar02.send(packetData.sender, "led off");
            _PL("led off")
        }
    }
}

void LoraTask::recv_packet_cb()
{
    String title = "RSSI:" + String(packetData.rssi) + " SNR:" + String(packetData.snr) + " 0x" + String(packetData.sender, HEX);
    _PP("[Recv]:")
    _PL(title);
    _PP("[Recv]:")
    _PL(packetData.content);
    _PL("")

    if (packetData.content == "ping")
    {
        isPing = true;

        //启动任务，100毫秒后执行
        lora_sendping_task.restart();
    }
    else if (packetData.content == "led")
    {
        stopMillis = millis() + 2000;
    }
}

void LoraTask::received(_ReceiveData data)
{
    packetData = data;
    // _PL("start recv_packet_task")
    recv_packet_task.restart();
}