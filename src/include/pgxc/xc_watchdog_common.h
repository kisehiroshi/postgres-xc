#infdef XC_WATCHDOG_COMMON_H
#define XC_WATCHDOG_COMMON_H

#include <unistd.h>
#include "c.h"

typedef struct xc_watchdog_info
{
	int64	len;
	int32	magic;
	int32	node_type;
	int64	timer;
	int32	nodename_offset;
	int32	dir_offset;
} xc_watchdog_info;

typedef struct xc_watchdog_name
{
	int32	len;
	char	name[0];
} xc_watchdog_name;

#endif /* XC_WATCHDOG_COMMON_H */
