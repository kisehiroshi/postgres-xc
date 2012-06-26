#include "gtm/gtm_c.h"
#include "gtm/gtm_proxy_watchdog.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define GTM_PROXY_WATCHDOG_FILE	"gtm_proxy_watchdog"
#define GTM_PROXY_WATCHDOG_SIZE	4096
#define GTM_PROXY_MAX_PATH 1024

static XcWatchdogInfo *watchdog = NULL;

bool gtm_proxy_watchdog;
int	gtm_proxy_watchdog_interval;
char gtm_proxy_watchdog_file[GTM_PROXY_MAX_PATH];

extern char *GTMProxyDataDir;
extern char *GTMProxyNodeName;

static void write_watchdog_file(int shm_id);
static int read_watchdog_file(void);

void gtmPxyWd_init(void)
{
	if (gtm_proxy_watchdog == false)
	{
		watchdog = NULL;
		write_watchdog_file(0);
		return;
	}
	watchdog = xcWd_initTimer(GTM_PROXY_WATCHDOG_SIZE, XCNode_GTM_Proxy, GTMProxyNodeName, GTMProxyDataDir);
	if (watchdog == NULL)
	{
		fprintf(stderr, "Could not initialize watchdog area. %s\n", strerror(errno));
		exit(2);
	}
	write_watchdog_file(watchdog->shm_id);
}

void gtmPxyWd_increment(void)
{
	if (!watchdog)
		return;
	watchdog->watchdog_timer ++;
}

void gtmPxyWd_restore(void)
{
	int shm_id;

	shm_id = read_watchdog_file();
	if (shm_id == 0)
	{
		/* No watchdog was used in the previous run */
		gtmPxyWd_init();
	}
	else
	{
		/* Some watchdog may have been used in the previous run */
		if ((watchdog = xcWd_attachTimer(shm_id)) == (XcWatchdogInfo *)(-1))
		{
			/* The watchdog no longer exists */
			gtmPxyWd_init();
		}
		else
		{
			if (xcWd_checkTimer(watchdog, shm_id, GTM_PROXY_WATCHDOG_SIZE, XCNode_GTM_Proxy,
								GTMProxyNodeName, GTMProxyDataDir) == false)
			{
				/* The shared memory might be for other use. */
				xcWd_detachTimer(watchdog);
				watchdog = NULL;
				gtmPxyWd_init();
			}
			/*
			 * Here, continues to use old watchdog timer
			 * This happens when a watcher keeps detection, while GTM
			 * faile/stops and then restarts
			 */
		}
	}
}

void gtmPxyWd_detach(void)
{
	if (watchdog)
		xcWd_detachTimer(watchdog);
	watchdog = NULL;
	write_watchdog_file(0);
}

static void write_watchdog_file(int shm_id)
{
	int wd_fd;

	snprintf(gtm_proxy_watchdog_file, GTM_PROXY_MAX_PATH-1, "%s/%s", GTMProxyDataDir, GTM_PROXY_WATCHDOG_FILE);
	wd_fd = open(gtm_proxy_watchdog_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRUSR);
	if (wd_fd == -1)
	{
		fprintf(stderr, "Failed to create/open watchdog file.\n");
		exit(2);
	}
	write(wd_fd, &shm_id, sizeof(int));
	close(wd_fd);
}

static int read_watchdog_file(void)
{
	int wd_fd;
	int shm_id;

	snprintf(gtm_proxy_watchdog_file, GTM_PROXY_MAX_PATH-1, "%s/%s", GTMProxyDataDir, GTM_PROXY_WATCHDOG_FILE);
	wd_fd = open(gtm_proxy_watchdog_file, O_RDONLY, 0);
	if (wd_fd == -1)
	{
		if (errno != EACCES)
		{
			/*
			 * For the first run, the watchdog file may not exist.
			 */
			fprintf(stderr, "Failed to open watchdog file for read.\n");
			exit(2);
		}
		return(0);
	}
	read(wd_fd, &shm_id, sizeof(int));
	close(wd_fd);
	return(shm_id);
}



