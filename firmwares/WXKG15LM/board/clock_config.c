#include "clock_config.h"

void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

void BOARD_BootClockRUN(void)
{
    CLOCK_EnableAPBBridge();                        /*!< The Async_APB clock is enabled. */
    CLOCK_EnableClock(kCLOCK_Xtal32M);     /*!< Enable XTAL 32 MHz output */
    /*!< Configure RTC OSC */
    CLOCK_EnableClock(kCLOCK_Fro32k);     /*!< Enable RTC FRO 32 KHz output */
    CLOCK_AttachClk(kFRO32K_to_OSC32K_CLK);                  /*!< Switch OSC32K to FRO32K */

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gint);
    // CLOCK_EnableClock(kCLOCK_Adc0);
    // CLOCK_EnableClock(kCLOCK_Usart0);

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kXTAL32M_to_MAIN_CLK);                 /*!< Switch MAIN_CLK to XTAL32M */
    CLOCK_AttachClk(kMAIN_CLK_to_ASYNC_APB);                 /*!< Switch ASYNC_APB to MAIN_CLK */
    CLOCK_AttachClk(kXTAL32M_to_ADC_CLK);                 /*!< Switch ADC_CLK to XTAL32M */
    CLOCK_AttachClk(kXTAL32M_to_OSC32M_CLK);                 /*!< Switch OSC32M_CLK to XTAL32M */
    CLOCK_AttachClk(kOSC32M_to_USART_CLK);                 /*!< Switch USART_CLK to OSC32M */
    CLOCK_AttachClk(kOSC32K_to_WDT_CLK);                 /*!< Switch WDT_CLK to OSC32K */

    /*!< Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;
}
