
#include <Arduino.h>

extern Adafruit_BMP085 bmp;

extern LiquidCrystal_I2C lcd;

extern Lora lora;
#define KEY1 5
#define KEY2 6

class KeyBtn
{
private:
    /* data */
    int key_count = 0;
    void (*btn1_click)();

public:
    void init()
    {
        pinMode(KEY1, INPUT_PULLUP);
        pinMode(KEY2, INPUT_PULLUP);
    }

    void setup_btn1_click_cb(void (*click_cb)())
    {
        this->btn1_click = click_cb;
    }

    void loop()
    {
        //read the pushbutton value into a variable
        int sensorVal1 = digitalRead(KEY1);
        int sensorVal2 = digitalRead(KEY2);

        //key1 down
        if (sensorVal1 == LOW)
        {
            delay(100);

            
            key_count++;
            this->btn1_click();

            lcd.setCursor(0, 0);
            lcd.print("key1:");
            lcd.print(key_count);
            lcd.print(" Sended!");

            delay(100);
        }
        else if (sensorVal2 == LOW)
        {
            delay(100);
            key_count++;

            lcd.setCursor(0, 1);
            lcd.print("key2downs:");
            lcd.print(key_count);
            delay(100);
        }
    }
};
