#pragma once
#include <avr/io.h>

#define BUMPER_dir  DDRC
#define BUMPER_pin  PINC
#define FRONT_BUMPER_bit  PC4
#define BACK_BUMPER_bit   PC5

extern volatile _Bool front_snag;
extern volatile _Bool back_snag;
extern uint8_t current_dir;
void bumper_init(void);
void bumper_check_way(void);
