#include "MACResolving.h"

uint8_t strToMAC(const char *strMAC, uint8_t len, uint8_t *MAC, uint8_t size)
{
    memset(MAC, 0, size);
    printf("%s\n\r", strMAC);
    bool up = true;
    uint8_t MAC_index = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        if (strMAC[i] == ':')
        {
            MAC_index++;
            up = true;
        }
        else
        {
            if (up)
            {
                MAC[MAC_index] |= getHEX(strMAC[i]) << 4;
                up = false;
            }
            else
            {
                MAC[MAC_index] |= getHEX(strMAC[i]);
            }
        }
    }
    printf("%d \n\r", MAC_index+1);
    printf("RESOLVED -> %02x:%02x:%02x:%02x:%02x:%02x\n\r", MAC2STR(MAC));
    return MAC_index + 1;
}

uint8_t getHEX(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c = c - 'A' + 'a';
    }
    switch (c)
    {
    case '0':
        return 0x00;
    case '1':
        return 0x01;
    case '2':
        return 0x02;
    case '3':
        return 0x03;
    case '4':
        return 0x04;
    case '5':
        return 0x05;
    case '6':
        return 0x06;
    case '7':
        return 0x07;
    case '8':
        return 0x08;
    case '9':
        return 0x09;
    case 'a':
        return 0x0a;
    case 'b':
        return 0x0b;
    case 'c':
        return 0x0c;
    case 'd':
        return 0x0d;
    case 'e':
        return 0x0e;
    case 'f':
        return 0x0f;
    default:
        return 0x00;
    }
}