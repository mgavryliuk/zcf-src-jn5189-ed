#include <jendefs.h>

#include "fsl_clock.h"
#include "fsl_debug_console.h"

#include "dbg.h"
#include "pwrm.h"

static PWRM_DECLARE_CALLBACK_DESCRIPTOR(PreSleep);
static PWRM_DECLARE_CALLBACK_DESCRIPTOR(Wakeup);

void hardware_init(void)
{
    uint32_t uartClkSrcFreq = CLOCK_GetFreq(kCLOCK_Fro32M);
    DbgConsole_Init(0U, 115200U, kSerialPort_Uart, uartClkSrcFreq);
}

void vAppRegisterPWRCallbacks(void)
{
    PWRM_vRegisterPreSleepCallback(PreSleep);
    PWRM_vRegisterWakeupCallback(Wakeup);
}

PWRM_CALLBACK(PreSleep)
{
    DBG_vPrintf(TRUE, "Going to sleep\n");
}

PWRM_CALLBACK(Wakeup)
{
    DBG_vPrintf(TRUE, "Wake up from sleep\n");
}

void main_task(uint32_t parameter)
{
}
