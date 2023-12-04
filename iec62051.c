/**
 * @author AHMET AKTAŞ 
 * @date
 * @copyright You can use this lib or change it just with a condition that indicate my name in description
 *            For example; This code develop over AHMET AKTAŞ code's
 * @note If you have any question please contact on me aktasahmet4info@gmail.com
*/

/* ******************************************************* */
/*                        INCLUDES                         */
/* ******************************************************* */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "iec62051.h"

/* ******************************************************* */
/*                          DEFINES                        */
/* ******************************************************* */

#define OSOS_DEBUGE(fmt, ...)\
        do{ if(OSOS_DEBUG){ printf("\033[0;31m" "ERROR: %s:%d:%s(): " #fmt "\r\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} }while (false);

#define OSOS_DEBUGI(fmt, ...)\
        do{ if(OSOS_DEBUG) {printf("\033[0;37m" "INFO: %s:%d:%s(): " #fmt "\r\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} }while (false);


#define	CR 		'\r'
#define	LF 		'\n'

#define	ACK		0x06
#define	NACK	0x15

#define	SOH		0x01
#define	STX		0x02
#define	ETX		0x03
#define	EOT		0x04


/* ******************************************************* */
/*                         TYPEDEF                         */
/* ******************************************************* */

struct 
{
    osos_write_t osos_write;

    osos_read_t  osos_read;
}iec;


/* ******************************************************* */
/*                   PRIVATE FUNCTIONS                     */
/* ******************************************************* */

static char select_baud(uint32_t baudrate)
{
    char ret_val = -1;
    
    switch(baudrate)
    {
        case 300:
            ret_val = 0x30; break;
        case 600:
            ret_val = 0x31; break;
        case 1200:
            ret_val = 0x32; break;
        case 2400:
            ret_val = 0x33; break;
        case 4800:
            ret_val = 0x34; break;
        case 9600:
            ret_val = 0x35; break;
        case 19200:
            ret_val = 0x36; break;
        default:
            MTLOGE("Invalid baud rate options");
            break;
    }
    return ret_val;
}


/* ******************************************************* */
/*                    GLOBAL FUNCTIONS                     */
/* ******************************************************* */

void print_osos(char* msg, int len)
{
    for(int i=0; i<len; i++)
    {
        switch(msg[i])
        {
            case '/':    printf("/ ");     break;
            
            case '?':    printf("? ");     break;
            
            case '!':    printf("! ");     break;
            
            case '\r':   printf("CR");     break;
            
            case '\n':   printf("LF\r\n"); break;

            case '\x06': printf("ACK");    break;

            case '\x00': printf("0");      break;

            case '\x01': printf("SOH");    break;

            case '\x02': printf("STX");    break;

            case '\x03': printf("ETX");    break;

            case '\x04': printf("EOT");    break;
            
            case 'v'   : printf("v");      break;

            case '\x0B': printf("LF");     break;

            case '\x0C': printf("VT");     break;
            
            default: printf("%c", msg[i]); break;
        }
    }
    printf("\r\n");
}

bool start_firstMsg(char* dev_serial_num, int len)
{
    char* start_msg = "/?";
    char* end_msg   = "!\r\n";

    int  buff_len = sizeof(start_msg) + len + sizeof(end_msg);

    char buffer[buff_len] = {0};
    int  offset = 0;
    
    memcpy(buffer, start_msg, strlen(start_msg));
    offset = strlen(start_msg);
    memcpy(buffer + offset, dev_serial_num, len);
    offset = offset + len;
    memcpy(buffer + offset, end_msg, strlen(end_msg));

    if(iec.osos_write(buffer, buff_len) <= 0){
        return false;
    }

    return true;
}

bool send_ackWithMode(uint32_t baudrate, char mod)
{
    char baud = 0;
    char readout_message[6] = {0x06, 0x30, baud, mod, 0x0D, 0x0A};

    baud = select_baud(baudrate);
    readout_message[2] = baud;
    

    if(iec.osos_write(&readout_message, sizeof(readout_message)) <= 0){
        OSOS_DEBUGE("Uart write is failed");
        return false;
    }

    OSOS_DEBUGI("Uart write is successed");

    return true;
}

bool send_commandMessage(char* obis_code, int port_num)
{
	int16_t i=0;
	char    bcc = 0;
    char    command[13] = {0};

	command[0] = SOH;
	command[1] = 'R';
	command[2] = '2';
	command[3] = STX;

	bcc = command[1] ^ command[2] ^ command[3];

	for (i = 0; obis_code[i]!='\0'; ++i)
	{
		command[i+4] = obis_code[i];
		bcc = bcc ^ obis_code[i];
	}

	command[i+4] = ETX;
	bcc = bcc ^ command[i+4];
	command[i+5] = bcc;

    OSOS_DEBUGI("BCC = 0x%02X", bcc);
    
    if(iec.osos_write(command, sizeof(command)) < 0){
                OSOS_DEBUGE("Send commend is failed");
        return false;

    }

    OSOS_DEBUGI("Send commend is succeed");

    return true;
}


void init_iec62051(osos_write_t write, osos_read_t read)
{
    iec.osos_write = write;

    iec.osos_read  = read;  
}

/* ******************************************************* */
/*                           END                           */
/* ******************************************************* */