#include "reg51.h"
sbit LED = 0x90;
unsigned int j;

main()
{
   
    void delay(unsigned int);
    for(j = 0; j < 3; j++)
    {
        LED = 0x00;
        delay(100000);
        LED = 0xff;
        delay(100000);
    }
    while(1)
    {
        P2 = 0xff;
        delay(100000);
        P2 = 0xE8;
        delay(100000);

    }
}

void delay(unsigned int count)
{
    unsigned int i;
    for(i = 0; i < count; i++);
}