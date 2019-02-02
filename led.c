#include "led.h"
#include <avr/interrupt.h>

volatile funcptr_t t2_procedure;
static volatile uint8_t counter = 0;

void led_init(void)
{
    LED_dir |= _BV(HEADLIGHTS)
            | _BV(REARLIGHTS)
            | _BV(INTERRIOR);
    LED_port |= _BV(HEADLIGHTS) | _BV(INTERRIOR);
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
    t2_nointerrupts();
}

void led_blink(void)
{
    if(!(counter % BLINK_DELAY))
        rearlight_toggle();
    counter++;
}

ISR(TIMER2_OVF_vect)
{
    if(t2_procedure) t2_procedure();
}
