#include "reg51.h"

sbit LED=0x80;


void main()
{ 
    void delay(unsigned int);
	while(1)
	{
        LED=0xFF;
        delay(100000);
        LED=0x00;
        delay(100000);
	}
}

void delay(unsigned int t)
{
    unsigned int i;
    for(i=0; i<t; i++);
}