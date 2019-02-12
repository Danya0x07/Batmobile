#pragma once

#define LED_dir     DDRC
#define LED_port    PORTC

#define HEADLIGHTS  PC0
#define REARLIGHTS  PC2
#define INTERRIOR   PC1

#define headlight_on()      LED_port |= _BV(HEADLIGHTS)
#define headlight_off()     LED_port &= ~_BV(HEADLIGHTS)
#define headlight_toggle()  LED_port ^= _BV(HEADLIGHTS)
#define rearlight_on()      LED_port |= _BV(REARLIGHTS)
#define rearlight_off()     LED_port &= ~_BV(REARLIGHTS)
#define rearlight_toggle()  LED_port ^= _BV(REARLIGHTS)
#define interlight_on()     LED_port |= _BV(INTERRIOR)
#define interlight_off()    LED_port &= ~_BV(INTERRIOR)
#define interlight_toggle() LED_port ^= _BV(INTERRIOR)

#define BLINK_DELAY 2
#define t2_interrupts()    TIMSK2 |= _BV(TOIE2)
#define t2_nointerrupts()  TIMSK2 &= ~_BV(TOIE2)

typedef void(*funcptr_t)(void);
extern volatile funcptr_t t2_procedure;
void led_init(void);
void led_blink(void);
