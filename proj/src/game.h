#pragma once

#include <lcom/lcf.h>



#include "car.h"
#include "tile.h"
#include "keyboard.h"
#include <lcom/timer.h>
#include "i8042.h"
#include "i8254.h"
#include "v_gr.h"
#include <lcom/read_xpm.h>
#include "powerup.h"
#include "numbers.h"
#include "pista.h"
#include "MouseD.h"
#include "mouse.h"

#include "rtc.h"
#include "serial_port.h"


#define PI 3.1415926


void sp1_read_char();
void game();
