#pragma once
#include <avr/io.h>

#define MOT_dir     DDRD
#define MOT_port    PORTD

#define LM_1   PD7
#define LM_2   PD4
#define LM_COUNTER  OCR0B

#define RM_1   PD2
#define RM_2   PD3
#define RM_COUNTER  OCR0A

#define NUM_DIRECTIONS  5
#define SPD_UPDATE_TIME 6000
#define MIN_SPEED   50
#define ACCELERATION_DELAY  200
//#define ACCELERATION_FADE   2

#define abs(x)  (((x) >= 0) ? (x) : (-(x)))

enum Directions
{
    NONE,
    FORWARD,
    BACKWARD,
    LEFTWARD,
    RIGHTWARD
};

typedef struct {
    volatile uint8_t* m_ctr;
    uint8_t mbit_1;
    uint8_t mbit_2;
    int16_t spd_buf;
    _Bool dir;
} MotorSide;

void motors_init(void);
void set_dir(uint8_t);
void set_spd(uint8_t, uint8_t);
void check_motorside(MotorSide*, int8_t);
void cruise_handle(int8_t, int8_t);
void cruise_stop_motors(void);
void acceleration_handle(uint8_t);
uint8_t get_current_direction(void);

