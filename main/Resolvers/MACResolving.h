#ifndef __MAC_RESOLVING_H__
#define __MAC_RESOLVING_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
uint8_t strToMAC(const char *strMAC, uint8_t len, uint8_t *MAC, uint8_t size);
uint8_t getHEX(char c);

#endif