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

class LoraR02
{
public:
    void begin();
    void loop();
    void send(byte destination, String outgoing);
    static void LoRa_rxMode();
    static void LoRa_txMode();

private:
    static void onReceive(int packetSize);
    static void sendMessage(byte destination, String outgoing);
};
#endif