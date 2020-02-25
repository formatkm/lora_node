
#include <LinkedList.h>
#include "lora_task.h"
extern Scheduler lora_runner;
extern LoraR02 lorar02;

LinkedList<_LoRaSendMsg *> sendList = LinkedList<_LoRaSendMsg *>();
//QList<_LoRaSendMsg> sendList; //发送字符串链表
_ReceiveData packetData;
boolean isPing = false;
unsigned long stopMillis = 0;

Task loop_lora_sending_task(1000, TASK_FOREVER, LoraTask::loop_lora_sending_cb, &lora_runner, true);
Task show_led_cb_task(200, TASK_FOREVER, LoraTask::show_led_cb, &lora_runner, true);
Task recv_packet_task(200, TASK_ONCE, LoraTask::recv_packet_cb, &lora_runner, false);

void LoraTask::begin()
{
    loop_lora_sending_task.enable();
}

void LoraTask::loop_lora_sending_cb()
{
    if (sendList.size() > 0)
    {
        while (sendList.size() > 0)
        {
            _LoRaSendMsg *msg = sendList.shift();
            LoraR02::sendMessage(msg->dist, msg->content);
            //删除指针
            delete msg;
        }
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
        lorar02.send(packetData.sender, "pong");
    }
    else if (packetData.content == "led")
    {
        stopMillis = millis() + 2000;
    }
}

void LoraTask::received(_ReceiveData data)
{
    packetData = data;
    _PL("start recv_packet_task")
    recv_packet_task.restart();
}

void LoraTask::send(byte destination, String outgoing)
{
    //_PL("add to send list.")

    _LoRaSendMsg *msg = new _LoRaSendMsg();
    msg->dist = destination;
    msg->content = outgoing;
    sendList.add(msg);
}