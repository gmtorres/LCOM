#include "serial_port.h"

int sp1_hook_id = COM1_IRQ;
int sp2_hook_id = COM2_IRQ;

int sp1_subscribe_int(uint8_t * bit_no){
  *bit_no = sp1_hook_id;
  int r;
  if((r =sys_irqsetpolicy( COM1_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE , &sp1_hook_id)) != OK){
    return r;
  }
  return 0;

}

int sp2_subscribe_int(uint8_t * bit_no){
  *bit_no = sp2_hook_id;
  int r;
  if((r =sys_irqsetpolicy( COM2_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE , &sp2_hook_id)) != OK){
    return r;
  }
  return 0;

}

int sp1_unsubscribe_int(){
  int r;
    if ((r = sys_irqdisable(&sp1_hook_id)) != OK)
      return r;
    if((r = sys_irqrmpolicy(&sp1_hook_id)) != OK)
      return r;
    return 0;
}
int sp2_unsubscribe_int(){
  int r;
    if ((r = sys_irqdisable(&sp2_hook_id)) != OK)
      return r;
    if((r = sys_irqrmpolicy(&sp2_hook_id)) != OK)
      return r;
    return 0;
}






void sp1_config(){

  //dlab
  sys_outb(COM1 + LCR , CHAR_LENGHT_8 | STOP_BIT_2 | DLAB | EVEN_PARITY);

  sys_outb(COM1 + DLL, 0x0C); //9600
  sys_outb(COM1 + DLM, 0x00);

  //data
  sys_outb(COM1 + LCR , CHAR_LENGHT_8 | STOP_BIT_2 | EVEN_PARITY);

  //fifo

  //interrupts
  sys_outb(COM1 + IER , RECEIVED_DATA_INT_ENB );

}


void sp1_send_char(uint8_t c){
  uint32_t lsr;
  sys_inb(COM1 + LSR , &lsr);
  if(lsr & TX_HOLDING_EMPTY){
    sys_outb(COM1 + THR , c);
  }
}

void sp1_send_char2(uint8_t c){
  sp1_send_char(0);
  sp1_send_char(0);
  sp1_send_char(0);
  sp1_send_char(c);
}

void sp1_send_int(int v){
  sp1_send_char((uint8_t)(v >>24));
  sp1_send_char((uint8_t)(v >>16));
  sp1_send_char((uint8_t)(v >> 8));
  sp1_send_char((uint8_t)(v & 0x000000FF));
}

bool sp1_receive_char(uint8_t *c){
  uint32_t lcr;
  sys_inb(COM1 + LCR , &lcr);
  if(lcr & DLAB){
    sys_outb(COM1 + LCR , lcr & 0x7F);
    return false;
  }
  uint32_t lsr;
  sys_inb(COM1 + LSR , &lsr);
  if(lsr & (OVERRUN_ERR | PARITY_ERR | FRAME_ERR))
    return false;
  if(lsr & DATA){
    uint32_t ch = 0;
    sys_inb(COM1 + RBR, &ch);
    *c = (uint8_t) ch;
    return true;
  }
    return false;
}
