#ifndef XC_WATCHDOG_H
#define XC_WATCHDOG_H

#include <unistd.h>
#include "c.h"
#include "postgres.h"
#include "utils/guc.h"

extern bool xc_watchdog;
extern int	xc_watchdog_interval;

#endif /* XC_WATCHDOG_H */
