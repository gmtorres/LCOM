#pragma once

#include <lcom/lcf.h>

#define BIT(n) (0x01<<(n))

#define COM1 0X3F8
#define COM2 0X2F8

#define COM1_IRQ 4
#define COM2_IRQ 3


#define RBR 0  //Receiver buffer
#define THR 0  //Transmitter Holding
#define IER 1  //Interrupt Enable
#define IIR 2  // Interrupt Identification
#define FCR 2  //FIFO Control
#define LCR 3  //Line Control
#define MCR 4  //Modem Control
#define LSR 5  //Line Status
#define MST 6  //Modem Status
#define SR 7   //Scratchpad

#define DLL 0
#define DLM 1


//LCR line control register
#define CHAR_LENGHT_5 0x00
#define CHAR_LENGHT_6 0x01
#define CHAR_LENGHT_7 0x02
#define CHAR_LENGHT_8 0x03

#define STOP_BIT_1 0
#define STOP_BIT_2 BIT(2)

#define NO_PARITY 0
#define ODD_PARITY BIT(3)
#define EVEN_PARITY BIT(3) | BIT(4)
#define PARITY_BIT_1 BIT(5) | BIT(3)
#define PARITY_BIT_10 BIT(5) | BIT(3) | BIT(4)

#define BREAK_CONTROL BIT(6)
#define DLAB BIT(7)



//LSR line status register
#define DATA BIT(0)
#define OVERRUN_ERR BIT(1)
#define PARITY_ERR BIT(2)
#define FRAME_ERR BIT(3)
#define BREAK_INTERRUPT BIT(4)
#define TX_RDY BIT(5)
#define TX_HOLDING_EMPTY BIT(5)
#define TX_EMPTY BIT(6)
#define FIFO_ERR BIT(7)



// IER interrupt enable register

#define RECEIVED_DATA_INT_ENB BIT(0)
#define TX_HOLDING_EMPTY_INT_ENB BIT(1)
#define RECEIVED_LINE_STATUS_INT_ENB BIT(2)
#define MODEM_STATUS_INT_ENB BIT(3)



//IIR interrupt identification register

#define NO_INT BIT(0)
#define RECEIVER_LINE_STATUS BIT(2) | BIT(1)
#define RECEIVED_DATA_AVAILABLE BIT(2)
#define IIR_INT_ID 0x0E


//FCR FIFO control register

#define ENABLE_BOTH_FIFO BIT(0)
#define CLEAR_RCVR_FIFO BIT(1)
#define CLEAR_XMIT_FIFO BIT(2)
#define ENB_64_FIFO BIT(5)

#define FIFO_TRIGGER_LEVEL_1 0
#define FIFO_TRIGGER_LEVEL_4 BIT(6)
#define FIFO_TRIGGER_LEVEL_8 BIT(7)
#define FIFO_TRIGGER_LEVEL_14 BIT(7) | BIT(8)



int sp1_subscribe_int(uint8_t * bit_no);
int sp2_subscribe_int(uint8_t * bit_no);

int sp1_unsubscribe_int();
int sp2_unsubscribe_int();

void sp1_config();

void sp1_send_char(uint8_t c);
void sp1_send_char2(uint8_t c);
void sp1_send_int(int v);
bool sp1_receive_char(uint8_t *c);

void sp1_read_char();
