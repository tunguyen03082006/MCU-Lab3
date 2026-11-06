#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include <stdint.h>

/* Chu ky ngat Timer2 (ms). PHAI khop voi cau hinh TIM2:
 * f = 8MHz / (Prescaler+1) / (Period+1)  ->  Prescaler=7999, Period=9 => 10ms
 * Doi sang 1ms hay 100ms chi can sua Period va dong nay (cac hieu ung khac khong doi). */
#define TIMER_CYCLE      10

#define MAX_VALUE        99      /* thoi gian toi da cua 1 den (giay) */

typedef enum { MODE_NORMAL = 1, MODE_RED = 2, MODE_YELLOW = 3, MODE_GREEN = 4 } Mode_t;
typedef enum { LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW } Light_t;

extern Mode_t mode;          /* mode hien tai                        */
extern Mode_t lastEdited;    /* den vua duoc chinh sua gan nhat      */
extern int durRed;           /* thoi gian den do (s)                 */
extern int durYellow;        /* thoi gian den vang (s)               */
extern int durGreen;         /* thoi gian den xanh (s)               */
extern int tempValue;        /* gia tri dang chinh sua (1..99)       */

#endif /* INC_GLOBAL_H_ */
