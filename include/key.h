
#include <Arduino.h>

extern Adafruit_BMP085 bmp;

extern LiquidCrystal_I2C lcd;

extern Lora lora;
#define KEY1 5
#define KEY2 6

int key_count = 0;
void init_key()
{
    pinMode(KEY1, INPUT_PULLUP);
    pinMode(KEY2, INPUT_PULLUP);
}

void key_loop()
{
    //read the pushbutton value into a variable
    int sensorVal1 = digitalRead(KEY1);
    int sensorVal2 = digitalRead(KEY2);

    //key1 down
    if (sensorVal1 == LOW)
    {
        delay(100);

        String str = "T:";
        str += bmp.readTemperature();

        str += ",P:";
        str += bmp.readPressure();

        str += ",A:";
        str += bmp.readAltitude();

        Serial.println(str);

        lora.send_data(str.c_str());
        key_count++;

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