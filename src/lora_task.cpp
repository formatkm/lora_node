
#include "lora_task.h"
extern Scheduler lora_runner;
extern LoraR02 lorar02;

_ReceiveData packetData;
boolean isPing = false;
byte pingSource;
unsigned long stopMillis = 0;

Task lora_sendping_task(100, TASK_ONCE, LoraTask::lora_sendping_cb, &lora_runner, false);
Task show_led_cb_task(100, TASK_FOREVER, LoraTask::show_led_cb, &lora_runner, true);
Task recv_packet_task(50, TASK_ONCE, LoraTask::recv_packet_cb, &lora_runner, false);
void LoraTask::lora_sendping_cb()
{
    if (isPing)
    {
        isPing = false;
        lorar02.send(pingSource, "pong");
    }
}
void LoraTask::show_led_cb()
{
    if (stopMillis != 0)
    {
        if (stopMillis > millis())
        {
            _PL("led on")
            LED_ON
        }
        else
        {
            LED_OFF
            stopMillis = 0;
            _PL("led off")
        }
    }
}

void LoraTask::recv_packet_cb()
{
    String title = "RSSI:" + String(packetData.rssi) + " SNR:" + String(packetData.snr) + " 0x" + String(packetData.sender, HEX);
    _PP("recv:")
    _PL(title);

    if (packetData.content == "ping")
    {
        isPing = true;
        pingSource = packetData.sender;
        //启动任务，100毫秒后执行
        lora_sendping_task.enable();
    }
    else if (packetData.content == "led")
    {
        stopMillis = millis() + 2000;
    }
}

void LoraTask::received(_ReceiveData data)
{
    packetData = data;
    recv_packet_task.enable();
}