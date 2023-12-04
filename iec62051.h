#ifndef __IEC62051__
#define __IEC62051__

/* ******************************************************* */
/*                        INCLUDES                         */
/* ******************************************************* */

#include <stdint.h>

/* ******************************************************* */
/*                          DEFINES                        */
/* ******************************************************* */

#define OSOS_DEBUG  0 /* AS DEFAULT, It is disabled and if you want to use it, please rewrite 1 to activate */

#define READOUT_COMMAND         0x30
#define PROGRAMMING_COMMAND     0x31

/* ******************************************************* */
/*                         TYPEDEF                         */
/* ******************************************************* */

typedef int32_t (*osos_write_t)(uint8_t* content, uint16_t len);
typedef int32_t (*osos_read_t)(uint8_t* buff, uint16_t len);

/* ******************************************************* */
/*                   FUNCTION PROTOTYPE                    */
/* ******************************************************* */

void print_osos(char* msg, int len);

bool start_firstMsg(char* dev_serial_num, int len);

bool send_ackWithMode(uint32_t baudrate, char mod);

bool send_commandMessage(char* obis_code, int port_num);

void init_iec62051(osos_write_t write, osos_read_t read);

#endif /* __IEC62051__ */

/* ******************************************************* */
/*                           END                           */
/* ******************************************************* */