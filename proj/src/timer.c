/*************************************************************************
*
*
*   Code developed by tgusta and jokinho
*
*
***************************************************************************/

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include <minix/syslib.h>

#include "i8254.h"

uint32_t global_counter = 0;
int tmr_hook_id = 0x00;

int (timer_set_frequency) (uint8_t timer, uint32_t freq) {
  //timer out of range
  if (timer > 2)
    return -1;
  //invalid frequency
  if(freq == 0)
    return -2;
  uint8_t st;
  int result = timer_get_conf(timer,&st);
  //check whether get_config went wrong
  if(result != 0)
    return result;
  uint8_t mask = 0x0F; //00001111
  st = (mask & st) | TIMER_LSB_MSB;
  switch (timer) {
    case 0 :st = st | TIMER_SEL0;
           break;
    case 1 :st = st | TIMER_SEL1;
          break;
    case 2:st = st | TIMER_SEL2;
          break;
        }
  // send config to the control register
  result = sys_outb(TIMER_CTRL,st);
  if (result != 0) {
    return result;
  }
  //test if the division value can be stored in a 16 bits register
  uint32_t division_test = TIMER_FREQ / freq;
  if((division_test>>16) != 0){
    printf("Frequency is too small to calculate a division for the 16 bits of the timer.\n" );
    return -2;
  }else if(division_test == 0){
    printf("Frequency is too big to calculate a proper division for the timer.\n" );
    return -3;
  }
  uint16_t division = (uint16_t)division_test;
  //send the division lsb to the timer
  util_get_LSB(division,&st);
  result = sys_outb(TIMER_0 + timer,st);
  if (result != 0) {
    return result;
  }
  //send the division msb to the timer
  util_get_MSB(division,&st);
  result = sys_outb(TIMER_0 + timer,st);
  if (result != 0) {
    return result;
  }
  return 0; //it worked
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  int hook_id_temp = tmr_hook_id;
  int r;
  if((r = sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE ,&tmr_hook_id)) != OK){
    return r;
  }
//  if((r = sys_irqenable(&tmr_hook_id)) != OK)
  //  return r;
  *bit_no = hook_id_temp;
  return 0; //everything went well
}

int (timer_unsubscribe_int)() {

  int r;
  //if ((r = sys_irqdisable(&tmr_hook_id)) != OK)
  //  return r;
  if((r = sys_irqrmpolicy(&tmr_hook_id)) != OK)
    return r;

  return 0; //everything went well
}

void (timer_int_handler)() {
  global_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if(timer > 2)
    return -1;

    uint8_t word = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    uint32_t received = sys_outb(TIMER_CTRL, word); //send to the Controller(ReadBack Command , counter because is active low , and select the timer)
    if(received != 0){
        return received;  // return a number different than zero so something went wrong
    }

    uint32_t stored_value;
    received = sys_inb(TIMER_0 + timer, &stored_value);
    if(received != 0){
        return received;  // return a number different than zero, so something went wrong
    }
    *st = (uint8_t)stored_value;

  return 0; // went well
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  if(timer > 2)
    return -1;

  union timer_status_field_val val;
  if(field == 0){ /* Display status byte, in hexadecimal */
      val.byte = st;
  }
  else if(field == 1){ /* Display the initialization mode, only */
    uint8_t mask = 0x30; // puts every bit to 0 except 4th and 5th bits
    st = st & mask;
    if(st == TIMER_LSB ){
      val.in_mode = LSB_only;
    }else if(st == TIMER_MSB ){
      val.in_mode = MSB_only;
    }else if(st == TIMER_LSB_MSB ){
      val.in_mode = MSB_after_LSB;
    }else{
      val.in_mode = INVAL_val;
    }
  }
  else if(field == 2){ /* The counting mode: 0, 1,.., 5 */
    uint8_t mask = 0x0E; // puts every bit to 0 expect 1st,2nd,3rd bits
    st = st & mask;
    st = st >> 1;
    if(st == (BIT(2)|BIT(1)))
      val.count_mode = 2;
    else if(st == (BIT(2)|BIT(1)|BIT(0)))
      val.count_mode = 3;
    else
      val.count_mode = st;
  }else if(field == 3){ /* The counting base, true if BCD */
    uint8_t mask = 0x01; // puts every bit to 0 exepct the lsb
    st = st & mask;
    if(st)
      val.bcd = true;
    else
      val.bcd = false;
  }else
    return 1; //error, unknown field
  int print = timer_print_config(timer, field , val);
  if(print !=0){
    return print;
  }
  return 0; //went well
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)val;
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val>>8);
  return 1;
}
