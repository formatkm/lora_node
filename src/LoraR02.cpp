#include "LoraR02.h"
#include <SPI.h> // include libraries
#include <LoRa.h>
#include <TaskSchedulerDeclarations.h>
#include "config.h"
#include "lora_task.h"
// SX1278 has the following connections:
#ifdef DESP8266
// wemos mini d1
#define csPin 15   // LoRa radio chip select
#define resetPin 0 // LoRa radio reset
#define irqPin 2   // change for your board; must be a hardware interrupt pin
#else              //*************/
// arduino nano
#define csPin 10   // LoRa radio chip select
#define resetPin 9 // LoRa radio reset
#define irqPin 2   // change for your board; must be a hardware interrupt pin
#endif

extern LoraR02 lorar02;
extern LoraTask task;
extern _ReceiveData receiveData;
String outgoing;   // outgoing message
byte msgCount = 0; // count of outgoing messages

//------------------配置延时执行任务-------------------------

///////////////////////end 配置延时执行任务//////////////////////////////////////////////

void LoraR02::begin()
{

    Serial.println("LoRa starting...");

    // override the default CS, reset, and IRQ pins (optional)
    LoRa.setPins(csPin, resetPin, irqPin); // set CS, reset, IRQ pin

    //编码率
    //LoRa.setCodingRate4(9);

    if (!LoRa.begin(FREQUENCY))
    { // initialize ratio at 915 MHz
        Serial.println("LoRa init failed. Check your connections.");
        while (true)
            ; // if failed, do nothing
    }
    // 27 为pa_boost
    LoRa.setTxPower(18, 27);
    LoRa.setOCP(140);

    //扩频因子，默认7
    //LoRa.setSpreadingFactor(8);
    LoRa.disableInvertIQ();
    LoRa.onReceive(onReceive);
    LoRa_rxMode();
    task.begin();
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
    task.send(destination, outgoing);
}
void LoraR02::sendMessage(byte destination, String outgoing)
{

    //_PL("begin send.")
    LoRa_txMode();
    //delay(10);
    LoRa.beginPacket();            // start packet
    LoRa.write(destination);       // add destination address
    LoRa.write(LOCALADRESS);       // add sender address
    LoRa.write(msgCount);          // add message ID
    LoRa.write(outgoing.length()); // add payload length
    LoRa.print(outgoing);          // add payload
    LoRa.endPacket();              // finish packet and send it
    msgCount++;                    // increment message ID
    _PP("[Send] ID:" + String(msgCount));
    _PP(" to:0x" + String(destination, HEX));
    _PL(" ,payload: " + outgoing);
    //delay(10);
    LoRa_rxMode();
}

void LoraR02::onReceive(int packetSize)
{
    if (packetSize == 0)
        return; // if there's no packet, return

    // read packet header bytes:
    byte recipient = LoRa.read(); // recipient address
    // if the recipient isn't this device or broadcast,
    if (recipient != LOCALADRESS && recipient != 0xFF)
    {
        Serial.println("This message is not for me.");
        LED_OFF
        return; // skip rest of function
    }

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

    receiveData.rssi = LoRa.packetRssi();
    receiveData.snr = LoRa.getSNR();
    receiveData.sender = sender;
    receiveData.dist = recipient;
    receiveData.id = incomingMsgId;
    receiveData.length = incomingLength;
    receiveData.content = incoming;
    // if message is for this device, or broadcast, print details:
    // _PP("[Recv]RSSI:" + String(receiveData.rssi));
    // _PP(" SNR:" + String(receiveData.snr));
    // _PP(" from: 0x" + String(receiveData.sender, HEX));
    // _PP(" to: 0x" + String(receiveData.dist, HEX));
    // _PP(" ID: " + String(receiveData.id));
    // _PL(" length: " + String(receiveData.length));
    // _PL("Message: " + receiveData.content);
    // _PL("");
    //必须把任务放到一个线程延时执行，否则会重启
    task.received(receiveData);

    LED_OFF
}