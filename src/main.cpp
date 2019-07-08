#include <Arduino.h>

#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x20 or 0x27 for a 16 chars and 2 line display

Adafruit_BMP085 bmp;

#include <lora.h>
Lora lora;

//-----------加载自定义方法------------------
#include "func.h"
#include "key.h"

//接收到消息回调
void lora_recv_cb(char *buff)
{
    Serial.println(buff);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    init_key();

    // 初始化lora模块
    lora.init_lora();

    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Ready!");

    lora.setup_recv_cb(&lora_recv_cb);
}

void loop()
{
    lora.lora_recv_loop();
    key_loop();
}