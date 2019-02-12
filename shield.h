#pragma once
#include <avr/io.h>

#define SERVO_dir   DDRC
#define SERVO_port  PORTC
#define SERVO_bit   PC3
#define SERVO_PERIOD  625

#define shield_enable()     OCR1A = 20
#define shield_disable()   OCR1A = 55

void shield_init(void);
