#include "app_main.h"

#include <jendefs.h>

#include "JN5189.h"
#include "PDM.h"
#include "PWR_Interface.h"
#include "ZTimer.h"
#include "app_buttons.h"
#include "app_leds.h"
#include "app_resources.h"
#include "dbg.h"
#include "device_config.h"
#include "fsl_power.h"
#include "fsl_wwdt.h"
#include "pwrm.h"

#ifdef DBG_ENABLE
static uint32 logSleepAttempt = 0;
#endif

static void PreSleep(void);
static void OnWakeUp(void);
static void WakeCallBack(void);
static uint8 getBlockingTimersAmount(void);
static uint8 getNonBlockingTimersAmount(void);
static void ManageSleepPrerequisites(void);
static void EnterMainLoop(void);

static PWR_tsWakeTimerEvent sWake;

void vAppRegisterPWRCallbacks(void) {
    PWR_RegisterLowPowerEnterCallback(PreSleep);
    PWR_RegisterLowPowerExitCallback(OnWakeUp);
}

void main_task(uint32_t parameter) {
    DBG_vPrintf(TRUE, "APP_MAIN: main_task called\n");
    APP_Resources_Init();

    PDM_eInitialise(1200, 63, NULL);
    PWR_ChangeDeepSleepMode(PWR_E_SLEEP_OSCON_RAMON);
    PWR_Init();

    APP_Buttons_cbTimerScan(NULL);
    EnterMainLoop();
}

static void PreSleep(void) {
    DBG_vPrintf(TRACE_APP_MAIN, "APP_MAIN: PreSleep called\n");
    DbgConsole_Flush();
    DbgConsole_Deinit();
    ZTIMER_vSleep();
}

static void OnWakeUp(void) {
    DBG_vPrintf(TRACE_APP_MAIN, "APP_MAIN: On WakeUp called\n");
    ZTIMER_vWake();
    if (POWER_GetIoWakeStatus() & sDeviceConfig.sControlMasks.u32DioMask) {
        DBG_vPrintf(TRACE_APP_MAIN, "APP_MAIN: Button pressed: %08x\n", POWER_GetIoWakeStatus());
        ZTIMER_eStart(u8TimerButtonScan, BUTTON_SCAN_TIME_MSEC);
    }
}

static void WakeCallBack(void) { DBG_vPrintf(TRACE_APP_MAIN, "APP_MAIN: Wake callback called\n"); }

static uint8 getBlockingTimersAmount(void) {
    uint8 u8NumberOfRunningTimers = 0;

    if (ZTIMER_eGetState(u8TimerLedBlink) == E_ZTIMER_STATE_RUNNING) {
        u8NumberOfRunningTimers++;
    }

    if (ZTIMER_eGetState(u8TimerButtonScan) == E_ZTIMER_STATE_RUNNING) {
        u8NumberOfRunningTimers++;
    }

    return u8NumberOfRunningTimers;
}

static uint8 getNonBlockingTimersAmount(void) {
    uint8 u8NumberOfRunningTimers = 0;
    return u8NumberOfRunningTimers;
}

static void ManageSleepPrerequisites(void) {
#ifdef DBG_ENABLE
    logSleepAttempt++;
    if (logSleepAttempt == 3200 * 10) {
        DBG_vPrintf(TRUE, "APP MAIN: Activity Count = %d\n", PWRM_u16GetActivityCount());
        DBG_vPrintf(TRUE, "APP MAIN: Task Timers = %d\n", getBlockingTimersAmount());
        logSleepAttempt = 0;
    }
#endif

    // if (ZTIMER_eGetState(u8TimerPoll) == E_ZTIMER_STATE_RUNNING) {
    //     return;
    // }

    if ((PWRM_u16GetActivityCount() == getNonBlockingTimersAmount()) && (getBlockingTimersAmount() == 0)) {
        DBG_vPrintf(TRUE, "APP MAIN: Activity Count = %d\n", PWRM_u16GetActivityCount());
        DBG_vPrintf(TRUE, "APP MAIN: Task Timers = %d\n", getBlockingTimersAmount());

        // PWRM_vInit(E_AHI_SLEEP_OSCON_RAMON);
        DBG_vPrintf(TRACE_APP_MAIN,
                    "APP_MAIN: Going to E_AHI_SLEEP_OSCON_RAMON sleep "
                    "for %d seconds\n",
                    MAXIMUM_TIME_TO_SLEEP_SEC);
        PWR_vWakeUpConfig(sDeviceConfig.sControlMasks.u32DioMask);
        PWR_teStatus eStatus = PWR_eRemoveActivity(&sWake);
        DBG_vPrintf(TRACE_APP_MAIN, "APP_MAIN: PWR_eRemoveActivity status: %d\n", eStatus);
        eStatus = PWR_eScheduleActivity(&sWake, MAXIMUM_TIME_TO_SLEEP_SEC * 1000, WakeCallBack);
        DBG_vPrintf(TRACE_APP_MAIN, "APP_MAIN: PWR_eScheduleActivity status: %d\n", eStatus);
    }
}

static void EnterMainLoop(void) {
    while (1) {
        ZTIMER_vTask();
        WWDT_Refresh(WWDT);
        ManageSleepPrerequisites();
        PWR_EnterLowPower();
    }
}
