#include "software_timer.h"

static volatile int     timerCounter[MAX_TIMERS];
static volatile uint8_t timerFlag[MAX_TIMERS];

void setTimer(int index, int duration_ms)
{
    if (index < 0 || index >= MAX_TIMERS) return;
    int ticks = duration_ms / TIMER_CYCLE;
    if (ticks < 1) ticks = 1;
    timerCounter[index] = ticks;
    timerFlag[index] = 0;
}

int isTimerExpired(int index)
{
    if (index < 0 || index >= MAX_TIMERS) return 0;
    if (timerFlag[index]) {
        timerFlag[index] = 0;
        return 1;
    }
    return 0;
}

void timerRun(void)
{
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timerCounter[i] > 0) {
            timerCounter[i]--;
            if (timerCounter[i] == 0) timerFlag[i] = 1;
        }
    }
}
