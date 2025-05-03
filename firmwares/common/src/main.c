#include <jendefs.h>

#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_reset.h"
#include "fsl_gpio.h"

#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"

// #include "fsl_clock.h"
// #include "fsl_debug_console.h"

// #include "dbg.h"
// #include "pwrm.h"

// static PWRM_DECLARE_CALLBACK_DESCRIPTOR(PreSleep);
// static PWRM_DECLARE_CALLBACK_DESCRIPTOR(Wakeup);

void BOARD_InitHardware(void)
{
    POWER_Init();

    RESET_PeripheralReset(kUSART0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);

    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
}

void hardware_init(void)
{
    BOARD_InitHardware();
}

// void vAppRegisterPWRCallbacks(void)
// {
//     PWRM_vRegisterPreSleepCallback(PreSleep);
//     PWRM_vRegisterWakeupCallback(Wakeup);
// }

// PWRM_CALLBACK(PreSleep)
// {
//     DBG_vPrintf(TRUE, "Going to sleep\n");
// }

// PWRM_CALLBACK(Wakeup)
// {
//     DBG_vPrintf(TRUE, "Wake up from sleep\n");
// }

void main_task(uint32_t parameter)
{
    while (1)
    {
        /* Toggle left LED */
        GPIO_PortToggle(GPIO, 0U, 1u << 7U);
        CLOCK_uDelay(500000);
        /* Toggle right LED */
        GPIO_PortToggle(GPIO, 0U, 1u << 10U);
        CLOCK_uDelay(500000);
    }
}
