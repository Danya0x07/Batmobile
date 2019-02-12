#pragma once
#include <avr/io.h>

#define BUMPER_dir  DDRC
#define BUMPER_port PORTC
#define BUMPER_pin  PINC
#define FRONT_BUMPER_bit  PC4
#define BACK_BUMPER_bit   PC5

void bumper_init(void);
_Bool bumper_check_way(uint8_t);
