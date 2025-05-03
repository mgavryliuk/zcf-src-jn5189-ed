#ifndef _BOARD_H_
#define _BOARD_H_

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define CLOCK_32MfXtalIecLoadpF_x100 (600)   /* 6.0pF */
#define CLOCK_32MfXtalPPcbParCappF_x100 (20) /* 0.2pF */
#define CLOCK_32MfXtalNPcbParCappF_x100 (40) /* 0.4pF */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
