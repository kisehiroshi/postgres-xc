#ifndef NODE_WATCHDOG_H
#define NODE_WATCHDOG_H

#include "pgxc/xc_watchdog.h"

extern bool xc_watchdog;
extern int xc_watchdog_interval;

extern void nodeWd_init(void);
extern void nodeWd_increment(void);
extern void nodeWd_restore(void);
extern void nodeWd_detach(void);
extern void nodeWd_setStatus(XCNodeStatus status);

#endif /* NODE_WATCHDOG_H */
