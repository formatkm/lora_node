
#include "OneButton.h"
#include "btnfunc.h"
#include "sensor.h"
#include "lorar02.h"
#include "config.h"

extern Sensor sensor;
extern LoraR02 lorar02;
OneButton button(3, true);
void btnClick()
{

    LED_ON

    String content = String(sensor.getLight());
    content += "," + String(sensor.getTemp());
    content += "," + String(sensor.getPressure());

    lorar02.send(0xff, content);
    LED_OFF
}

void BtnFunc::begin(void)
{

    pinMode(3, INPUT_PULLUP);
    button.attachClick(btnClick);
    _PL("key started.")
}

//30秒后自动显
void BtnFunc::loop(void)
{
    // keep watching the push button:
    button.tick();
}
