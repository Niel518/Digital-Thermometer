#ifndef DS18B20_H
#define	DS18B20_H

//Family Code
#define MODEL_DS18B20 0x28

//8-bit CRC Generator
#define CRC 0b8

//Internal EEPROM Commands
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xCC
#define ALARM_SEARCH 0xEC

//DS18B20 Function Commands
#define CONVERT_T 0x44
#define WRITE_SCRATCHPAD 0x4E
#define READ_SCRATCHPAD 0xBE
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2 0xB8
#define READ_POWER_SUPPLY 0xB4

//Conversion Times for each resolution in ms
#define CONV_TIME_9_BIT 94
#define CONV_TIME_10_BIT 188
#define CONV_TIME_11_BIT 375
#define CONV_TIME_12_BIT 750

//Temperature Resolutions
#define RES_9 0x1F
#define RES_10 0x3F
#define RES_11 0x5F
#define RES_12 0x7F

void DS18B20_init(void);
void DS18B20_reset(void);
void DS18B20_DelayMicroseconds(unsigned int us);
void DS18B20_Write(volatile unsigned char data);
char DS18B20_Read(void);

#endif
