
#include "lora.h"
#include <SoftwareSerial.h>

/* 软串口 lora串口 */
SoftwareSerial loraSerial(9, 10); // RX, TX;

void Lora::init_lora()
{
    //setup lora
    pinMode(M0, OUTPUT);
    pinMode(M1, OUTPUT);

    // digitalWrite(M0, HIGH);
    // digitalWrite(M1, HIGH);

    loraSerial.begin(9600);
    while (!loraSerial)
    {
        ;
    }

    // 初始化时重置模块，恢复出厂设置
    // todo： 后续改为初始化为自定义设置
    this->e32_reset();

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
    delay(500);
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
        delay(2);

        //接收数据
        char ch = loraSerial.read();

        //判断数据包开始
        if (!is_rec_working && ch == 0xFE)
        {
            is_rec_working = true;
            //重置位置
            buffer_pos_ = -1;
        }

        //数据放入缓冲区
        if (is_rec_working)
        {
            lora_rev_buffer_[buffer_pos_++] = ch;
        }
        //处理包头
        if (buffer_pos_ == 8)
        {
            // 计算包长度
            recv_pack_length = lora_rev_buffer_[6] & 0xFF;
            recv_pack_length |= ((lora_rev_buffer_[7] << 8) & 0xFF00);
            recv_pack_length += 11;
        }

        //处理结束包尾,判断结束标志和接收长度
        if ((ch == 0xFF && buffer_pos_ >= recv_pack_length) || buffer_pos_ >= lora_rev_buffer_length)
        {
            //触发事件
            unsigned int len = buffer_pos_ - 11;
            char payload[len];
            //memset(result, 0, buffer_pos_);
            //memcpy(result, lora_rev_buffer_, buffer_pos_);
            for (unsigned int i = 0; i < len; i++)
            {
                payload[i] = lora_rev_buffer_[8 + len];
            }

            this->function(payload);
            // 完成一次接收
            is_rec_working = false;
        }
    }
}

void Lora::send_data(const char *str, int len)
{
    Serial.print("data length:");
    Serial.println(len);

    unsigned pack_len = len;

    char buffer_[pack_len];
    //构造包头
    buffer_[0] = 0xFE;                //包头
    buffer_[1] = 3;                   //包类型
    buffer_[2] = 23;                  //Node地址，频率
    buffer_[3] = 1;                   //Node地址，模块地址高位
    buffer_[4] = 1;                   //Node地址，模块地址低位
    buffer_[5] = 2;                   //Node地址，自编号
    buffer_[6] = (0xff00 & len) >> 8; //包长度 高位
    buffer_[7] = len;                 //包长度 低位

    int recv_pack_length = 0;
    recv_pack_length = (buffer_[6] & 0xFF) << 8;
    recv_pack_length |= buffer_[7] & 0xFF;

    Serial.print("cale data length:");
    Serial.println(recv_pack_length);

    //写入数据
    for (int i = 0; i < len; i++)
    {
        buffer_[i + 8] = str[i];
    }

    //构造包尾
    buffer_[8 + len] = 0; //crc
    buffer_[9 + len] = 0;
    buffer_[10 + len] = 0xFF;

    //串口发送
    loraSerial.write(buffer_, pack_len);
    Serial.print("Lora Send:");
    Serial.println(str);
}

void Lora::setup_recv_cb(void (*func)(char *))
{
    this->function = func;
}