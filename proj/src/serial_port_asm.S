#include <machine/asm.h>

#define BIT(n) (0x01<<(n))
#define COM1 0x3F8
#define COM2 0x2F8
#define RBR 0
#define THR 0
#define IER 1
#define IIR 2
#define FCR 2
#define LCR 3
#define MCR 4
#define LSR 5
#define MST 6
#define SR 7
#define DATA BIT(0)
#define OVERRUN_ERR BIT(1)
#define PARITY_ERR BIT(2)
#define FRAME_ERR BIT(3)
#define BREAK_INTERRUPT BIT(4)
#define TX_RDY BIT(5)
#define TX_HOLDING_EMPTY BIT(5)
#define TX_EMPTY BIT(6)
#define FIFO_ERR BIT(7)

.intel_syntax

.extern sp1_read:
  .byte 0


.globl sp1_read_char

_C_LABEL(sp1_read_char):
xor eax, eax
;inb COM1 + LSR
;test eax,OVERRUN_ERR | FRAME_ERR | PARITY_ERR
;jnz fim
;test eax, DATA
;jz fim
;inb COM1 + RBR
;mov _C_LABEL(sp1_read),al
fim: ret
