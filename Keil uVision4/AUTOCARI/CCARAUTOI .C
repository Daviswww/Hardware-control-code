#include "reg51.h"
void delay(unsigned int s);
sbit LED = 0x80;
sbit P27 = P2^7;
sbit P32= P3^2; //sbit SMOD=RCON^7;
unsigned char PSW_BUF;  //SAVE PSW in USART INT
unsigned char SBUF_BUF;  //SAVE SBUF for motor control
unsigned char AUTO;   //auto = 0x00 is manual car,auto = 0xff is auto car
unsigned char DIST;   //ULTRASONIC DISTANCE
unsigned char DIST_BUF;  //SAVE DIST
unsigned char gg;
unsigned char ATEMP,PSWTEMP,DPLTEMP,DPHTEMP;

void main()
{
	for(gg = 0; gg < 3; gg++)
	{
		LED=0x00;
		delay(10000);
		LED=0xFF;
		delay(10000);
	}
	
    TMOD = 0x21;              //SET TIMER1 MODE2 and TIMER0 MODE1. (00100001B)
	PT0 = 1;               //IP.1 SET TIMER0 INTERRUPT HIGH PRIORITY
	TF0 = 0;               //TCON.5 CLEAR TIMER0 OVERFLOW
	TH0 = (65536-20000)/256;
	TL0 = (65536-20000)%256; //-20000 = 45536 = #B1E0H
	TR0 = 1;                 //TCON.4 SET TIMER0 RUN

	TH1 = 253;     //baud rate 9600=11059000/(32*12*(256-253))
	TR1 = 1;     //TCON.6 SET TIMER1 RUN
	PCON = 0x00;    //SMOD = 0;//SET PCON.7=0 (SMOD=0)
	SCON = 0x50;    //SET RS232 MODE1. RX ENABLE
			//IE.7(SET ALL INTERRUPT ENABLE);IE.1(ENABLE TIMER0 INTERRUPT);IE.4(ENABLE USART INT)
	IE = 0x92;     //10010010B
	//Initial value
	P0 = 0xFF;     //observe SBUF
	P1 = 0x00;     //motor stop
	P2 = 0x00;     //ultrasonic stop
	AUTO = 0x00;    //auto = 0x00 is manual car, auto = 0xff is auto car
	//Main Flow------------------------------------------------------------------
	while(1)
	{
		DIST = 0x00;   //Reset DIST
		P27 = 1;    //Trig for ULRRASONIC
		delay(1);    //12 us
		P27 = 0;    //Trig for ULTRASONIC
		//---------------------------------------------------------------------------
		//    WHILE(P32 == 0)   // echo vait 
		delay(45);      //400us
		while(P32 == 1)     //echo on
		{
			delay(5);     //5*12us = 60us approximately 58.8us
			DIST = DIST + 1; //distance increase 1cm
			if (DIST > 250)
			{
				DIST = 250;
			}
		}
 
		DIST_BUF = DIST;   //echo off
		//--------------------------------------------------
		if(AUTO == 0xFF)   //auto = 0x00 is manual car. auto = 0xff is auto car
		{
			if(DIST_BUF > 10)
			{
				P1 = 0x0A;    //Forward turn #00001010
			}
			else
			{                 
				P1 = 0x00;    //motor stop
				delay(8333);   //8333*12us = 0.1sec
				P1 = 0x06;    //motor turn left #00000110
				delay(41667);   //41667*12us = 0.5sec
				P1 = 0x00;    //motor stop
			}
		}
//---------------------------------------------------
	}//Main-Flow****************************************
}//Main

void delay(unsigned int s)
{ 
	unsigned int m;
	for(m=0;m<s;m++);
}

//---------------------------------
//INTERRUPT USART   RS232cut
//---------------------------------
void Serial (void) interrupt 4
{
	PSW_BUF = PSW;       // save PSW
	RI = 0;      //clear RI Flag
	//P0 = SBUF;     //observe SBUF
	SBUF_BUF = SBUF;    //save SBUF for motor control

	//'C' = #43H change
	if (SBUF_BUF == 0x43)
	{
		AUTO = ~AUTO;    //auto = 0x00 is manual car. auto = 0xff is auto car
		P1 = 0x00;     //motor stop  ,all MODE will become zero
	}

	PSW = PSW_BUF;    //cat PSW
}


//-------------------------------------
//TIMER0 INT
//--------------------------------------
void Timer0(void)  interrupt 1 
{
   ATEMP = ACC;
   PSWTEMP = PSW;
   DPLTEMP = DPL;
   DPHTEMP = DPH;
   TR0 = 0;        //stop timer
   TH0 = (65536-20000)/256;
   TL0 = (65536-20000)%256;
   TR0 = 1;        //TIMER0 TO BE CONTINUS

	if(AUTO == 0x00)      //auto = 0x00 is manual car. auto = 0xff is auto car
	{
		if (SBUF_BUF == 0x51)    //stop. SBUF ='O' = #51H
		{
			P1 = 0x00;      //MOTOR stop
			P0 = 0xFF;
		}
		else if(SBUF_BUF == 0X53)   //back. SBUF ='S' = #53H
		{
			P1=0x05;     //motor back. #00000101B
			P0=0x09;
		} 
		else if(SBUF_BUF == 0X44) //right. SBUF = 'D' = #44H
		{
			P1=0x09;     //motor RIGHT. #00001001B
			P0=0x0C;
		} 
		else if(SBUF_BUF == 0X41) //left. SBUF ='A' = #41H
		{
			P1=0x06;     //moyor left. #00000110B
			P0=0x03;
		} 
		else if(SBUF_BUF == 0X57) //forward. SBUF ='W' = #57H
		{ 
			if(DIST_BUF > 10)
			{
				P1 = 0x0A;   //motor forward. #00001010B
				P0=0x06;
			}
			else
			{
				P1 = 0x00;   //motor stop. short distance
				P0=0xFF;
			}
		}
		else
		{}
	}

	//RETURM;
	ACC=ATEMP;
	PSW=PSWTEMP;
	DPL=DPLTEMP;
	DPH=DPHTEMP;

}