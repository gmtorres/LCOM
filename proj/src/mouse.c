
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "mouse.h"
#include "keyboard.h"
#include "i8042.h"

#include <stdint.h>
//#include "lab4.c"

#define MOUSE_IRQ  12

int mouse_hook_id = 0x0C;


int (mouse_subscribe_int) (uint8_t *bit_no){
  *bit_no = mouse_hook_id;
  int r;
  if((r =sys_irqsetpolicy( MOUSE_IRQ , IRQ_REENABLE|IRQ_EXCLUSIVE , &mouse_hook_id)) != OK){
    return r;
  }
  //if((r = sys_irqenable(&kbd_hook_id)) != OK)
  //  return r;
  //*bit_no = temp_hook_id;
  return 0;

}

int (mouse_unsubscribe_int)(){
  int r;
    if ((r = sys_irqdisable(&mouse_hook_id)) != OK)
      return r;
    if((r = sys_irqrmpolicy(&mouse_hook_id)) != OK)
      return r;
    return 0;
}

int mouse_wait_for_ibf(){

	uint8_t stat;
	while(1)
	{
  //  printf("a");
		kbd_get_status(&stat); /*assuming it returns OK*/
		/*loop while 8042 input buffer is not empty*/
		if (check_bytes(stat,BIT(1)) == 0)
		{
			//sys_outb(STAT_REG, MC);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int mouse_write_command(uint8_t cmd){
  mouse_wait_for_ibf();
  //printf("b");
  sys_outb(KBD_CNTRL_REG,MOUSE_WRITE);
  mouse_wait_for_ibf();
  //printf("c");
  sys_outb(KBD_OUT_BUF,cmd);
  uint32_t ack;
  while(1){
    //printf("while");
    tickdelay(micros_to_ticks(DELAY_US));
    mouse_wait_for_ibf();

    while(sys_inb(KBD_OUT_BUF,&ack)!=0){}

    uint8_t ack_8 = (uint8_t) ack;
    //printf("%x with command %x\n", ack_8 , cmd );
    if(ack_8 == ACK){
      return 0;
    }else if(ack_8 == NACK){
      continue;
    }else if (ack_8 == ERROR){
      return -2;
    }



  }

  return 0;
}

int mouse_disable_data_rep(){
  return mouse_write_command(DSBL_DATA);
}
int mouse_enable_data_rep(){
  return mouse_write_command(ENBL_DATA);
}
int mouse_set_remote(){
  return mouse_write_command(SET_RMT);
}
int mouse_set_stream_mode(){
  return mouse_write_command(SET_STREAM);
}
int mouse_request_packet(){
  return mouse_write_command(READ_DATA);
}




int mouse_enbl_int(){
    mouse_set_stream_mode();
    mouse_enable_data_rep();
    return 0;
}
int mouse_dsbl_int(){
  return mouse_disable_data_rep() & mouse_set_stream_mode();
}
