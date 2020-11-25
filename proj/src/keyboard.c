#include "keyboard.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
extern uint32_t count_sys_inb;


#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

int kbd_hook_id = 0x01;

int (keyboard_subscribe_int) (uint8_t *bit_no){
  int temp_hook_id = kbd_hook_id;
  int r;
  if((r = sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbd_hook_id)) != OK){
    return r;
  }
  //if((r = sys_irqenable(&kbd_hook_id)) != OK)
  //  return r;
  *bit_no = temp_hook_id;
  return 0;

}


int (keyboard_unsubscribe_int)(){
  int r;
    if ((r = sys_irqdisable(&kbd_hook_id)) != OK)
      return r;
    if((r = sys_irqrmpolicy(&kbd_hook_id)) != OK)
      return r;
    return 0;
}

int (kbd_get_status)(uint8_t * st){
  int r;
  uint32_t temp;
  if((r = sys_inb_cnt(KBD_STAT_REG, &temp)) != 0){
    return r;
  }
  *st = temp;
  return 0;
}
int (check_bytes)(uint8_t val , uint8_t check){
   return ((val & check) == check);
}

void (kbd_enable_kbd_interrupts)(){
  uint32_t b;
  sys_outb(KBD_STAT_REG,READ_CMD);
  sys_inb_cnt(KBD_OUT_BUF , &b);
  uint8_t byte_command = ((uint8_t)b | BIT(0));
  sys_outb(KBD_STAT_REG,WRITE_CMD);
  sys_outb(KBD_OUT_BUF , byte_command);
}

int (kbd_read_outbuf)(uint8_t * out) {
  int r;
  uint32_t temp;
  r = sys_inb_cnt(KBD_OUT_BUF, &temp);
  if (r != 0) {
    return r;
  }
  *out = (uint8_t) temp;
  return 0;
}
int (check_outbuf) (void){
  uint8_t status;
  kbd_get_status(&status);
  return check_bytes(status,CHECK_OUT_BUF);
}
