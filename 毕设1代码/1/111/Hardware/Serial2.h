#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init2(void);
void Serial_SendByte2(uint8_t Byte);
uint8_t Serial_GetRxFlag_2(void);
int fputc2(int ch, FILE *f);
void Serial_SendString2(char *String);
void Serial_Printf2(char *format, ...);

#endif
