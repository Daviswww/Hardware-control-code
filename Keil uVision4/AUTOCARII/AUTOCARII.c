#include "reg51.h"
sbit LED1=0x80;
sbit trig=P3^5;

unsigned int cms;
unsigned char j;
unsigned char AUTO;
unsigned char SBUFTMP;
unsigned char MYCHAR, TMPCHAR;
unsigned char seg[10] =
{
    0xC0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char buf[4] = {0, 0, 0, 0};
void convert(unsigned int);
void display(unsigned char );
void delayz(unsigned int);
void delay(unsigned int);
void send_pulse(void);
unsigned int get_range(void);
void updata(void);
void main(void)
{
    //ready
    for(j = 0 ; j < 3; j++)
    {
        LED1=0x00;
        delay(10000);
        LED1=0xFF;
        delay(10000);
    }
    ES=1;           //Serial
    EA=1;           //Timer

    TMOD=0x29;      //open timer1 and timer0 in 16 bit mode with gate enable
    SCON=0x50;      //01010000
    PCON=0x00;
    AUTO=0xFF;      //set NOT AUTO
    TH1=0xFD;       //253
    ET1=1;
    TR1=1;          //open timer 1

    TR0=1;//timer run enabled
    TH0=0x00;
    TL0=0x00;
    P3|=0x04;//setting pin P3.2
    while(1)
    {  
        EA=0;           //close Timer
        get_range();    //get cms
        delayz(2);
        if(AUTO==0x00)
        {
			if (cms > 20)  //>20cms go go
			{
				P1 = 0x0A; // Forward run #00001010
			}
			else
			{
            	P1 = 0x00; // motor stop
                delay(8333);
                P1 = 0x05; // motor back
                delay(8333);
				P1 = 0x06; // motor turn left #00000110
				delay(8333); // 41667*12us = 0.5sec

			}
        }
        EA=1;           //open Timer 
        delay(200);     //wait order
    }
}
void SCON_int(void) interrupt 4
{
    if(RI==1)      //wait ri
    {
        RI = 0;
        MYCHAR = SBUF;
        LED1=0x00;
        delay(1000);
        LED1=0xFF;
        delay(1000);
    }
    //car order
    if(MYCHAR == 'E')
    {
        //open auto
        AUTO= 0x00;
    }
    else if(MYCHAR == 'F')
    {
        //close auto
        AUTO= 0xFF;
    }
    else if(AUTO==0xFF)
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

void send_pulse(void) 
{
    TH0=0x00;TL0=0x00;
    trig=1; //Sending trigger pulse P3^^5
    delayz(10); //Wait for about 10milliseconds 
    trig=0; //Turn off trigger
}
unsigned int get_range(void)
{
    long int timer_val,i=0;
    send_pulse();
    while(!INT0); //Waiting until echo pulse is detected
    while(INT0); //Waiting until echo pulse changes its state
    timer_val=(TH0<<8)+TL0;
    if(timer_val<38000)
    {
        cms=timer_val/59;
        if ((cms!=0)&&(cms<100))
        {
            convert(cms);
            updata();
        }
        else
        {
            cms=99;
            updata();
        }
    }
    else
    {
        cms=99;
        updata();
    }
    return cms;
}
void convert(unsigned int distance)
{
    buf[3]=distance/1000;
    buf[2]=(distance%1000)/100;
    buf[1]=(distance%100)/10;
    buf[0]=distance%10;
}
void updata(void)
{
    P2 = 0x0e;
    P0 = seg[buf[3]];
    delay(200);
    P2 = 0x0d;
    P0 = seg[buf[2]];
    delay(200);
    P2 = 0x0b;
    P0 = seg[buf[1]];
    delay(200);
    P2 = 0x07;
    P0 = seg[buf[0]];
    delay(200);
}
void delayz(unsigned int n)
{
    unsigned char i;
    for(n;n>0;n--)
    {
        for(i=250;i>0;i--);
        for(i=247;i>0;i--);
    }
}
void delay(unsigned int count)
{
	unsigned int i;
	for(i = 0; i < count; i++);
}