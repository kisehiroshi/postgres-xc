#ifndef GTM_PROXY_WATCHDOG_H
#define GTM_PROXY_WATCHDOG_H
#include "pgxc/xc_watchdog.h"

extern bool gtm_proxy_watchdog;
extern int gtm_proxy_watchdog_interval;

extern void gtmPxyWd_init(void);
extern void gtmPxyWd_increment(void);
extern void gtmPxyWd_restore(void);
extern void gtmPxyWd_detach(void);

#endif /* GTM_PROXY_WATCHDOG_H */
