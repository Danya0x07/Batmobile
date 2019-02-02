#pragma once
#include <avr/io.h>

#define SERVO_dir   DDRC
#define SERVO_port  PORTC
#define SERVO_bit   PC3
#define SERVO_PERIOD  4999

#define shield_up()     OCR1A = 250
#define shield_down()   OCR1A = 500

void shield_init(void);
