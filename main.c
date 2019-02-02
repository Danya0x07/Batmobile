#include <avr/interrupt.h>
#include "movement.h"
#include "uart.h"
#include "winch.h"
#include "shield.h"
#include "bumper.h"
#include "led.h"

enum Commands {
    SET_SPEED,
    SET_DIRECTION,
    WINCH_PULL,
    WINCH_DROP,
    WINCH_STOP,
    TARAN_ON,
    TARAN_OFF,
    CRUISE_ON,
    CRUISE_OFF,
    SET_FADE
};

uint8_t current_dir = 0;
uint8_t spd_L = 0, spd_R = 0;
uint8_t command;
int8_t spd_fade_L = 0, spd_fade_R = 0;
_Bool cruise_en = 0; 

int main(void)
{
    motors_init();
    winch_init();
    shield_init();
    led_init();
    UART_init(103);
    bumper_init();
    sei();

    while(1)
    {
        if(new_command)
        {
            command = UART_getc();
            switch(command)
            {
                case SET_SPEED:
                    spd_L = UART_getc();
                    spd_R = UART_getc();
                    set_spd(spd_L, spd_R);
                    break;
                    
                case SET_DIRECTION:
                    current_dir = UART_getc();
                    set_dir(current_dir);
                    break;
                    
                case WINCH_PULL: winch_pull(); break;
                case WINCH_DROP: winch_drop(); break;
                case WINCH_STOP: winch_stop(); break;
                    
                case TARAN_ON:
                    t2_nointerrupts();
                    t2_procedure = led_blink;
                    t2_interrupts();
                    headlight_off();
                    interlight_on();
                    shield_up();
                    break;
                    
                case TARAN_OFF:
                    t2_nointerrupts();
                    t2_procedure = 0;
                    shield_down();
                    rearlight_off();
                    headlight_on();
                    interlight_on();
                    break;

                case CRUISE_ON:
                    t2_nointerrupts();
                    t2_procedure = bumper_check_way;
                    t2_interrupts();
                    cruise_en = 1;
                    interlight_off();
                    headlight_on();
                    rearlight_on();
                    break;

                case CRUISE_OFF:
                    t2_nointerrupts();
                    t2_procedure = 0;
                    cruise_en = 0;
                    rearlight_off();
                    headlight_on();
                    interlight_on();
                    break;

                case SET_FADE:
                    spd_fade_L = UART_getc();
                    spd_fade_R = UART_getc();
                    break;
            }
        }
        if(cruise_en) {
            handle_cruise(spd_fade_L, spd_fade_R);
        }
    }
}

