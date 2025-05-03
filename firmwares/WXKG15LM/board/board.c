#include <stdint.h>

#include "fsl_clock.h"
#include "fsl_debug_console.h"

#include "board.h"

/**
 * @brief Set up and initialize all required blocks and functions related to the board hardware.
 */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = CLOCK_GetFreq(kCLOCK_Xtal32M);
    DbgConsole_Init(0U, 115200U, kSerialPort_Uart, uartClkSrcFreq);
}
