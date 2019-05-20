#include "reg51.h"

sbit LED=P1^7;
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
    P1 = 0x06; // Forward run #00001010
    delay(4166);
    P1 = 0x00; // Forward run #00001010
    while (1)
    {
        LED = 0x00;
        delay(100000);
        LED = 0xFF;
        delay(100000);
    }
}

void delay(unsigned int count)
{
    unsigned int j;
    for(j = 0; j < count; j++);
}