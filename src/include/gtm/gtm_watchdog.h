#ifndef GTM_WATCHDOG_H
#define GTM_WATCHDOG_H
#include "pgxc/xc_watchdog.h"

extern void gtmWd_init(void);
extern void gtmWd_increment(void);
extern void gtmWd_restore(void);
extern void gtmWd_detach(void);

#endif /* GTM_WATCHDOG_H */
