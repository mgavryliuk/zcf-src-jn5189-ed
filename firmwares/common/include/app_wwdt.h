#ifndef APP_WWDT_H
#define APP_WWDT_H

#ifdef DEBUG_AWWDT
#define TRACE_WWDT TRUE
#else
#define  TRACE_WWDT FALSE
#endif

PUBLIC void vAppSetupWWDT(void);

#endif  /* APP_WWDT_H */
