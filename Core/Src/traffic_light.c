#include "traffic_light.h"
#include "led_display.h"
#include "software_timer.h"

/* moi duong co 1 FSM rieng: RED -> GREEN -> YELLOW -> RED */
static Light_t stateA, stateB;
static int     countA, countB;      /* so giay con lai cua mau hien tai */
static int     blinkOn = 1;

static void step(Light_t *s, int *c)
{
    if (--(*c) <= 0) {
        switch (*s) {
        case LIGHT_RED:    *s = LIGHT_GREEN;  *c = durGreen;  break;
        case LIGHT_GREEN:  *s = LIGHT_YELLOW; *c = durYellow; break;
        default:           *s = LIGHT_RED;    *c = durRed;    break;
        }
    }
}

/* dam bao do = xanh + vang; gia tri vua sua gan nhat duoc uu tien */
static void validate_durations(void)
{
    if (lastEdited == MODE_RED) {
        if (durRed < 2) durRed = 2;
        if (durYellow >= durRed) durYellow = durRed - 1;
        durGreen = durRed - durYellow;
    } else {
        durRed = durGreen + durYellow;
        if (durRed > MAX_VALUE) {
            if (lastEdited == MODE_YELLOW) durGreen  = MAX_VALUE - durYellow;
            else                           durYellow = MAX_VALUE - durGreen;
            durRed = MAX_VALUE;
        }
        if (durGreen  < 1) { durGreen  = 1; durYellow = MAX_VALUE - 1; durRed = MAX_VALUE; }
        if (durYellow < 1) { durYellow = 1; durGreen  = MAX_VALUE - 1; durRed = MAX_VALUE; }
    }
}

static Light_t modeColor(Mode_t m)
{
    return (m == MODE_RED) ? LIGHT_RED : (m == MODE_YELLOW) ? LIGHT_YELLOW : LIGHT_GREEN;
}

static void show2digits(int firstIdx, int value)
{
    setDigit(firstIdx,     value / 10);
    setDigit(firstIdx + 1, value % 10);
}

void traffic_enter_mode(Mode_t newMode)
{
    mode = newMode;
    allTrafficLedOff();

    if (newMode == MODE_NORMAL) {
        validate_durations();
        stateA = LIGHT_GREEN; countA = durGreen;   /* duong A xanh */
        stateB = LIGHT_RED;   countB = durRed;     /* duong B do   */
        setRoadA(stateA);
        setRoadB(stateB);
        show2digits(0, countA);
        show2digits(2, countB);
        setTimer(TIMER_SEC, 1000);
    } else {
        tempValue = (newMode == MODE_RED) ? durRed :
                    (newMode == MODE_YELLOW) ? durYellow : durGreen;
        blinkOn = 1;
        blinkColor(modeColor(newMode), blinkOn);
        setTimer(TIMER_BLINK, 250);                /* 2Hz = bat/tat moi 250ms */
    }
}

void traffic_run(void)
{
    if (mode == MODE_NORMAL) {
        if (isTimerExpired(TIMER_SEC)) {
            setTimer(TIMER_SEC, 1000);
            step(&stateA, &countA);
            step(&stateB, &countB);
            setRoadA(stateA);
            setRoadB(stateB);
            show2digits(0, countA);     /* EN0,EN1: duong A */
            show2digits(2, countB);     /* EN2,EN3: duong B */
        }
    } else {
        if (isTimerExpired(TIMER_BLINK)) {
            setTimer(TIMER_BLINK, 250);
            blinkOn = !blinkOn;
            blinkColor(modeColor(mode), blinkOn);
        }
        show2digits(0, tempValue);      /* EN0,EN1: gia tri dang sua */
        setDigit(2, 0);                 /* EN2,EN3: so mode (02..04) */
        setDigit(3, (int)mode);
    }
}
