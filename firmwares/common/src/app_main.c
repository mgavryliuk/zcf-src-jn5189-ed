#include <jendefs.h>

#include "JN5189.h"

#include "fsl_gpio.h"
#include "fsl_os_abstraction.h"
#include "fsl_wwdt.h"

#include "dbg.h"
#include "PDM.h"
#include "PWR_Interface.h"
#include "SecLib.h"
#include "RNG_Interface.h"
#include "MemManager.h"
#include "TimersManager.h"

#include "app_pwr.h"
#include "app_wwdt.h"
#include "buttons_defintions.h"

void main_task(uint32_t parameter)
{
    DBG_vPrintf(TRUE, "APP_MAIN: main_task called\n");
    PDM_eInitialise(1200, 63, NULL);

    // PWR_ChangeDeepSleepMode(PWR_E_SLEEP_OSCON_RAMON);
    // PWR_Init();

    // TODO: move this config to hardware_init function
    SecLib_Init();
    if (gRngSuccess_d != RNG_Init())
    {
        DBG_vPrintf(TRUE, "RNG_Init failed\n");
    }
    if (MEM_SUCCESS_c != MEM_Init())
    {
        DBG_vPrintf(TRUE, "MEM_Init failed\n");
    };
    TMR_Init();
    vAppSetupWWDT();
    // TODO: init resources
    // TODO: bdb start
    PWR_vWakeUpConfig(BTN_CTRL_MASK);

    while (1)
    {
        WWDT_Refresh(WWDT);
        vAppScheduleActivity();
        PWR_EnterLowPower();
    }
}
