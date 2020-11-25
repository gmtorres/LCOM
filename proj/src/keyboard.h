#pragma once

#include <stdint.h>

int (keyboard_subscribe_int)(uint8_t *bit_no);


int (keyboard_unsubscribe_int)();

//int kbd_scan_C(uint8_t bit_no);

//int kbd_poll();

void (kbd_enable_kbd_interrupts)();

int (kbd_get_status)(uint8_t * st);

int (check_bytes)(uint8_t val , uint8_t check);

int (kbd_read_outbuf)(uint8_t * out);

int (check_outbuf) (void);

//extern int kbd_scan_ASM(uint8_t &bit_no);
