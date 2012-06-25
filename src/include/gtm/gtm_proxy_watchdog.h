#ifndef GTM_WATCHDOG_H
#define GTM_WATCHDOG_H
#include "pgxc/xc_watchdog.h"

void gtmPxyWd_init(void);
void gtmPxyWd_increment(void);
void gtmPxyWd_restore(void);

#endif /* GTM_WATCHDOG_H */
