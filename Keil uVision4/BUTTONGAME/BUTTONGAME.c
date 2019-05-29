#include "reg51.h"
#define GPIO_KEY P2   //矩陣按鍵
sbit ready=0x90;


void delay(unsigned int );
//void test(void);
void key_down(void);
unsigned int LED[3][3] = 
{
    { 0x27, 0x47, 0x87},
    { 0x2B, 0x4B, 0x8B},
    { 0x2D, 0x4D, 0x8D}
};
unsigned int rankKey[5] = {2, 4, 6, 7, 9};

unsigned int RANK[9] = {0x47, 0x4B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void main(void)
{
    unsigned int i;
    for(i = 0; i < 3; i++)
    {
        ready = 0x00;
        delay(10000);
        ready = 0xFF;
        delay(10000);
    }
    //10000010
    IE=0x82;
    TMOD=0x01;
    TH0=(65536-20000)/256;
    TL0=(65536-20000)%256;
    TR0=1;

    while (1)
    {
        ready = 0x00;
        delay(10000);
        ready = 0xFF;
        delay(10000);
    }
}
void key_down(void ) interrupt 1
{
    unsigned int R;
    unsigned int L;
    unsigned int T;
    TR0=0;
    TH0=(65536-20000)/256;
    TL0=(65536-20000)%256;
    TR0=1;
    //reset
    R=0;
    L=0;
    T=0;
    
    GPIO_KEY = 0x0F;
    if( GPIO_KEY != 0x0F )
    { 
        if( GPIO_KEY != 0x0F )
        {
            //H
            switch( GPIO_KEY ){
                case 0x07:
                    R = 0;
                    break;
                case 0x0D:
                    R = 1;
                    break;
                case 0x0B:
                    R = 2;
                    break;
            }
            //L
            GPIO_KEY = 0xF0;
            switch( GPIO_KEY ){
                case 0x70:
                    L = 0;
                    break;
                case 0xD0:
                    L = 1;
                    break;
                case 0xB0:
                    L = 2;
                    break;
            }

            P0 = LED[R][L];
            delay(10000);
            P0=0x00;
            /*
            //如果一直按下按鍵，等待500ms，強制退出
            while( ( T < 5 ) && ( GPIO_KEY != 0xF0 ) ) {
                T++;
            }
            */
        }
    }
}/*
void test(void)
{
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            P0=LED[i][j];
            delay(10000);
            P0=LED[i][j];
            delay(10000);
        }
    }
}*/
void delay(unsigned int count)
{
    unsigned int k;
    for(k = 0; k < count; k++);
}