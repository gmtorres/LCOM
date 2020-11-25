#include "rtc.h"

int rtc_hook_id = 0x08;

int rtc_subscribe_int(uint8_t * bit_no){
  *bit_no = rtc_hook_id;
  int r;
  if((r =sys_irqsetpolicy( RTC_IRQ , IRQ_REENABLE , &rtc_hook_id)) != OK){
    return r;
  }
  return 0;

}

int rtc_unsubscribe_int(){
  int r;
    if ((r = sys_irqdisable(&rtc_hook_id)) != OK)
      return r;
    if((r = sys_irqrmpolicy(&rtc_hook_id)) != OK)
      return r;
    return 0;
}

void bcd_to_bn(uint8_t *x){
  uint8_t t = (uint8_t)*x;
  t = (t >> 4) * 10 + (t & 0x0F);
  *x = (uint32_t) t;
}

void rtc_disable_int(){
  uint32_t regB;
  sys_outb(RTC_ADDR_REG , RTC_REG_B);
  sys_inb(RTC_DATA_REG , &regB);
  regB = regB & 0xEF;
  regB = regB | 0x80;
  sys_outb(RTC_ADDR_REG , RTC_REG_B);
  sys_outb(RTC_DATA_REG , regB);
}

void rtc_enable_int(){
  uint32_t regB;
  sys_outb(RTC_ADDR_REG , RTC_REG_B);
  sys_inb(RTC_DATA_REG , &regB);
  regB = regB | 0x10;
  regB = regB & 0x7F;
  sys_outb(RTC_ADDR_REG , RTC_REG_B);
  sys_outb(RTC_DATA_REG , regB);
}

void wait_valid_rtc(void) {
  uint32_t regA = 0;
  do {
    sys_outb(RTC_ADDR_REG, RTC_REG_A);
    sys_inb(RTC_DATA_REG, &regA);
  } while ( regA & RTC_UIP);
}

void read_rtc_time(uint8_t date[6]){
  wait_valid_rtc();

  uint32_t t;
  sys_outb(RTC_ADDR_REG, 0);
  sys_inb(RTC_DATA_REG, &t);
  date[0] = t;

  sys_outb(RTC_ADDR_REG, 2);
  sys_inb(RTC_DATA_REG, &t);
  date[1] = t;

  sys_outb(RTC_ADDR_REG, 4);
  sys_inb(RTC_DATA_REG, &t);
  date[2] = t;

  sys_outb(RTC_ADDR_REG, 7);
  sys_inb(RTC_DATA_REG, &t);
  date[3] = t;

  sys_outb(RTC_ADDR_REG, 8);
  sys_inb(RTC_DATA_REG, &t);
  date[4] = t;

  sys_outb(RTC_ADDR_REG, 9);
  sys_inb(RTC_DATA_REG, &t);
  date[5] = t;

  uint32_t bcd;
  sys_outb(RTC_ADDR_REG , RTC_REG_B);
  sys_inb(RTC_DATA_REG , &bcd);


  if(!(bcd & 0x04)){
    for(int i = 0; i <6;i++)
      bcd_to_bn(&date[i]);
  }

}

void write_rtc(uint8_t reg, uint8_t v){
  sys_outb(RTC_ADDR_REG , reg);
  sys_outb(RTC_DATA_REG , v);
}


void read_rtc(uint8_t reg, uint8_t *v){
    wait_valid_rtc();
    sys_outb(RTC_ADDR_REG , reg);
    uint32_t temp = 0;
    sys_outb(RTC_DATA_REG , temp);
    *v = temp;
}
