#include <jendefs.h>

#include "JN5189.h"

#include "dbg.h"

#include "fsl_common.h"
#include "fsl_gint.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "fsl_power.h"
#include "fsl_reset.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"

#include "PWR_Interface.h"

#define UART_PIO_TX (8U)
#define UART_PIO_RX (9U)
#define UART_BAUD_RATE (115200U)

#define APP_BUTTONS_DIO_MASK (1 << 7)
#define GINT0_POL_MASK (APP_BUTTONS_DIO_MASK)
#define GINT0_ENA_MASK (APP_BUTTONS_DIO_MASK)

PRIVATE void InitClocks(void);
PRIVATE void InitDebugConsole(void);
PRIVATE void InitPins(void);
PRIVATE void ConfigureGINT(void);

void gint_callback(void);

static bool_t bWarmStart = FALSE;

void hardware_init(void)
{
    POWER_Init();

    InitClocks();
    InitDebugConsole();
    InitPins();

    // TODO: configure GINT here, cause it needs to be configured on cold start and reconfigured on each wake up...
    // Init Leds
    // Init Buttons
    ConfigureGINT();

    if (!bWarmStart)
    {
        bWarmStart = TRUE;
        PWR_vColdStart();
    }
}

PRIVATE void InitClocks(void)
{

    RESET_PeripheralReset(kUSART0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGINT_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kRTC_RST_SHIFT_RSTn);

    CLOCK_EnableAPBBridge();
    CLOCK_EnableClock(kCLOCK_Xtal32M);
    CLOCK_EnableClock(kCLOCK_Fro32k);

    CLOCK_AttachClk(kXTAL32M_to_MAIN_CLK);
    CLOCK_AttachClk(kMAIN_CLK_to_ASYNC_APB);
    CLOCK_AttachClk(kXTAL32M_to_OSC32M_CLK);
    CLOCK_AttachClk(kFRO32K_to_OSC32K_CLK);
    CLOCK_AttachClk(kOSC32M_to_USART_CLK);
    CLOCK_AttachClk(kOSC32K_to_WDT_CLK);
    CLOCK_AttachClk(kXTAL32M_to_ADC_CLK);

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_InputMux);
    CLOCK_EnableClock(kCLOCK_Gint);
    CLOCK_EnableClock(kCLOCK_Rtc);

    SYSCON->MAINCLKSEL = SYSCON_MAINCLKSEL_SEL(2);
    SystemCoreClockUpdate();
}

PRIVATE void InitDebugConsole(void)
{
    // TODO: we should not enable USART for non debug
    IOCON_PinMuxSet(IOCON, kGINT_Port0, UART_PIO_TX, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    IOCON_PinMuxSet(IOCON, kGINT_Port0, UART_PIO_RX, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    DbgConsole_Init(0U, UART_BAUD_RATE, kSerialPort_Uart, CLOCK_GetFreq(kCLOCK_Xtal32M));
}

PRIVATE void InitPins(void)
{
    // TODO: make it dynamic based on the config
    IOCON_PinMuxSet(IOCON, kGINT_Port0, 16U, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    gpio_pin_config_t led_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U};
    GPIO_PinInit(GPIO, kGINT_Port0, 16U, &led_config);

    IOCON_PinMuxSet(IOCON, kGINT_Port0, 7U, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_INV_EN | IOCON_DIGITAL_EN);
    gpio_pin_config_t button_config = {
        .pinDirection = kGPIO_DigitalInput,
    };
    GPIO_PinInit(GPIO, kGINT_Port0, 7U, &button_config);
}

PRIVATE void ConfigureGINT(void)
{
    DBG_vPrintf(TRUE, "Configure GINT\n");
    GINT_Init(GINT0);
    GINT_SetCtrl(GINT0, kGINT_CombineOr, kGINT_TrigEdge, gint_callback);
    GINT_ConfigPins(GINT0, kGINT_Port0, GINT0_POL_MASK, GINT0_ENA_MASK);
    GINT_EnableCallback(GINT0);
    DBG_vPrintf(TRUE, "GINT Configured\n");
}


void gint_callback(void)
{
    DBG_vPrintf(TRUE, "gint_callback: Wake up from button press: 0x%08x\n", POWER_GetIoWakeStatus());
    DBG_vPrintf(TRUE, "gint_callback: GPIO_PinRead: 0x%08x\n", GPIO_PinRead(GPIO, 0, 7));
    DBG_vPrintf(TRUE, "gint_callback: GPIO_PortRead: 0x%08x\n", GPIO_PortRead(GPIO, 0));
}
