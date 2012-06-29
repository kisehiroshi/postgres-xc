/*
#include "gtm/gtm_c.h"
*/
#include "c.h"
#include "gtm/gtm_watchdog.h"
#include "gtm/elog.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define GTM_WATCHDOG_FILE	"gtm_watchdog"
#define GTM_MAX_PATH 1024

static XcWatchdogInfo *watchdog = NULL;

bool gtm_watchdog;
int	gtm_watchdog_interval;
char gtm_watchdog_file[GTM_MAX_PATH];

extern char *GTMDataDir;
extern char *NodeName;

static void write_watchdog_file(int shm_id);
static int read_watchdog_file(void);

void gtmWd_init(void)
{
	if (gtm_watchdog == false)
	{
		watchdog = NULL;
		write_watchdog_file(0);
		return;
	}
	watchdog = xcWd_initTimer(XCNode_GTM, NodeName, GTMDataDir);
	if (watchdog == NULL)
	{
		fprintf(stderr, "Could not initialize watchdog area. %s\n", strerror(errno));
		exit(2);
	}
	write_watchdog_file(watchdog->shm_id);
	elog(LOG, "GTM watchdog initialized, interval %d", gtm_watchdog_interval);
}

void gtmWd_increment(void)
{
	if (!watchdog)
		return;
	watchdog->watchdog_timer ++;
}

void gtmWd_restore(void)
{
	int shm_id;

	shm_id = read_watchdog_file();
	if (shm_id == 0)
	{
		/* No watchdog was used in the previous run */
		gtmWd_init();
	}
	else
	{
		/* Some watchdog may have been used in the previous run */
		if ((watchdog = xcWd_attachTimer(shm_id)) == (XcWatchdogInfo *)(-1))
		{
			/* The watchdog no longer exists */
			gtmWd_init();
		}
		else
		{
			if (xcWd_checkTimer(watchdog, shm_id, XCNode_GTM,
								NodeName, GTMDataDir) == false)
			{
				/* The shared memory might be for other use. */
				xcWd_detachTimer(watchdog);
				watchdog = NULL;
				gtmWd_init();
			}
			/*
			 * Here, continues to use old watchdog timer
			 * This happens when a watcher keeps detection, while GTM
			 * fails/stops and then restarts
			 */
		}
	}
	elog(LOG, "GTM watchdog restored, interval %d", gtm_watchdog_interval);
}

void gtmWd_detach(void)
{
	if (watchdog)
		xcWd_detachTimer(watchdog);
	watchdog = NULL;
	write_watchdog_file(0);
	elog(LOG, "GTM watchdog finished.");
}


static void write_watchdog_file(int shm_id)
{
	int wd_fd;

	snprintf(gtm_watchdog_file, GTM_MAX_PATH-1, "%s/%s", GTMDataDir, GTM_WATCHDOG_FILE);
	wd_fd = open(gtm_watchdog_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if (wd_fd == -1)
	{
		fprintf(stderr, "Failed to create/open watchdog file. %s\n", gtm_watchdog_file);
		exit(2);
	}
	write(wd_fd, &shm_id, sizeof(int));
	close(wd_fd);
}

static int read_watchdog_file(void)
{
	int wd_fd;
	int shm_id;

	snprintf(gtm_watchdog_file, GTM_MAX_PATH-1, "%s/%s", GTMDataDir, GTM_WATCHDOG_FILE);
	wd_fd = open(gtm_watchdog_file, O_RDONLY, 0);
	if (wd_fd == -1)
	{
		if (errno != ENOENT)
		{
			/*
			 * For the first run, the watchdog file may not exist.
			 */
			fprintf(stderr, "Failed to open watchdog file for read. %s\n", gtm_watchdog_file);
			exit(2);
		}
		return(0);
	}
	read(wd_fd, &shm_id, sizeof(int));
	close(wd_fd);
	return(shm_id);
}

