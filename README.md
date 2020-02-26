# lora_arduino
arduino base  lora sx1278 node 


# 接线
* SPI接口
```
SCK  <----> D13
MISO <----> D12
MOSI <----> D11
NSS  <----> D10
REST <----> D09
DIO0 <----> D02 (中断)
```

* I2C
```
SDA  <----> A4
SCL  <----> A5
```

* 其他
```
LED  <----> D06
KEY  <----> D03
电池  <----> A1
光敏  <----> A2
光敏供电 <----> D4
```

> 节约电能,在光敏电阻测量时候使用拉高D4对模块供电,其他时候低电平.