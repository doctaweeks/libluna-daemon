AUTOMAKE_OPTIONS = foreign

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = src/libluna-daemon.pc
DISTCLEANFILES = $(pkgconfig_DATA)

include_HEADERS=src/libluna-daemon.h

lib_LTLIBRARIES=libluna-daemon.la
libluna_daemon_la_SOURCES=src/libluna-daemon.c
libluna_daemon_la_CPPFLAGS=$(libcap_ng_CFLAGS)
libluna_daemon_la_LIBADD=$(libcap_ng_LIBS)
