#include "reg51.h"

sbit LED1=0x80;
sbit LED2=0x83;
sbit LED3=0x81;
unsigned int j;
int main()
{
    void delay(unsigned int);

    for(j = 0; j < 3; j++)
    {
        LED1=0;
        LED2=1;
        delay(100000);
        LED1=1;
        LED2=0;
        delay(100000);
    }
    LED2=1;

    EA=1;
    TMOD=0x11;
    PT1=1;

    ET0=1;
    TH0=(65536-5000)/256;
    TL0=(65536-5000)%256;
    TR0=1;
    
    ET1=1;
    TH1=(65536-20000)/256;
    TL1=(65536-20000)%256;
    TR1=1;

    while(1)
    {
        LED3=0;
        delay(5000);
        LED3=1;
        delay(5000);
    };
}

void T0_int(void) interrupt 1
{
    TR0=0;
    TH0=(65536-5000)/256;
    TL0=(65536-5000)%256;
    
    LED1 = 0;
    delay(1000);
    LED1 = 1; 
    delay(1000);

    TR0=1;
}

void T1_int(void) interrupt 3
{
    TR1=0;
    TH1=(65536-20000)/256;
    TL1=(65536-20000)%256;
    
    LED2 = 0;
    delay(4000);
    LED2 = 1; 
    delay(4000);

    TR1=1;
}
void delay(unsigned int count)
{
    unsigned int i;
    for(i=0; i < count; i++);
}