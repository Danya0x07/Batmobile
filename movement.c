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
    LM_COUNTER = RM_COUNTER = 0;
}

void set_dir(uint8_t dir)
{
    LM_COUNTER = RM_COUNTER = 0;
    MOT_port &= directions[0];
    MOT_port |= directions[dir];
}

void set_spd(uint8_t L, uint8_t R)
{
    if(L < LM_COUNTER) LM_COUNTER = L;
    else lside.spd_buf = L;
    if(R < RM_COUNTER) RM_COUNTER = R;
    else rside.spd_buf = R;
}

void check_motorside(MotorSide* mside, int8_t fade)
{
    mside->spd_buf += fade;
    if(abs(mside->spd_buf) < MIN_SPEED) {
        *mside->m_ctr = 0;
        mside->dir = NONE;
    } else if(abs(mside->spd_buf) >= 255) {
        *mside->m_ctr = 255;
        mside->spd_buf = (mside->spd_buf > 0) ? 255 : -255;
    } else {
        if(mside->spd_buf >= 0) {
            if(mside->dir != FORWARD) {
                mside->dir = FORWARD;
                MOT_port &= ~_BV(mside->mbit_2);
                MOT_port |= _BV(mside->mbit_1);
            }
            *mside->m_ctr = mside->spd_buf;
        } else {
            if(mside->dir != BACKWARD) {
                mside->dir = BACKWARD;
                MOT_port &= ~_BV(mside->mbit_1);
                MOT_port |= _BV(mside->mbit_2);
            }
            *mside->m_ctr = -mside->spd_buf;
        }
    }
}

void cruise_handle(int8_t fade_L, int8_t fade_R)
{
    static uint32_t spd_update_timer = 0;
    if(spd_update_timer >= SPD_UPDATE_TIME) {
        check_motorside(&lside, fade_L);
        check_motorside(&rside, fade_R);
        spd_update_timer = 0;
    } else spd_update_timer++;
}

uint8_t get_current_direction(void)
{
    for(uint8_t i = 1; i < NUM_DIRECTIONS; i++)
        if(MOT_port & directions[i])
            return i;
    return 0;
}

void cruise_stop_motors(void)
{
    lside.spd_buf = rside.spd_buf = 0;
    lside.dir = rside.dir = NONE;
    LM_COUNTER = RM_COUNTER = 0;
}

static void smoothly_accelerate(MotorSide* mside)
{
    if(*mside->m_ctr < mside->spd_buf)
        (*mside->m_ctr)++;
}

void acceleration_handle(uint8_t current_dir)
{
    static uint32_t accel_timer = 0;
    if(current_dir != NONE && accel_timer >= ACCELERATION_DELAY) {
        smoothly_accelerate(&lside);
        smoothly_accelerate(&rside);
        accel_timer = 0;
    } else accel_timer++;
}
