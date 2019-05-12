#include <reg52.h>

#define GPIO_7SEG P0   //7SEG
#define GPIO_KEY P2   //矩陣按鍵
sbit LED = 0x90;
unsigned char key;
unsigned char j;
unsigned char seg[16] = {0xC0, 0xF9, 0xA4, 0xB0, 
                         0x99, 0x92, 0x82,
                         0xF8, 0x80, 0x90};
main ()
{
    void key_down(void);
    void delay( unsigned int);
    
    for(j = 0; j < 2; j++)
    {
        LED = 0x00;
        delay(10000);
        LED = 0xff;
        delay(10000);
    }
    for(j = 0; j < 10; j++)
    {
        GPIO_7SEG = seg[j];
        delay(10000);

    }
    GPIO_7SEG = 0xC0;
    while( 1 ) 
    {
        key_down(); 
    }
}

void delay( unsigned int count ){
    unsigned int i;
    for(i = 0; i < count; i++);
}

void key_down()
{
    unsigned char count = 0;
    //H
    GPIO_KEY = 0x0F;
    if( GPIO_KEY != 0x0F )
    { 
        if( GPIO_KEY != 0x0F )
        {
            switch( GPIO_KEY ){
                case 0x07:
                    key = 1;
                    break;
                case 0x0B:
                    key = 2;
                    break;
                case 0x0D:
                    key = 3;
                    break;
            }
            //L
            GPIO_KEY = 0xF0;
            switch( GPIO_KEY ){
                case 0x70:
                    key = key;
                    break;
                case 0xB0:
                    key = key + 3;
                    break;
                case 0xD0:
                    key = key + 6;
                    break;
            }

            GPIO_7SEG = seg[key];

            //如果一直按下按鍵，等待500ms，強制退出
            while( ( count < 50 ) && ( GPIO_KEY != 0xF0 ) ) {
                count++;
            }
        }
    }
}