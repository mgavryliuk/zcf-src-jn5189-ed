#include "device_config.h"

#include "device_definitions.h"
#include "zps_gen.h"

const tsEndpointButtonConfig sEndpointButtonsConfigs[] = {
    {
        .u32DioPin = BTN_LEFT_DIO,
        .u32DioMask = BTN_LEFT_MASK,
        .u32LedPin = LED_LEFT_DIO,
        .u32LedMask = LED_LEFT_MASK,
        .bHasLed = TRUE,
        .u16Endpoint = WXKG15LM_LEFTBUTTON_ENDPOINT,
    },
    {
        .u32DioPin = BTN_RIGHT_DIO,
        .u32DioMask = BTN_RIGHT_MASK,
        .u32LedPin = LED_RIGHT_DIO,
        .u32LedMask = LED_RIGHT_MASK,
        .bHasLed = TRUE,
        .u16Endpoint = WXKG15LM_RIGHTBUTTON_ENDPOINT,
    },
};

const tsDeviceConfig sDeviceConfig = {
    .u8BasicEndpoint = WXKG15LM_BASIC_ENDPOINT,
    .u8ZdoEndpoint = WXKG15LM_ZDO_ENDPOINT,
    .sControlMasks =
        {
            .u32DioMask = BTN_CTRL_MASK,
            .u32LedMask = LEDS_CTRL_MASK,
        },
    .sResetButtonConfig =
        {
            .u32DioMask = BTN_CTRL_MASK,
            .u32LedMask = LEDS_CTRL_MASK,
            .bHasLed = TRUE,
        },
    .u8EndpointButtonsCount = sizeof(sEndpointButtonsConfigs) / sizeof(tsEndpointButtonConfig),
    .psEndpointButtons = sEndpointButtonsConfigs,
};
