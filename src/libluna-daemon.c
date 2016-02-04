#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cap-ng.h>
#include <string.h>
#include <stdio.h>

#include "libluna-daemon.h"

int switch_user(int uid, int gid)
{
	capng_clear(CAPNG_SELECT_BOTH);
	return capng_change_id(uid, gid, CAPNG_INIT_SUPP_GRP | CAPNG_CLEAR_BOUNDING);
}

#define BUF_SIZE 100

static int create_pid_file(const char *name)
{
	char *path;
	char buf[BUF_SIZE];
	int fd;
	int ret;
	int len;

	const char *base = "/var/run";

	//TODO: replace / with \0
	len = 2 * strlen(name) + strlen(base) + 7;
	path = malloc(len);
	if (path == NULL)
		return -1;

	snprintf(path, len, "%s/%s/%s.pid", base, name, name);

	fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	free(path);

	if (fd < 0)
		return fd;

	ret = ftruncate(fd, 0);
	if (ret < 0) {
		close(fd);
		return ret;
	}

	snprintf(buf, BUF_SIZE, "%ld\n", (long) getpid());
	len = strlen(buf);
	ret = write(fd, buf, len);
	if (ret != len) {
		close(fd);
		return ret;
	}

	return fd;
}

int daemonize(const char *name)
{
	int ret;
	pid_t pid, sid;

	pid = fork();
	if (pid < 0)
		return pid;
	if (pid > 0)
		exit(EXIT_SUCCESS);

	sid = setsid();
	if (sid < 0)
		return sid;

	//setup_signal_handler();

	pid = fork();
	if (pid < 0)
		return pid;
	if (pid > 0)
		exit(EXIT_SUCCESS);

	/* always succeeds */
	umask(0);
	
	ret = chdir(getenv("HOME"));
	if (ret < 0) {
		ret = chdir("/");
		if (ret < 0)
			return ret;
	}

	// NOTE: if other file descriptors are used close them here
	ret = close(STDIN_FILENO);
	if (ret < 0)
		return ret;

	ret = close(STDOUT_FILENO);
	if (ret < 0)
		return ret;

	ret = close(STDERR_FILENO);
	if (ret < 0)
		return ret;

	ret = create_pid_file(name);
	if (ret < 0)
		return ret;

	close(ret);

	return 0;
}
