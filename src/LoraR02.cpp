#include "LoraR02.h"
#include <SPI.h> // include libraries
#include <LoRa.h>
#include <TaskSchedulerDeclarations.h>
#include "config.h"

// SX1278 has the following connections:

#define csPin 10   // LoRa radio chip select
#define resetPin 9 // LoRa radio reset
#define irqPin 2   // change for your board; must be a hardware interrupt pin

extern Scheduler lora_runner;

extern LoraR02 lorar02;

extern _ReceiveData receiveData;
String outgoing;   // outgoing message
byte msgCount = 0; // count of outgoing messages

boolean isPing = false;
byte pingSource;

//------------------配置延时执行任务-------------------------
void loop_loraping_cb()
{
    if (isPing)
    {
        isPing = false;
        lorar02.send(pingSource, "pong");
    }
}
Task loop_loraping_task(800, TASK_ONCE, &loop_loraping_cb, &lora_runner, false);

void recv_packet_cb()
{
    String title = "RSSI:" + String(receiveData.rssi) + " SNR:" + String(receiveData.snr) + " 0x" + String(receiveData.sender, HEX);
    _PP("recv:")
    _PL(title);

    if (receiveData.content == "ping")
    {

        isPing = true;
        pingSource = receiveData.sender;
        //启动任务，500毫秒后执行
        loop_loraping_task.restart();
    }
}
Task recv_packet_task(50, TASK_ONCE, &recv_packet_cb, &lora_runner, false);
///////////////////////end 配置延时执行任务//////////////////////////////////////////////

void LoraR02::begin()
{

    Serial.println("LoRa starting...");

    // override the default CS, reset, and IRQ pins (optional)
    LoRa.setPins(csPin, resetPin, irqPin); // set CS, reset, IRQ pin

    //信号带宽 越小越远
    // LoRa.setSignalBandwidth(7.8E3);
    //扩频因子，实际设置为12
    //LoRa.setSpreadingFactor(14);

    //编码率
    //LoRa.setCodingRate4(9);
    //#ifdef SERVER // 网关模块功率最大
    //LoRa.setTxPower(20);
    //LoRa.setOCP(140);
    //#else
    //LoRa.setTxPower(4);
    //LoRa.setOCP(50);
    //#endif
    if (!LoRa.begin(FREQUENCY))
    { // initialize ratio at 915 MHz
        Serial.println("LoRa init failed. Check your connections.");
        while (true)
            ; // if failed, do nothing
    }
    LoRa.disableInvertIQ();
    LoRa.onReceive(onReceive);
    LoRa_rxMode();
    Serial.println("LoRa init succeeded.");
}

void LoraR02::LoRa_rxMode()
{

    // LoRa.disableInvertIQ(); // normal mode
    //LoRa.sleep();
    LoRa.receive(); // set receive mode
}

void LoraR02::LoRa_txMode()
{
    LoRa.idle(); // set standby mode
                 //  LoRa.enableInvertIQ(); // active invert I and Q signals
}
void LoraR02::send(byte destination, String outgoing)
{
    LoraR02::sendMessage(destination, outgoing);
}
void LoraR02::sendMessage(byte destination, String outgoing)
{

    LoRa_txMode();
    delay(10);
    LoRa.beginPacket();            // start packet
    LoRa.write(destination);       // add destination address
    LoRa.write(LOCALADRESS);       // add sender address
    LoRa.write(msgCount);          // add message ID
    LoRa.write(outgoing.length()); // add payload length
    LoRa.print(outgoing);          // add payload
    LoRa.endPacket();              // finish packet and send it
    msgCount++;                    // increment message ID
    _PL("Sending " + outgoing);
    delay(10);
    LoRa_rxMode();
}

void LoraR02::onReceive(int packetSize)
{
    if (packetSize == 0)
        return; // if there's no packet, return

    // read packet header bytes:
    byte recipient = LoRa.read();      // recipient address
    byte sender = LoRa.read();         // sender address
    byte incomingMsgId = LoRa.read();  // incoming msg ID
    byte incomingLength = LoRa.read(); // incoming msg length

    String incoming = ""; // payload of packet
    LED_ON
    while (LoRa.available())
    {                                  // can't use readString() in callback, so
        incoming += (char)LoRa.read(); // add bytes one by one
    }

    if (incomingLength != incoming.length())
    { // check length for error
        Serial.println("error: message length does not match length");
        LED_OFF
        return; // skip rest of function
    }

    // if the recipient isn't this device or broadcast,
    if (recipient != LOCALADRESS && recipient != 0xFF)
    {
        Serial.println("This message is not for me.");
        LED_OFF
        return; // skip rest of function
    }

    receiveData.rssi = LoRa.packetRssi();
    receiveData.snr = LoRa.getSNR();
    receiveData.sender = sender;
    receiveData.dist = recipient;
    receiveData.id = incomingMsgId;
    receiveData.length = incomingLength;
    receiveData.content = incoming;
    // if message is for this device, or broadcast, print details:
    _PL("Received from: 0x" + String(receiveData.sender, HEX));
    _PL("Sent to: 0x" + String(receiveData.dist, HEX));
    _PL("Message ID: " + String(receiveData.id));
    _PL("Message length: " + String(receiveData.length));
    _PL("Message: " + receiveData.content);
    _PL("RSSI: " + String(receiveData.rssi));
    _PL("Snr: " + String(receiveData.snr));
    _PL("");
    //必须把任务放到一个线程延时执行，否则会重启
    recv_packet_task.restart();

    LED_OFF
}