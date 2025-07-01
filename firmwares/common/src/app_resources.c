#include "app_resources.h"

#include <jendefs.h>

#include "ZQueue.h"
#include "ZTimer.h"
#include "app_buttons.h"
#include "app_events.h"
#include "app_leds.h"
#include "dbg.h"

static ZTIMER_tsTimer asTimers[APP_ZTIMER_STORAGE];

tszQueue APP_msgAppEvents;

static void APP_vInitTimers(void);
static void APP_vInitQueues(void);

void APP_Resources_Init(void) {
    DBG_vPrintf(TRACE_APP_RESOURCES, "APP_RESOURCES: APP_vInitResources called\n");
    APP_vInitTimers();
    APP_vInitQueues();
    DBG_vPrintf(TRACE_APP_RESOURCES, "APP_RESOURCES: APP_vInitResources finished\n");
}

static void APP_vInitTimers(void) {
    DBG_vPrintf(TRACE_APP_RESOURCES, "APP_RESOURCES: APP_vInitTimers called\n");
    ZTIMER_eInit(asTimers, sizeof(asTimers) / sizeof(ZTIMER_tsTimer));
    ZTIMER_eOpen(&u8TimerLedBlink, APP_Leds_cbTimerBlink, NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerButtonScan, APP_Buttons_cbTimerScan, NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    DBG_vPrintf(TRACE_APP_RESOURCES, "APP_RESOURCES: APP_vInitTimers finished\n");
}

static void APP_vInitQueues(void) {
    DBG_vPrintf(TRACE_APP_RESOURCES, "APP_RESOURCES: APP_vInitQueues called\n");
    ZQ_vQueueCreate(&APP_msgAppEvents, APP_QUEUE_SIZE, sizeof(APP_tsEvent), NULL);
    DBG_vPrintf(TRACE_APP_RESOURCES, "APP_RESOURCES: APP_vInitQueues finished\n");
}
