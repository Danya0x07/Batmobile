#include "movement.h"

static const uint8_t directions[NUM_DIRECTIONS] = {
    0b00000011,
    _BV(LM_1) | _BV(RM_1),  // вперёд
    _BV(LM_2) | _BV(RM_2),  // назад
    _BV(LM_2) | _BV(RM_1),  // влево
    _BV(LM_1) | _BV(RM_2),  // вправо
};

static MotorSide lside = {&LM_COUNTER, LM_1, LM_2, 0, 0};
static MotorSide rside = {&RM_COUNTER, RM_1, RM_2, 0, 0};

void motors_init(void)
{
    MOT_dir |= 0b11111100;
    MOT_port = 0;
    TCCR0A = _BV(COM0A1) | _BV(COM0B1)
            | _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS01) | _BV(CS00);
    OCR0A = OCR0B = 0;
}

void set_dir(uint8_t dir)
{
    set_spd(0, 0);
    MOT_port &= directions[0];
    MOT_port |= directions[dir];
}

void set_spd(uint8_t L, uint8_t R)
{
    LM_COUNTER = L;
    RM_COUNTER = R;
}

void check_motorside(MotorSide* mside, int8_t fade)
{
    mside->spd_buf += fade;
    if(abs(mside->spd_buf) < MIN_SPEED) {
        *mside->m_ctr = 0;
    } else if(abs(mside->spd_buf) >= 255) {
        *mside->m_ctr = 255;
        mside->spd_buf = mside->spd_buf > 0 ? 255 : -255;
    } else {
        if(mside->spd_buf >= 0) {
            if(mside->dir == BACKWARD) {
                mside->dir = FORWARD;
                MOT_port &= ~_BV(mside->mbit_2);
                MOT_port |= ~_BV(mside->mbit_1);
            }
            *mside->m_ctr = mside->spd_buf;
        } else {
            if(mside->dir == FORWARD) {
                mside->dir = BACKWARD;
                MOT_port &= ~_BV(mside->mbit_1);
                MOT_port |= ~_BV(mside->mbit_2);
            }
            *mside->m_ctr = -mside->spd_buf;
        }
    }
}

void handle_cruise(int8_t fade_L, int8_t fade_R)
{
    static uint16_t spd_update_timer = 0;
    if(spd_update_timer >= SPD_UPDATE_TIME) {
        check_motorside(&lside, fade_L);
        check_motorside(&rside, fade_R);
        spd_update_timer = 0;
    } else spd_update_timer++;
}
