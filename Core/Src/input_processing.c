#include "main.h"
#include "global.h"
#include "input_reading.h"
#include "input_processing.h"
#include "software_timer.h"
#include "traffic_light.h"

typedef enum { BTN_RELEASED, BTN_PRESSED, BTN_PRESSED_MORE_THAN_1_SECOND } ButtonState_t;
static ButtonState_t buttonState[3] = { BTN_RELEASED, BTN_RELEASED, BTN_RELEASED };

/* FSM cho 1 nut. Tra ve 1 khi co "su kien": luc vua nhan,
 * va (neu autoRepeat) moi 500ms khi giu qua 1 giay. */
static uint8_t button_event(uint8_t i, uint8_t autoRepeat)
{
    uint8_t fire = 0;
    switch (buttonState[i]) {
    case BTN_RELEASED:
        if (is_button_pressed(i)) {
            buttonState[i] = BTN_PRESSED;
            fire = 1;
        }
        break;
    case BTN_PRESSED:
        if (!is_button_pressed(i)) {
            buttonState[i] = BTN_RELEASED;
        } else if (is_button_pressed_1s(i)) {
            buttonState[i] = BTN_PRESSED_MORE_THAN_1_SECOND;
            if (autoRepeat) setTimer(TIMER_REPEAT, 500);
        }
        break;
    case BTN_PRESSED_MORE_THAN_1_SECOND:
        if (!is_button_pressed(i)) {
            buttonState[i] = BTN_RELEASED;
        } else if (autoRepeat && isTimerExpired(TIMER_REPEAT)) {
            setTimer(TIMER_REPEAT, 500);
            fire = 1;
        }
        break;
    }
    return fire;
}

void fsm_for_input_processing(void)
{
    uint8_t ev1 = button_event(0, 0);   /* nut 1: doi mode           */
    uint8_t ev2 = button_event(1, 1);   /* nut 2: tang gia tri       */
    uint8_t ev3 = button_event(2, 0);   /* nut 3: xac nhan (set)     */

    if (ev1) {
        Mode_t next = (mode == MODE_GREEN) ? MODE_NORMAL : (Mode_t)(mode + 1);
        traffic_enter_mode(next);
        return;
    }

    if (mode != MODE_NORMAL) {
        if (ev2) {
            if (++tempValue > MAX_VALUE) tempValue = 1;
        }
        if (ev3) {
            if (mode == MODE_RED)         durRed    = tempValue;
            else if (mode == MODE_YELLOW) durYellow = tempValue;
            else                          durGreen  = tempValue;
            lastEdited = mode;
        }
    }
}
