#include "winch.h"
#include <avr/io.h>

void winch_init(void)
{
    WINCH_dir |= _BV(WM_1) | _BV(WM_2);
    WINCH_port = 0;
}

void winch_pull(void)
{
    WINCH_port &= ~_BV(WM_2);
    WINCH_port |= _BV(WM_1);
}

void winch_drop(void)
{
    WINCH_port &= ~_BV(WM_1);
    WINCH_port |= _BV(WM_2);
}

void winch_stop(void)
{
    WINCH_port &= ~_BV(WM_1) & ~_BV(WM_2);
}
