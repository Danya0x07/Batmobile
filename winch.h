#pragma once

#define WINCH_dir   DDRB
#define WINCH_port  PORTB
#define WM_1  PB1
#define WM_2  PB2

void winch_init(void);
void winch_pull(void);
void winch_drop(void);
void winch_stop(void);
