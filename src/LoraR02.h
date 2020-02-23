#ifndef _LoraR02_H
#define _LoraR02_H

#include <Arduino.h>

struct _ReceiveData
{
    byte sender;
    byte dist;
    byte id;
    byte length;
    String content;
    int rssi;
    int snr;
};

struct _LoRaSendMsg
{
    byte dist;
    String content;
};

class LoraR02
{
public:
    void begin();
    void loop();
    void send(byte destination, String outgoing);
    static void LoRa_rxMode();
    static void LoRa_txMode();
    static void sendMessage(byte destination, String outgoing);

private:
    static void onReceive(int packetSize);
};
#endif