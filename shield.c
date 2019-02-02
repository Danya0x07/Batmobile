#include "shield.h"
#include <avr/interrupt.h>

void shield_init(void)
{
    SERVO_dir |= _BV(SERVO_bit);
    SERVO_port = 0;
    TCCR1A = _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12)
            | _BV(CS11) | _BV(CS10);
    TIMSK1 = _BV(OCIE1A) | _BV(TOIE1);
    ICR1 = SERVO_PERIOD;
    shield_down();
}

ISR(TIMER1_COMPA_vect)
{
    SERVO_port &= ~_BV(SERVO_bit);
}

ISR(TIMER1_OVF_vect)
{
    SERVO_port |= _BV(SERVO_bit);
}
