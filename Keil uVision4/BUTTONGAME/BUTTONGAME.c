#include "reg51.h"

sbit LED = 0x90;
unsigned int i;
void delay(unsigned int );
void main(void)
{
    for(i = 0; i < 3; i++)
    {
        LED = 0x00;
        delay(100000);
        LED = 0xFF;
        delay(100000);
    }

    while (1)
    {

        P0=80;
    }
}

void delay(unsigned int count)
{
    unsigned int j;
    for(j = 0; j < count; j++);
}