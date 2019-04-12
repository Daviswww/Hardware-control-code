#include "reg51.h"
#include "intrins.h"

sbit trig_pin=P2^6;
sbit echo_pin=P3^2;

unsigned int cm=6666;
unsigned char sample=0;
unsigned char num=0;
unsigned char scanline=0x08;
unsigned char seg[10] =
{
    0xC0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char buf[4] = {0, 0, 0, 0};
void delayMicroseconds(unsigned int);
void convert(unsigned int);
void ping(void);

void Delay_us()
{
	TL0=0xF5;
	TH0=0xFF;
	TR0=1;
	while (TF0==0);
	TR0=0;
	TF0=0;
}

main(void)
{
    EA=1;
    ET1=1;
    TMOD=0x11;
    TH1=(65536-5000)/256;
    TL1=(65536-5000)/256;
    TR1=1;
    P1=0xff;
    while(1)
    {
        ping();
        sample++;
        if(sample >= 50)
        {
            sample=0;
            
            //if(cm >= 2 && cm <= 300)
            {
                convert(cm);
            }
           //convert(9487);
        }
    }
}
void ping(void)
{
    unsigned int temp;
    trig_pin=1;
    Delay_us();
    trig_pin=0;

    while(!echo_pin);
    TR0=1;
    while(echo_pin && !TF0);
    TR0=0;
    temp=TH0;
    temp=(temp << 8) | TL0;
    cm = temp / 58;
    delayMicroseconds(500);
}

void delayMicroseconds(unsigned int count)
{
    unsigned int i;
    for(i=0; i<count-4; i++)
    {
        _nop_();
    }
}

void convert(unsigned int distance)
{
    buf[3]=distance/1000;
    buf[2]=(distance%1000)/100;
    buf[1]=(distance%100)/10;
    buf[0]=distance%10;
}
void T1_int(void) interrupt 3
{
    TH1 = (65536-5000) / 256;
    TL1 = (65536-5000) % 256;
    scanline >>= 1; 
    num++;
    if(scanline == 0)
    {
        scanline = 0x08;
        num = 0;
    }
    P1 = 0xff;
    P2 = ~scanline;
    P1 = seg[buf[num]];
}