#ifndef APP_PWR_H
#define APP_PWR_H

#ifdef DEBUG_APWR
#define TRACE_PWR TRUE
#else
#define  TRACE_PWR FALSE
#endif

// #define MAXIMUM_TIME_TO_SLEEP_SEC (60 * 60)
#define MAXIMUM_TIME_TO_SLEEP_SEC (10 * 1)

void vAppScheduleActivity(void);

extern bool_t bWaked;

#endif /* APP_PWR_H */
