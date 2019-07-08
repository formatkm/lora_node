
#ifndef Format_Lora_h
#define Format_Lora_h

#include <Arduino.h>

//lora 控制引脚，发射，睡眠，休眠
#define M0 7
#define M1 8
/*定义lora串口缓冲区长度*/
#define lora_rev_buffer_length 1024

class Lora
{
public:
    void init_lora();
    void e32_sleep();
    void e32_work();

    void e32_reset();
    void e32_setting(char addr1, char addr2, char channel);
    void lora_recv_loop();

    void setup_recv_cb(void (*func)(char *));

    void send_data(char *str);

    void (*function)(char *);

private:
    void get_e32_ver();
    void get_e32_setting();
    char lora_rev_buffer_[lora_rev_buffer_length];
    int buffer_pos_ = 0;
    void run_cmd(unsigned char *cmd, int cmd_length, int return_length);

    bool is_rec_working;
};

#endif