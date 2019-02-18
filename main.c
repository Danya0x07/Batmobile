#include <avr/interrupt.h>
#include "movement.h"
#include "uart.h"
#include "winch.h"
#include "shield.h"
#include "bumper.h"
#include "led.h"

enum Commands {
    SET_SPEED,      // 0
    SET_DIRECTION,  // 1
    WINCH_PULL,     // 2
    WINCH_DROP,     // 3
    WINCH_STOP,     // 4
    TARAN_ON,       // 5
    CRUISE_ON,      // 6
    NORMAL_MODE,    // 7
    SET_FADE        // 8
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
    UART_init(8);
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
                    shield_enable();
                    break;
                    

                case CRUISE_ON:
                    t2_nointerrupts();
                    cruise_en = 1;
                    cruise_stop_motors();
                    interlight_off();
                    headlight_on();
                    rearlight_on();
                    break;

                case NORMAL_MODE:
                    t2_nointerrupts();
                    t2_procedure = 0;
                    shield_disable();
                    cruise_en = 0;
                    spd_fade_L = spd_fade_R = 0;
                    cruise_stop_motors();
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

        current_dir = get_current_direction();
        if(cruise_en) {
            cruise_handle(spd_fade_L, spd_fade_R);
            if(bumper_check_way(current_dir)) {
                spd_fade_L = spd_fade_R = 0;
                cruise_stop_motors();
            }
        } else {
            acceleration_handle(current_dir);
        }
    }
}

