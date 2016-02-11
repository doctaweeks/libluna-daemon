#ifndef LIBLUNA_DAEMON_H_
#define LIBLUNA_DAEMON_H_

#include <sys/types.h>

int daemonize(const char *name);
int switch_user(uid_t uid, gid_t gid);

#endif
