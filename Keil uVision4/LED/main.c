#include "reg51.h"

sbit LED=0x90;
unsigned int j;
void main()
{ 
    void delay(unsigned int);
    for(j = 0; j < 3; j++)
    {
        LED=0xFF;
        delay(100000);
        LED=0x00;
        delay(100000);
    }
	while(1)
	{
        P0=0xFF;
        P1=0xFF;
        P2=0xFF;
        delay(100000);
        P0=0x00;
        P1=0x00;
        P2=0x00;
        delay(100000);
	}
}

void delay(unsigned int t)
{
    unsigned int i;
    for(i=0; i<t; i++);
}