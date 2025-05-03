#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

static void BOARD_InitUart0Pins(void);
static void BOARD_InitLedPins(void);
static void BOARD_InitButtonPins(void);

void BOARD_InitBootPins(void)
{
    BOARD_InitUart0Pins();
    BOARD_InitLedPins();
    BOARD_InitButtonPins();
}

static void BOARD_InitUart0Pins(void)
{
    /* PORT0 PIN8 (coords: 11) is configured as USART0_TXD */
    IOCON_PinMuxSet(IOCON, 0U, 8U, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    /* PORT0 PIN9 (coords: 12) is configured as USART0_RXD */
    IOCON_PinMuxSet(IOCON, 0U, 9U, IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
}

static void BOARD_InitLedPins(void)
{
    /* PORT0 PIN0 (coords: 3) is configured as PIO0_0; left led */
    IOCON_PinMuxSet(IOCON, 0U, 7U, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    /* PORT0 PIN15 (coords: 18) is configured as PIO0_15; right led */
    IOCON_PinMuxSet(IOCON, 0U, 10U, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);

    gpio_pin_config_t led_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U};
    /* Initialize GPIO functionality on pin PIO0_0 (pin 3)  */
    GPIO_PinInit(GPIO, 0U, 7U, &led_config);
    /* Initialize GPIO functionality on pin PIO0_15 (pin 18)  */
    GPIO_PinInit(GPIO, 0U, 10U, &led_config);
}

static void BOARD_InitButtonPins(void)
{
    /* PORT0 PIN12 (coords: 15) is configured as PIO0_12 - left btn */
    IOCON_PinMuxSet(IOCON, 0U, 15U, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);
    /* PORT0 PIN14 (coords: 17) is configured as PIO0_14 - right btn */
    IOCON_PinMuxSet(IOCON, 0U, 14U, IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGITAL_EN);

    gpio_pin_config_t button_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_12 (pin 15)  */
    GPIO_PinInit(GPIO, 0U, 15U, &button_config);
    /* Initialize GPIO functionality on pin PIO0_14 (pin 17)  */
    GPIO_PinInit(GPIO, 0U, 14U, &button_config);
}
