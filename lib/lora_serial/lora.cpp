
#include "lora.h"

#include <SoftwareSerial.h>
/* 软串口 lora串口 */
SoftwareSerial loraSerial(9, 10); // RX, TX;

void Lora::init_lora()
{
    //setup lora
    pinMode(M0, OUTPUT);
    pinMode(M1, OUTPUT);

    digitalWrite(M0, HIGH);
    digitalWrite(M1, HIGH);

    loraSerial.begin(9600);
    while (!loraSerial)
    {
        ;
    }

    Serial.println("Lora Serial Ready!");
}

void Lora::e32_sleep()
{
    digitalWrite(M0, HIGH);
    digitalWrite(M1, HIGH);
    delay(10);
}

void Lora::e32_work()
{
    digitalWrite(M0, LOW);
    digitalWrite(M1, LOW);
    delay(10);
}
/**
   运行命令
   cmd  命令
   cmd_length:命令长度，必须传入，否则sizeof是指针的大小
   return_length： 返回数据的长度，判断是否读取结束
*/
void Lora::run_cmd(unsigned char *cmd, int cmd_length, int return_length)
{
    e32_sleep();

    loraSerial.write(cmd, cmd_length);

    char buffer_[return_length];
    int buffer_pos_ = 0;
    while (buffer_pos_ < return_length)
    {
        if (loraSerial.available() > 0)
        {
            int ch = loraSerial.read();
            buffer_[buffer_pos_++] = ch;
        }
    }
    Serial.println(buffer_);
    //return return_str;
}

void Lora::get_e32_ver()
{
    unsigned char hexdata[3] = {0xC3, 0xC3, 0xC3};

    run_cmd(hexdata, sizeof(hexdata), 4);
}

/**
   重启模块
*/
void Lora::e32_reset()
{

    unsigned char hexdata[3] = {0xC4, 0xC4, 0xC4};

    run_cmd(hexdata, sizeof(hexdata), 0);
    delay(1000);
}

void Lora::get_e32_setting()
{
    unsigned char hexdata[3] = {0xC1, 0xC1, 0xC1};

    run_cmd(hexdata, sizeof(hexdata), 6);
}
/**
   设置模块参数
   addr1：地址段1(0-255)
   addr2：地址段2(0-255)
   channel:频段(0-31)对应频率410-441
*/
void Lora::e32_setting(char addr1, char addr2, char channel = 23)
{

    unsigned char hexdata[6];
    hexdata[0] = 0xC2;
    hexdata[1] = addr1;
    hexdata[2] = addr2;
    hexdata[3] = 0x1A;
    hexdata[4] = channel;
    hexdata[5] = 0x44;

    run_cmd(hexdata, sizeof(hexdata), 6);
}

void Lora::lora_recv_loop()
{

    while (loraSerial.available() > 0)
    {
        char ch = loraSerial.read();
        delay(2);
        //判断数据包开始
        if (!is_rec_working && ch == 0xFE)
        {
            is_rec_working = true;
        }

        if (is_rec_working)
        {
            lora_rev_buffer_[buffer_pos_++] = ch;
        }
        //处理包头
        if (buffer_pos_ == 8)
        {
        }

        //处理结束包尾
        if (ch == 0xFF || buffer_pos_ >= 11)
        {
            //lora_rev_buffer_[buffer_pos_++] = '\0';

            //触发事件
            this->function(lora_rev_buffer_);
            //重置位置
            buffer_pos_ = 0;
            is_rec_working = false;
        }
    }
}

void Lora::send_data(char *str)
{
    //获取字符串长度
    int len = sizeof(str);

    char buffer_[11 + len];
    //构造包头
    buffer_[0] = 0xFE;
    buffer_[6] = len << 8;
    buffer_[7] = len;

    //写入数据
    for (int i = 0; i < len; i++)
    {
        buffer_[i + 8] = str[i];
    }

    //构造包尾
    buffer_[10 + len] = 0xFF;
    //串口发送
    loraSerial.write(buffer_);
}

void Lora::setup_recv_cb(void (*func)(char *))
{
    this->function = func;
}