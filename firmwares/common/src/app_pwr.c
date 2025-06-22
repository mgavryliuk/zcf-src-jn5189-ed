#include <jendefs.h>

#include "JN5189.h"

#include "dbg.h"

#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "fsl_power.h"

#include "PWR_Interface.h"
#include "app_pwr.h"

PRIVATE void PreSleep(void);
PRIVATE void OnWakeUp(void);
PRIVATE void ConfigurePinsForPowerMode(void);
PRIVATE void vWakeCallBack(void);

PRIVATE PWR_tsWakeTimerEvent sWake;

bool_t bWaked = FALSE;

void vAppRegisterPWRCallbacks(void)
{
    PWR_RegisterLowPowerEnterCallback(PreSleep);
    PWR_RegisterLowPowerExitCallback(OnWakeUp);
}

void vAppScheduleActivity(void) {
    PWR_teStatus eStatus = PWR_eRemoveActivity(&sWake);
    DBG_vPrintf(TRACE_PWR, "APP_PWR: PWR_eRemoveActivity status: %d\n", eStatus);
    eStatus = PWR_eScheduleActivity(&sWake, MAXIMUM_TIME_TO_SLEEP_SEC * 1000, vWakeCallBack);
    DBG_vPrintf(TRACE_PWR, "APP_PWR: PWR_eScheduleActivity status: %d\n", eStatus);
}

PRIVATE void PreSleep(void)
{
    DBG_vPrintf(TRACE_PWR, "APP_PWR: PreSleep called\n");
    ConfigurePinsForPowerMode();
    DbgConsole_Flush();
    DbgConsole_Deinit();
}

PRIVATE void OnWakeUp(void)
{
    DBG_vPrintf(TRACE_PWR, "APP_PWR: On WakeUp called\n");
    if (POWER_GetIoWakeStatus() & (1 << 7)) {
        DBG_vPrintf(TRACE_PWR, "APP_PWR: Wake up from button press: 0x%08x\n", POWER_GetIoWakeStatus());
        DBG_vPrintf(TRACE_PWR, "APP_PWR: GPIO_PinRead: 0x%08x\n", GPIO_PinRead(GPIO, 0, 7));
        DBG_vPrintf(TRACE_PWR, "APP_PWR: GPIO_PortRead: 0x%08x\n", GPIO_PortRead(GPIO, 0));
    }
}

PRIVATE void ConfigurePinsForPowerMode(void)
{
    DBG_vPrintf(TRACE_PWR, "APP_PWR: Configuring pins for power mode...\n");
    // TODO: skip PIOs that are used for buttons
    for ( int i=0; i<22; i++)
    {
        if (i != 7)
        {
            GPIO_PinInit(GPIO, 0, i, &((const gpio_pin_config_t){kGPIO_DigitalInput, 1}));
            IOCON_PinMuxSet(IOCON, 0, i, IOCON_FUNC0 |  IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);
        }
    }
}

PRIVATE void vWakeCallBack(void)
{
    DBG_vPrintf(TRUE, "APP_PWR: Wake callback called\n");
    bWaked = TRUE;
}
