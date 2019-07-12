#include <Arduino.h>
#include <SimpleSleep.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x20 or 0x27 for a 16 chars and 2 line display

Adafruit_BMP085 bmp;
SimpleSleep Sleep;
#include <lora.h>
Lora lora;

//-----------加载自定义方法------------------
#include "func.h"
#include "key.h"
KeyBtn keybtn;

//接收到消息回调
void lora_recv_cb(char *buff)
{
    Serial.print("lora recv:");
    Serial.println(buff);

    lcd.setCursor(0, 1);
    lcd.print("recv:");
    lcd.print(buff);
}
//按键1 获取气压湿度，lora发送
void btn1_click_cb()
{
    String str = "T:";
    str += bmp.readTemperature();

    str += ",P:";
    str += bmp.readPressure();

    str += ",A:";
    str += bmp.readAltitude();

    const char *data = str.c_str();
    int length = str.length() + 1;
    lora.send_data(data, length);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // 初始化lora模块
    lora.init_lora();

    lora.e32_work();

    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Ready!");

    lora.setup_recv_cb(&lora_recv_cb);
    //按键回调
    keybtn.init();
    keybtn.setup_btn1_click_cb(btn1_click_cb);
}
unsigned long previousMillis = 0;
void loop()
{
    lora.lora_recv_loop();
    keybtn.loop();

    unsigned long currentMillis = millis();

    //15秒发送一次
    if (currentMillis - previousMillis >= 1000 * 15)
    {
        btn1_click_cb();
    }

    Sleep.deeplyFor(100);
}