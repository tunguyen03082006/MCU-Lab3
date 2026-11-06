#include "led_display.h"
#include "software_timer.h"

/* LED noi anode len +3.3V  -> muc 0 = SANG, muc 1 = TAT */
#define LED_ON   GPIO_PIN_RESET
#define LED_OFF  GPIO_PIN_SET

/* ================= 12 LED giao thong ================= */
static void roadA_off(void)
{
    HAL_GPIO_WritePin(RED_A_GPIO_Port,    RED_A_Pin,    LED_OFF);
    HAL_GPIO_WritePin(YELLOW_A_GPIO_Port, YELLOW_A_Pin, LED_OFF);
    HAL_GPIO_WritePin(GREEN_A_GPIO_Port,  GREEN_A_Pin,  LED_OFF);
}
static void roadB_off(void)
{
    HAL_GPIO_WritePin(RED_B_GPIO_Port,    RED_B_Pin,    LED_OFF);
    HAL_GPIO_WritePin(YELLOW_B_GPIO_Port, YELLOW_B_Pin, LED_OFF);
    HAL_GPIO_WritePin(GREEN_B_GPIO_Port,  GREEN_B_Pin,  LED_OFF);
}

void setRoadA(Light_t color)
{
    roadA_off();
    switch (color) {
    case LIGHT_RED:    HAL_GPIO_WritePin(RED_A_GPIO_Port,    RED_A_Pin,    LED_ON); break;
    case LIGHT_YELLOW: HAL_GPIO_WritePin(YELLOW_A_GPIO_Port, YELLOW_A_Pin, LED_ON); break;
    case LIGHT_GREEN:  HAL_GPIO_WritePin(GREEN_A_GPIO_Port,  GREEN_A_Pin,  LED_ON); break;
    }
}

void setRoadB(Light_t color)
{
    roadB_off();
    switch (color) {
    case LIGHT_RED:    HAL_GPIO_WritePin(RED_B_GPIO_Port,    RED_B_Pin,    LED_ON); break;
    case LIGHT_YELLOW: HAL_GPIO_WritePin(YELLOW_B_GPIO_Port, YELLOW_B_Pin, LED_ON); break;
    case LIGHT_GREEN:  HAL_GPIO_WritePin(GREEN_B_GPIO_Port,  GREEN_B_Pin,  LED_ON); break;
    }
}

void allTrafficLedOff(void)
{
    roadA_off();
    roadB_off();
}

void blinkColor(Light_t color, int on)
{
    allTrafficLedOff();
    if (!on) return;
    setRoadA(color);
    setRoadB(color);
}

/* ================= 4 LED 7 doan (anode chung, qua PNP) ================= */
/* bit0 = a ... bit6 = g (1 = doan sang) */
static const uint8_t segMask[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

static GPIO_TypeDef * const segPort[7] = {
    SEG_A_GPIO_Port, SEG_B_GPIO_Port, SEG_C_GPIO_Port, SEG_D_GPIO_Port,
    SEG_E_GPIO_Port, SEG_F_GPIO_Port, SEG_G_GPIO_Port };
static const uint16_t segPin[7] = {
    SEG_A_Pin, SEG_B_Pin, SEG_C_Pin, SEG_D_Pin, SEG_E_Pin, SEG_F_Pin, SEG_G_Pin };

static GPIO_TypeDef * const enPort[4] = {
    EN0_GPIO_Port, EN1_GPIO_Port, EN2_GPIO_Port, EN3_GPIO_Port };
static const uint16_t enPin[4] = { EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin };

static uint8_t ledBuffer[4] = { 0, 0, 0, 0 };
static int scanIndex = 0;

void setDigit(int index, int value)
{
    if (index < 0 || index > 3 || value < 0 || value > 9) return;
    ledBuffer[index] = (uint8_t)value;
}

static void update7SEG(int index)
{
    /* tat het EN truoc (PNP: base = 1 -> tat) de tranh bong mo */
    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(enPort[i], enPin[i], GPIO_PIN_SET);

    uint8_t mask = segMask[ledBuffer[index]];
    for (int s = 0; s < 7; s++)
        HAL_GPIO_WritePin(segPort[s], segPin[s],
                          (mask >> s) & 1 ? GPIO_PIN_RESET : GPIO_PIN_SET);

    /* bat LED can hien thi (PNP: base = 0 -> dan) */
    HAL_GPIO_WritePin(enPort[index], enPin[index], GPIO_PIN_RESET);
}

void display_scan(void)
{
    if (isTimerExpired(TIMER_SCAN)) {
        setTimer(TIMER_SCAN, SCAN_MS);
        update7SEG(scanIndex);
        scanIndex = (scanIndex + 1) % 4;
    }
}
