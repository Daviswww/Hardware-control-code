/*******************************************************************************
;
; CARAUTO.asm
;
; Car with bluetooth and ultrasonic
;
; SD90 motor speed 60 degree/0.1s, design 90 degree/(20ms*8) then 93 (+-) 4*8 = 125 (2.016ms) or 61 (0.984ms)
;
; HC-SR04 UltraSonic Speed 340m/sec, 5Vdc, 15mA, Work Frequency 40 Hz, Measure Angle 15 degree, Dim. 45x20x15 mm
; Distance 2cm (0.11765ms) ~ 400cm (23.53ms), t = 1cm*2/(340m/sec) = 58.825 us
; 1. Trig: 10us TTL Pulse, then HC-SR04 generates 8 pulses (40kHz)
; 2. Echo: time period, t = 1cm*2/(340m/sec) = 58.825 us
;
; Use Interrupt USART_RXC for bluetooth and TIMER0 MODE1 for control arduino-car
;
; Created: 2016/12/03
; Author : Chung-Chuan Hou
; CLOCK : 11.059 MHz for baud rate 9600
;
;******************************************************************************/

#include"reg51.h"


void convert(unsigned int);
void delay(unsigned int s);
sbit P27 = P2^7;
sbit P32 = P3^2;
//sbit SMOD= PCON^7;
unsigned char PSW_BUF; // SAVE PSW in USART INT
unsigned char SBUF_BUF; // SAVE SBUF for motor control
unsigned char AUTO; // auto = 0x00 is manual car, auto = 0xff is auto car
unsigned char DIST; // ULTRASONIC DISTANCE
unsigned char DIST_BUF; // SAVE DIST
unsigned char ATEMP,PSWTEMP,DPLTEMP,DPHTEMP;
unsigned char sample=0;
unsigned char num=0;
unsigned char scanline=0x08;
unsigned char seg[10] =
{
    0xC0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char buf[4] = {0, 0, 0, 0};
void main()
{
	TMOD = 0x21; //	SET TIMER1 MODE 2 and TIMER0 MODE 1. (00100001B)
	PT0 = 1; //IP.1 SET TIMER0 INTERRUPT HIGH PRIORITY
	TF0 = 0; //TCON.5 CLEAR TIMER0 OVERFLOW
	TH0 = (65536-20000)/256;
	TL0 = (65536-20000)%256; // -20000 = 45536 = #B1E0H
	TR0 = 1; //TCON.4 SET TIMER0 RUN

	TH1 = 253; // baud rate 9600=11059000/(32x12x(256-253))
	TR1 = 1; //TCON.6 SET TIMER1 RUN
	PCON = 0x00;
//	SMOD = 0; // SET PCON.7=0 (SMOD=0)
	SCON = 0x50; // SET RS232 MODE1, RX ENABLE
// IE.7(SET ALL INTERRUPT ENABLE);IE.1(ENABLE TIMER0 INTERRUPT); IE.4 (ENABLE USART INT)
	IE   = 0x92; // 10010010B

// Initial value
	P0 = 0x00; // observe SBUF
	P1 = 0x00; // motor stop
	P2 = 0x00; // ultrasonic stop
	AUTO = 0x00; // auto = 0x00 is manual car, auto = 0xff is auto car

// Main-Flow ********************************************
	while(1)
	{
		DIST = 0x00; // Reset DIST
		P27 = 1; // Trig for ULTRASONIC
		delay(1); // 12 us
		P27 = 0; // Trig for ULTRASONIC		
//-------------------------------------------------------
//		while(P32 == 0) // echo wait
		delay(33); // 400 us
		while(P32 == 1) // echo on
		{
			delay(5); // 5*12us = 60us approximately 58.8us
			DIST = DIST + 1; // distance increases 1cm
			if (DIST > 250)
			{
				DIST = 250;
			}
			convert(DIST);
		}
		DIST_BUF = DIST; // echo off
//-------------------------------------------------------
		if (AUTO == 0xFF) // auto = 0x00 is manual car, auto = 0xff is auto car
		{
			if (DIST_BUF > 10)
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
//-------------------------------------------------------

	} // Main-Flow ***************************************
} // Main

void delay(unsigned int s)
{
	unsigned int m;
	for(m=0;m<s;m++);
}

/******************************************************************************
; INTERRUPT  USART
;******************************************************************************/
void Serial (void) interrupt 4
{
	PSW_BUF = PSW; // save PSW
	RI = 0; // clear RI Flag
	P0 = SBUF; // observe SBUF
	SBUF_BUF = SBUF; // save SBUF for motor control

// 'C' = #43H
	if (SBUF_BUF == 0x43)
	{
		//AUTO = !AUTO; // error using 20180504 
		AUTO = ~AUTO;// auto = 0x00 is manual car, auto = 0xff is auto car
		P1 = 0x00; // motor stop
	}

	PSW = PSW_BUF; // get PSW
}

/******************************************************************************
; TIMER0_INT 
;******************************************************************************/
void Timer0(void) interrupt 1
{
	ATEMP  =ACC;
	PSWTEMP=PSW;
	DPLTEMP=DPL;
	DPHTEMP=DPH;
	TR0=0; // STOP TIMER
	TH0  = (65536-20000)/256;
	TL0  = (65536-20000)%256;
	TR0=1; // TIMER0 TO BE CONTINUE

	if (AUTO == 0x00) // auto = 0x00 is manual car, auto = 0xff is auto car
	{
		if (SBUF_BUF == 0x51) // stop, SBUF ='Q' =#51H
		{
			P1 = 0x00; // motor stop
		}
		else if (SBUF_BUF == 0x53) // back, SBUF ='S' =#53H
		{
			P1 = 0x05; // motor back, #00000101B	
		}
		else if (SBUF_BUF == 0x44) // right, SBUF ='D' =#44H
		{
			P1 = 0x09; // motor right, #00001001B
		}
		else if (SBUF_BUF == 0x41) // left, SBUF ='A' =#41H
		{
			P1 = 0x06; // motor left, #00000110B
		}
		else if (SBUF_BUF == 0x57) // forward, SBUF ='W' =#57H
		{
			if (DIST_BUF > 10)
			{
				P1 = 0x0A; // motor forward, #00001010B
			}
			else
			{
				P1 = 0x00; // motor stop, short distance
			}
		}
		else
		{}
	}

// RETURN:
	ACC=ATEMP;
	PSW=PSWTEMP;
	DPL=DPLTEMP;
	DPH=DPHTEMP;
}
void Timer1(void) interrupt 3
{
    TH1 = (65536-5000) / 256;
    TL1 = (65536-5000) % 256;
    scanline >>= 1; 
    num++;
    if(scanline == 0)
    {
        scanline = 0x08;
        num = 0;
    }
    P0 = 0xff;
    P2 = ~scanline;
    P0 = seg[buf[num]];
}

void convert(unsigned int distance)
{
    buf[3]=distance/1000;
    buf[2]=(distance%1000)/100;
    buf[1]=(distance%100)/10;
    buf[0]=distance%10;
}
/*
void main()
void INT0 (void) interrupt 0 using 0 // using bank0
void Timer0 (void) interrupt 1 using 1 // using bank1
void INT1 (void) interrupt 2 using 2 // using bank2
void Timer1 (void) interrupt 3 using 3 // using bank3
void Serial (void) interrupt 4

	ORG     0000H           ;
	JMP     START           ;

	ORG     0003H           ;
	JMP     EXTERNAL_INT0   ;EXTERNAL INT0 INTERRUPT

	ORG     000BH           ;
	JMP     TIMER0_INT      ;INTERNAL TIMER0 INTERRUPT

	ORG     0013H           ;
	JMP     EXTERNAL_INT1   ;EXTERNAL INT1 INTERRUPT

	ORG     001BH           ;
	JMP     TIMER1_INT      ;INTERNAL TIMER1 INTERRUPT

	ORG     0023H           ;
	JMP     SERIAL_TRANS    ;SERIAL TRANSMISSION INTERRUPT
*/
