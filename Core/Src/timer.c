#include "main.h"
#include "global.h"
#include "software_timer.h"
#include "input_reading.h"

static int buttonTick = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        timerRun();

        /* quet nut moi BUTTON_SCAN_MS (>10ms) - khong phu thuoc TIMER_CYCLE */
        if (++buttonTick >= BUTTON_SCAN_MS / TIMER_CYCLE) {
            buttonTick = 0;
            button_reading();
        }
    }
}
