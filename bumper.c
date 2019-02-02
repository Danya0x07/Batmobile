#include "bumper.h"
#include "movement.h"
#include <avr/io.h>

volatile _Bool front_snag;
volatile _Bool back_snag;

void bumper_init(void)
{
    BUMPER_dir &= ~(_BV(FRONT_BUMPER_bit) |
                    _BV(BACK_BUMPER_bit));
}

void bumper_check_way(void)
{
    front_snag = BUMPER_pin & _BV(FRONT_BUMPER_bit) == 0;
    back_snag = BUMPER_pin & _BV(BACK_BUMPER_bit) == 0;
    if(front_snag && current_dir == FORWARD) {
        set_spd(0, 0);
    }
    if(back_snag && current_dir == BACKWARD) {
        set_spd(0, 0);
    }
}
