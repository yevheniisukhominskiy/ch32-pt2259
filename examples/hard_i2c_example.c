#include "debug.h"
#include "time.h"
#include "pt2259.h"

PT2259_t pt = {.i2c = I2C1};

void i2c_init() {
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};

    // Note: ensure RCC_APB2Periph and RCC_APB1Periph are used correctly depending on your MCU device headers.
    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOC | RCC_PB2Periph_AFIO, ENABLE );
    RCC_PB1PeriphClockCmd( RCC_PB1Periph_I2C1, ENABLE );

    // SCL (PC2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    // SDA (PC1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = 80000;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = 0x00;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );

    I2C_Cmd( I2C1, ENABLE );
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    SDI_Printf_Enable();

    systick_init();
    i2c_init();

    pt2259_clear(&pt);
    pt2259_setvolume_both(&pt, false, 25);
    
    while(1)
    {
        static uint32_t timer = 0;
        static uint8_t vol = 25;

        // Кожні 4 секунди міняємо гучність
        if (millis() - timer >= 4000) {
            timer = millis();
            
            vol += 25;
            if (vol > 100) vol = 25;

            pt2259_setvolume_both(&pt, false, vol);
        }
    }
}
