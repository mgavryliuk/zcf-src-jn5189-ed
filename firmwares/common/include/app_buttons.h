#ifndef APP_BUTTONS_H
#define APP_BUTTONS_H

#include <jendefs.h>

#include "ZTimer.h"
#include "device_config.h"

#ifdef DEBUG_APP_BUTTONS
#define TRACE_BUTTONS TRUE
#else
#define TRACE_BUTTONS FALSE
#endif

#ifndef APP_RESET_DEVICE_CYCLES
#define APP_RESET_DEVICE_CYCLES 500  // 500 * BUTTON_SCAN_TIME_MSEC = 5 sec
#endif

#define APP_BTN_DEBOUNCE_MASK 0b0111

#define BUTTON_SCAN_TIME_MSEC ZTIMER_TIME_MSEC(10)
// 40 * BUTTON_SCAN_TIME_MSEC = 400ms
#define APP_BTN_REGISTER_WINDOW_CYCLES 40
// 70 * BUTTON_SCAN_TIME_MSEC = 700ms
#define APP_BTN_LONG_PRESS_REGISTER_CYCLES 70
// 100 * BUTTON_SCAN_TIME_MSEC = 1sec
#define APP_BTN_IDLE_CYCLES_MAX 100

typedef enum { IDLE = 0, SINGLE_CLICK, DOUBLE_CLICK, TRIPLE_CLICK, LONG_CLICK } APP_teButtonState;

typedef struct {
    bool_t bPressed;
    uint16_t u16Cycles;
    uint8_t u8Debounce;
    APP_teButtonState eState;
    union {
        const tsEndpointButtonConfig *sEndpointBtnConfig;
        const tsResetButtonConfig *sResetBtnConfig;
    } u;
} tsButtonState;

extern uint8_t u8TimerButtonScan;

void APP_Buttons_Init(void);
void APP_Buttons_cbTimerScan(void *pvParam);

#endif /* APP_BUTTONS_H */
