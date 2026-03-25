#ifndef PT2259_H
#define PT2259_H

#include "stdbool.h"

#define SOFT_I2C_ON  0

#if SOFT_I2C_ON == 0
    #include "ch32v00X_i2c.h"
    typedef struct {
        I2C_TypeDef* i2c;
    } PT2259_t;
#else
    #include "soft_i2c.h"
    typedef struct {
        SoftI2_t* i2c;
    } PT2259_t;
#endif

void pt2259_setvolume_both(PT2259_t* pt2259, bool mute, uint8_t vol_percent);
void pt2259_setvolume_left(PT2259_t* pt2259, bool mute, uint8_t vol_percent);
void pt2259_setvolume_right(PT2259_t* pt2259, bool mute, uint8_t vol_percent);
void pt2259_clear(PT2259_t* pt2259);

#endif