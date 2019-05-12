#include "reg51.h"
char one=0;
char zero=0;
unsigned char KeyTemp=0xff;
unsigned char KeyData=0xff;
unsigned char num=4;
char seg[] = {
    0xc0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90};

main()
{
    void KeypadScan(void);
    while(1)
    {
        KeypadScan();
        if(KeyData >= 0 && KeyData <= 9)
        {
            P0=KeyData;
            KeyData=0xff;
        }
    }
}

void KeypadScan(void)
{
    char col, row;
    char ScanLine=0x80;
    unsigned char key=0;
    unsigned char KeyStatus;
    for(col = 0; col < num; col++)
    {
        P0=0xff;
        P2=~ScanLine;
        KeyStatus=~P2;
        P0=seg[buf[num-col-1]];
        KeyStatus &= 0xf0;
        for(row = 0; row < 4; row++)
        {
            if(KeyStatus == 0x80)
            {
                one=0;
                if(KeyTemp != key)
                {
                    KeyTemp=key;
                    zero=1;
                }
            }
            else
            {
                if(zero < 5)
                {
                    zero += 1;
                    if(zero == 5)
                    {
                        KeyData = KeyTemp;
                    }
                }
            }
            key += 1;
            KeyStatus <<= 1;
        }
        ScanLine >>= 1;
    }
    one += 1;
    if(one == 5)
    {
        zero = 0;
        KeyTemp = 0xff;
        KeyData = 0xff;
    }
}