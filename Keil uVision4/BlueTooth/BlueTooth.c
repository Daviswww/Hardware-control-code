#include "reg51.h"
sbit LED1=0x90;
sbit LED2=0x80;
unsigned char j;
void main(void)
{
    void delay(unsigned int);
    unsigned char mychar;

    TMOD=0x20;
    TH1=0xFD;       //9600=12000000/(32x12x(256-x))
    SCON=0x50;      //01010000
    PCON = 0x00;

    for(j = 0 ; j < 3; j++)
    {
        LED1=0x00;
        delay(10000);
        LED1=0xff;
        delay(10000);
    }    
    TR1=1;
    while(1)
    {
        while(RI==0);
        mychar=SBUF;
        RI=0;

        LED1=0x00;
        delay(10000);
        LED1=0xff;
        delay(10000);

        if(mychar == 'A')
        {
            LED2=0x00;
        }
        else if(mychar == 'a')
        {
            LED2=0xFF;
        }
    }
}

void delay(unsigned int s)
{
	unsigned int m;
	for(m=0;m<s;m++);
}