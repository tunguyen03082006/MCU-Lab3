#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "global.h"

#define TIMER_SEC      0   /* dem 1 giay cho den giao thong     */
#define TIMER_BLINK    1   /* nhap nhay 2Hz (250ms)             */
#define TIMER_SCAN     2   /* quet LED 7 doan                   */
#define TIMER_REPEAT   3   /* tu tang khi giu nut 2 (500ms)     */
#define MAX_TIMERS     4

void setTimer(int index, int duration_ms);
int  isTimerExpired(int index);   /* tra ve 1 mot lan khi het gio */
void timerRun(void);              /* goi trong ngat Timer2        */

#endif /* INC_SOFTWARE_TIMER_H_ */
