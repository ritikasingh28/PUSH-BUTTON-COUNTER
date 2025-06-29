#include <reg51.h>
#define lcd P2

sbit rs = P2^0;
sbit en = P2^1;

sbit rd = P3^7;
sbit wr = P3^6;
sbit INTR = P3^2;

void delay(unsigned int time) {
    unsigned int i, j;
    for(i=0;i<time;i++)
        for(j=0;j<1275;j++);
}

void lcd_cmd(unsigned char cmd) {
    lcd = (cmd & 0xF0);
    RS = 0;
    EN = 1;
    delay(1);
    EN = 0;
    
    lcd = (cmd << 4);
    rs = 0;
    en = 1;
    delay(1);
    en = 0;
}

void lcd_data(unsigned char data) {
    lcd = (data & 0xF0);
    rs = 1;
    en = 1;
    delay(1);
    en = 0;
    
    lcd = (data << 4);
    rs = 1;
    en = 1;
    delay(1);
    en = 0;
}

void lcd_init() {
    lcd_cmd(0x02); 
    lcd_cmd(0x28); 
    lcd_cmd(0x0C); 
    lcd_cmd(0x06); 
    lcd_cmd(0x01); 
}

void lcd_print(char *str) {
    while(*str)
        lcd_data(*str++);
}

unsigned char adc_read() {
    wr = 0; delay(1); wr = 1;     
    while(INTR);                  
    rd = 0;
    delay(1);
    rd = 1;
    return P1;                   
}

void main() {
    unsigned char value;
    float voltage, temp;
    char temp_str[16];

    lcd_init();
    lcd_cmd(0x80);
    lcd_print("Temp: ");

    while(1) {
        value = adc_read();
        voltage = (value * 5.0) / 255;
        temp = voltage * 100;              

        lcd_cmd(0x86);  
        sprintf(temp_str, "%.1f C  ", temp);
        lcd_print(temp_str);
        delay(500);
    }
}
