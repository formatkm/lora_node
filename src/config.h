
#ifndef _CONFIG_H
#define _CONFIG_H

#define _DEBUG_

#define LOCALADRESS 0x02 // address of this device
#define VERSION "0.0.20"
// 定义使用频率
#define FREQUENCY 421E6

#define LED_PIN 6
#define LIGHTSENSOR_PIN 4

#define LED_ON digitalWrite(LED_PIN, HIGH);
#define LED_OFF digitalWrite(LED_PIN, LOW);

#ifdef _DEBUG_
#define _PP(a) Serial.print(a);
#define _PL(a) Serial.println(a);
#else
#define _PP(a)
#define _PL(a)
#endif
#endif