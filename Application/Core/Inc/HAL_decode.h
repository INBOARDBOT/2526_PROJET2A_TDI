#ifndef HAL_DECODE_H
#define HAL_DECODE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define START_BYTE 0x42
#define SEP_BYTE   0x21
#define END_BYTE   0x45
#define RX_BUF_SIZE 64


void decodeFrame(uint8_t *buf, uint8_t len);

#endif