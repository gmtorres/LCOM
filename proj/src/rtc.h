#pragma once

#include <lcom/lcf.h>

#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A  // 10
#define RTC_REG_B 0x0B // 11
#define RTC_REG_C 0x0C // 12

#define RTC_UIP 0x80

int rtc_subscribe_int(uint8_t * bit_no);

int rtc_unsubscribe_int();

void read_rtc_time(uint8_t time[6]);

void write_rtc(uint8_t reg, uint8_t v);

void read_rtc(uint8_t reg, uint8_t *v);

void rtc_enable_int();

void rtc_disable_int();
