#include "debug.h"
#include "time.h"
#include "soft_i2c.h"
#include "pt2259.h" // Тут SOFT_I2C_ON має бути 1

// Налаштовуємо твої піни для ногодрыга
SoftI2_t i2c1 = {
    .sda_port = GPIOC,
    .sda_pin = GPIO_Pin_7,
    .scl_port = GPIOC,
    .scl_pin = GPIO_Pin_5
};

// Прив'язуємо аудіочип до нашої софтової шини
PT2259_t pt1 = {.i2c = &i2c1};

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    SDI_Printf_Enable();

    // Обов'язково вмикаємо порт С!
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    systick_init();
    softi2c_init(&i2c1); // Ініціалізуємо піни шини

    // Скидаємо чип при старті
    pt2259_clear(&pt1);
    // Ставимо початкову гучність 25%
    pt2259_setvolume_both(&pt1, false, 25);

    while(1)
    {
        static uint32_t timer = 0;
        static uint8_t vol = 25;

        // Кожні 4 секунди міняємо гучність для тесту
        if (millis() - timer >= 4000) {
            timer = millis();
            
            vol += 25;
            if (vol > 100) vol = 25;

            pt2259_setvolume_both(&pt1, false, vol);
        }
    }
}
