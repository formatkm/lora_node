
extern LiquidCrystal_I2C lcd;
//十进制转十六进制
String tohex(int n)
{
    if (n == 0)
    {
        return "00"; //n为0
    }
    String result = "";
    char _16[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    const int radix = 16;
    while (n)
    {
        int i = n % radix;        // 余数
        result = _16[i] + result; // 将余数对应的十六进制数字加入结果
        n /= radix;               // 除以16获得商，最为下一轮的被除数
    }
    if (result.length() < 2)
    {
        result = '0' + result; //不足两位补零
    }
    return result;
}

void lcd_clean(int row)
{
    lcd.setCursor(0, row);
    for (int i = 0; i < 16; i++)
    {
        lcd.print(" ");
    }
}

void blink()
{
    // wait for a second
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(50);
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
}