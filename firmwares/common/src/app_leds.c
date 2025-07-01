#include "app_leds.h"

#include <jendefs.h>

#include "ZTimer.h"
#include "dbg.h"
#include "device_config.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"

uint8_t u8TimerLedBlink;
static ts_BlinkState sBlinkStates[MAX_BLINK_STATES];

void APP_Leds_Init(void) {
    DBG_vPrintf(TRACE_LEDS, "LEDS: APP_Leds_Init called\n");

    gpio_pin_config_t led_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U,
    };

    uint8_t i = 0;
    for (; i < sDeviceConfig.u8EndpointButtonsCount; i++) {
        DBG_vPrintf(TRACE_LEDS, "LEDS: Configuring LED %d for Button %d\n", sDeviceConfig.psEndpointButtons[i].u32LedPin,
                    sDeviceConfig.psEndpointButtons[i].u32DioPin);
        IOCON_PinMuxSet(IOCON, 0, sDeviceConfig.psEndpointButtons[i].u32LedPin, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);

        GPIO_PinInit(GPIO, 0, sDeviceConfig.psEndpointButtons[i].u32LedPin, &led_config);
    }
    DBG_vPrintf(TRACE_LEDS, "LEDS: APP_Leds_Init done\n");
}

void APP_Leds_Blink(uint32_t u32LedMask, uint8_t u8BlinkCount) {
    uint8_t i;
    for (i = 0; i < MAX_BLINK_STATES; i++) {
        if (sBlinkStates[i].u32Mask & u32LedMask) {
            DBG_vPrintf(TRACE_LEDS, "LEDS: Blink already in progress for mask: %08x\n", u32LedMask);
            return;
        }
    }

    DBG_vPrintf(TRACE_LEDS, "LEDS: Starting Blink for mask: %08x\n", u32LedMask);
    for (i = 0; i < MAX_BLINK_STATES; i++) {
        if (sBlinkStates[i].u32Mask == 0) {
            sBlinkStates[i].u32Mask = u32LedMask;
            sBlinkStates[i].u8Cycles = u8BlinkCount << 1;
            sBlinkStates[i].bIsOn = FALSE;
            break;
        }
    }

    ZTIMER_teState eZtimerState = ZTIMER_eGetState(u8TimerLedBlink);
    if (eZtimerState == E_ZTIMER_STATE_RUNNING) {
        DBG_vPrintf(TRACE_LEDS, "LEDS: Blink Timer is already running\n");
        return;
    }
    DBG_vPrintf(TRACE_LEDS, "LEDS: Starting Blink Timer\n");
    ZTIMER_eStart(u8TimerLedBlink, LED_BLINK_INTERVAL);
}

void APP_Leds_cbTimerBlink(void *pvParam) {
    DBG_vPrintf(TRACE_LEDS, "LEDS: Blink Timer Callback - Enter\n");
    uint32_t u32TurnOnMask = 0;
    uint32_t u32TurnOffMask = 0;
    uint8_t i;

    for (i = 0; i < MAX_BLINK_STATES; i++) {
        if (sBlinkStates[i].u8Cycles > 0) {
            if (sBlinkStates[i].bIsOn) {
                u32TurnOffMask |= sBlinkStates[i].u32Mask;
                sBlinkStates[i].bIsOn = FALSE;
            } else {
                u32TurnOnMask |= sBlinkStates[i].u32Mask;
                sBlinkStates[i].bIsOn = TRUE;
            }
            sBlinkStates[i].u8Cycles--;

            if (sBlinkStates[i].u8Cycles == 0) {
                sBlinkStates[i].u32Mask = 0;
            }
        }
    }

    if (u32TurnOffMask || u32TurnOnMask) {
        DBG_vPrintf(TRACE_LEDS, "LEDS: Blink Timer Callback - Restart. On: %08x, Off: %08x\n", u32TurnOnMask, u32TurnOffMask);
        GPIO_PortSet(GPIO, 0, u32TurnOffMask);
        GPIO_PortClear(GPIO, 0, u32TurnOnMask);
        ZTIMER_eStart(u8TimerLedBlink, LED_BLINK_INTERVAL);
    } else {
        DBG_vPrintf(TRACE_LEDS, "LEDS: Blink Timer Callback - Stop: %08x\n", sDeviceConfig.sControlMasks.u32LedMask);
        GPIO_PortSet(GPIO, 0, sDeviceConfig.sControlMasks.u32LedMask);
    }
}
