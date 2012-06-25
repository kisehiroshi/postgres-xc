#ifndef GTM_WATCHDOG_H
#define GTM_WATCHDOG_H
#include "pgxc/xc_watchdog.h"

void gtmWd_init(void);
void gtmWd_increment(void);
void gtmWd_restore(void);

#endif /* GTM_WATCHDOG_H */
