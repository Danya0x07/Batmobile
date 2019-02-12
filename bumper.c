#include "bumper.h"
#include "movement.h"

static _Bool front_snag;
static _Bool back_snag;

void bumper_init(void)
{
    BUMPER_dir &= ~(_BV(FRONT_BUMPER_bit) |
                    _BV(BACK_BUMPER_bit));
    BUMPER_port |= _BV(FRONT_BUMPER_bit) |
                   _BV(BACK_BUMPER_bit);
}

_Bool bumper_check_way(uint8_t current_dir)
{
    front_snag = (BUMPER_pin & _BV(FRONT_BUMPER_bit)) == 0;
    back_snag = (BUMPER_pin & _BV(BACK_BUMPER_bit)) == 0;
    if(front_snag && current_dir == FORWARD) {
        return 1;
    }
    if(back_snag && current_dir == BACKWARD) {
        return 1;
    }
    return 0;
}
