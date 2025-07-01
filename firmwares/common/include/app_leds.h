#ifndef LEDS_H
#define LEDS_H

#include <jendefs.h>

#include "ZTimer.h"

#ifdef DEBUG_APP_LEDS
#define TRACE_LEDS TRUE
#else
#define TRACE_LEDS FALSE
#endif

#define LED_BLINK_INTERVAL ZTIMER_TIME_MSEC(100)

typedef struct {
    uint32_t u32Mask;
    uint8_t u8Cycles;
    bool_t bIsOn;
} ts_BlinkState;

extern uint8_t u8TimerLedBlink;

void APP_Leds_Init(void);
void APP_Leds_Blink(uint32_t u32LedMask, uint8_t u8BlinkCount);
void APP_Leds_cbTimerBlink(void *pvParam);

#endif /* LEDS_H */
