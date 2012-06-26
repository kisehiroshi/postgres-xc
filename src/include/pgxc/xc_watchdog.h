#ifndef XC_WATCHDOG_H
#define XC_WATCHDOG_H

#include <unistd.h>
#include "c.h"

/*
 * Xc_watchdog memory structure
 */

typedef struct XcWatchdogInfo
{
	int64	len;
	int64	magic;
	uint64	watchdog_timer;
	int32	arch;				/* 32 or 64 */
	int		shm_id;
	int32	node_category;
	int32	nodename_offset;
	int32	wkdir_offset;
} XcWatchdogInfo;

typedef enum
{
	XCNode_NotUsed = 0,
	XCNode_GTM,
	XCNode_GTM_Proxy,
	XCNode_Coordinator,
	XCNode_Datanode
} XCNodeCategory;

#define XcWatchdogMagic 0xF0E1D2C3B4A59687
#define XcWatchdogModeFlags 0660

#define xcWd_timer(w) ((w)->watchdog_timer)
#define xcWd_arch(w)  ((w)->arch)
#define xcWd_shm_id(w) ((w)->shm_id)
#define xcWd_node_category(w) ((w)->node_category)
#define xcWd_nodename(w) (char *)((char *)(w) + (w)->nodename_offset + sizeof(int32))
#define xcWd_wkdir(w) (char *)((char *)(w) + (w)->wkdir_offset + sizeof(int32))

XcWatchdogInfo *xcWd_initTimer(size_t size, XCNodeCategory category, 
							   char * nodename, char * dirname);
XcWatchdogInfo *xcWd_attachTimer(int shm_id);
int	xcWd_detachTimer(XcWatchdogInfo *xcWatchdog);
bool xcWd_checkTimer(XcWatchdogInfo *watchdog, int shm_id,
					 size_t size, XCNodeCategory category, 
					 char *nodename, char *dirname);
#define xcWd_incrementTimer(w) 	do{(w)->watchdog_timer ++;}while(0)


#endif /* XC_WATCHDOG_H */
