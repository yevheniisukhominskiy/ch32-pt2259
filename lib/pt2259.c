#include "pt2259.h"
#include "time.h"

#define PT2259_ADDR  0b1000100

#define CLEAR     0xF0
#define MUTE_OFF  0x74
#define MUTE_ON   0x77
#define MUTE_L    0x76
#define MUTE_R    0x75

#define BOTH_1    0b11010000
#define BOTH_10   0b11100000
#define LEFT_10   0b10110000
#define LEFT_1    0b10100000
#define RIGHT_10  0b00110000
#define RIGHT_1   0b00100000

static void pt2259_send(PT2259_t* pt2259, uint8_t* cmds, uint8_t count) {
    #if SOFT_I2C_ON == 1
        softi2c_start(pt2259->i2c);
        if (softi2c_writebyte(pt2259->i2c, PT2259_ADDR << 1)) {
            for (uint8_t i = 0; i < count; i++) {
                softi2c_writebyte(pt2259->i2c, cmds[i]);
            }
        }
        softi2c_stop(pt2259->i2c);
    #else
        while( I2C_GetFlagStatus(pt2259->i2c, I2C_FLAG_BUSY ) != RESET );

        I2C_GenerateSTART(pt2259->i2c, ENABLE );
        while(!I2C_CheckEvent(pt2259->i2c, I2C_EVENT_MASTER_MODE_SELECT));

        I2C_Send7bitAddress(pt2259->i2c, (PT2259_ADDR << 1), I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(pt2259->i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

        for(uint8_t i = 0; i < count; i++) {
            if(I2C_GetFlagStatus(pt2259->i2c, I2C_FLAG_TXE) != RESET) {
                I2C_SendData(I2C1, cmds[i]);
            }
        }

        while(!I2C_CheckEvent(pt2259->i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        I2C_GenerateSTOP(pt2259->i2c, ENABLE);
    #endif

    
    delay_ms(1);
}

void pt2259_setvolume_both(PT2259_t* pt2259, bool mute, uint8_t vol_percent) {
    if (vol_percent > 100) vol_percent = 100;

    uint8_t vol_chip = (uint8_t)(79 - (79 * vol_percent / 100));

    uint8_t data[3];
    data[0] = mute ? MUTE_ON : MUTE_OFF;   // Стан Mute
    data[1] = BOTH_10 + (vol_chip / 10);   // Десятки дБ
    data[2] = BOTH_1 + (vol_chip % 10);    // Одиниці дБ

    pt2259_send(pt2259, data, 3);
}

void pt2259_setvolume_left(PT2259_t* pt2259, bool mute, uint8_t vol_percent) {
    if (vol_percent > 100) vol_percent = 100;

    uint8_t vol_chip = (uint8_t)(79 - (79 * vol_percent / 100));

    uint8_t data[3];
    data[0] = mute ? MUTE_L : MUTE_OFF;     // Стан Mute
    data[1] = LEFT_10 + (vol_chip / 10);    // Десятки дБ
    data[2] = LEFT_1 + (vol_chip % 10);     // Одиниці дБ

    pt2259_send(pt2259, data, 3);
}

void pt2259_setvolume_right(PT2259_t* pt2259, bool mute, uint8_t vol_percent) {
    if (vol_percent > 100) vol_percent = 100;

    uint8_t vol_chip = (uint8_t)(79 - (79 * vol_percent / 100));

    uint8_t data[3];
    data[0] = mute ? MUTE_R : MUTE_OFF;     // Стан Mute
    data[1] = RIGHT_10 + (vol_chip / 10);   // Десятки дБ
    data[2] = RIGHT_1 + (vol_chip % 10);    // Одиниці дБ

    pt2259_send(pt2259, data, 3);
}

void pt2259_clear(PT2259_t* pt2259) {
    uint8_t cmd = CLEAR;
    pt2259_send(pt2259, &cmd, 1);
}