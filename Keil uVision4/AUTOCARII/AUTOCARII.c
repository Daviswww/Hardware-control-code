#include "reg51.h"
sbit LED1=0x80;
sbit ECHO=P3^2;
sbit TRIG=P2^7;

unsigned char j;
unsigned char AUTO;
unsigned char SBUFTMP;
unsigned char DIST, SDIST;
unsigned char MYCHAR, TMPCHAR;
void main(void)
{
    void delay(unsigned int);
    for(j = 0 ; j < 3; j++)
    {
        LED1=0x00;
        delay(10000);
        LED1=0xFF;
        delay(10000);
    }
    ES=1;           //Serial
    EA=1;           //Timer

    TMOD=0x21;      //00100001
    SCON=0x50;      //01010000
    PCON=0x00;
    AUTO=0xFF;      //set
    TH1=0xFD;       //253
    TR1=1;          //open timer 1

    ET0=1;
    ET1=1;
    TH0=(65536-20000)/256;
    TL0=(65536-20000)%256;
    TR0=1;          //open timer 0
    
    while(1)
    {
		DIST = 0x00; // Reset DIST
		TRIG = 1; // Trig for ULTRASONIC
		delay(1); // 12 us
		TRIG = 0; // Trig for ULTRASONIC		
		delay(33); // 400 us
		while(ECHO == 1) // echo on
		{
			delay(5); // 5*12us = 60us approximately 58.8us
			DIST = DIST + 1; // distance increases 1cm
			if (DIST > 250)
			{
				DIST = 250;
			}
		}
        SDIST=DIST;
        if(AUTO==0x00)
        {
			if (SDIST > 10)
			{
				P1 = 0x0A; // Forward run #00001010
			}
			else
			{
				P1 = 0x00; // motor stop
				delay(8333); // 8333*12us = 0.1 sec
				P1 = 0x06; // motor turn left #00000110
				delay(41667); // 41667*12us = 0.5sec
				P1 = 0x00; // motor stop
			}
        }
    }
}
void SCON_int(void) interrupt 4
{
    if(RI==1)
    {
        RI = 0;
        MYCHAR = SBUF;
        LED1=0x00;
        delay(1000);
        LED1=0xFF;
        delay(1000);
    }
    if(MYCHAR == 'E')
    {
        AUTO= 0xFF;
        P1 = 0x00;
    }
    else if(MYCHAR == 'R')
    {
        AUTO= 0x00;
        P1 = 0x00;
    }
}
void T0_int(void) interrupt 1
{
	TR0=0;
	TH0  = (65536-20000)/256;
	TL0  = (65536-20000)%256;
	TR0=1;

    if(AUTO==0xFF)
    {
        if(MYCHAR == 'W')
        {
            P1 = 0x0A; // motor forward, #00001010B

        }
        else if(MYCHAR == 'A')
        {
            P1 = 0x06; // motor left,    #00000110B
        }
        else if(MYCHAR == 'S')
        {
            P1 = 0x05; // motor back,    #00000101B

        }
        else if(MYCHAR == 'D')
        {
            P1 = 0x09; // motor right,   #00001001B

        }
        else if(MYCHAR == 'Q')
        {
            P1 = 0x00; // motor stop
        }
    }
}

void delay(unsigned int count)
{
	unsigned int i;
	for(i = 0; i < count; i++);
}