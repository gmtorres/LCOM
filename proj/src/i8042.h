#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042
 * @{
 *
 * Constants for programming the i8042 keyboard. Needs to be completed.
 */

#define BIT(n) (0x01<<(n))

#define KBD_IRQ    1

#define DELAY_US    20000

#define CHECK_MSB 0x80
#define CHECK_RCV_ERROR 0x40
#define CHECK_OUT_BUF 0x01

#define KBD_OUT_BUF 0x60
#define KBD_IN_BUF 0x60
#define KBD_STAT_REG 0x64
#define KBD_CNTRL_REG 0x64

#define READ_CMD 0x20
#define WRITE_CMD 0x60

#define DESBL_INTRPS 0xAD
#define ENBl_INTRPS 0xAE
#define KBD_INTRP BIT(0)

#define ESC_KEY 0x81
#define SET2_SCANCODE 0xE0


#define DSBL_MOUSE 0xA7
#define ENBL_MOUSE 0xA8
#define CHECK_MOUSE 0xA9
#define MOUSE_WRITE 0xD4


#define MOUSE_RESET 0xFF
#define DSBL_DATA 0xF5
#define ENBL_DATA 0xF4
#define SET_RMT 0xF0
#define READ_DATA 0xEB
#define SET_STREAM 0xEA
#define STATUS_REQ 0xE9


#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC



/**@}*/

#endif /* _LCOM_I8254_H */
