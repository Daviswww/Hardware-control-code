#include "reg51.h"
sbit LED1=0x80;
sbit ECHO=P3^2;
sbit TRIG=P2^7;

unsigned char j;

unsigned char seg[10] =
{
    0xC0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char buf[4] = {0, 0, 0, 0};
void delay(unsigned int);

void main(void)
{
    while(1)
    {

    }
}

void delay(unsigned int count)
{
    unsigned int k;
    for(k = 0; k < count; k++);
}
