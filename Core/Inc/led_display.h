#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "global.h"

#define SCAN_MS   10      /* thoi gian sang cua moi LED 7 doan */

/* 12 LED giao thong: duong A = D1-D3, D7-D9 ; duong B = D4-D6, D10-D12 */
void setRoadA(Light_t color);
void setRoadB(Light_t color);
void allTrafficLedOff(void);
void blinkColor(Light_t color, int on);   /* ca 2 duong cung mau */

/* 4 LED 7 doan: EN0,EN1 = trai/phai cua cap 1 ; EN2,EN3 = cap 2 */
void setDigit(int index, int value);
void display_scan(void);                  /* goi trong vong lap chinh */

#endif /* INC_LED_DISPLAY_H_ */
