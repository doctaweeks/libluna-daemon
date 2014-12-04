#ifndef LIBLUNA_DAEMON_H_
#define LIBLUNA_DAEMON_H_

int daemonize(const char *name);
int switch_user(int uid, int gid);

#endif
