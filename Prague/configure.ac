dnl $Id$
dnl
dnl This source file is a part of the Fresco Project.
dnl Copyright (C) 2000 Stefan Seefeld <stefan@fresco.org> 
dnl http://www.fresco.org/
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Library General Public
dnl License as published by the Free Software Foundation; either
dnl version 2 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public
dnl License along with this library; if not, write to the
dnl Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
dnl MA 02139, USA.
dnl
dnl Process this file with autoconf to produce a configure script.

dnl ------------------------------------------------------------------
dnl Autoconf initialization
dnl ------------------------------------------------------------------
AC_PREREQ(2.53)
AC_REVISION($Revision$)
AC_INIT(Prague, M2, devel@fresco.org)
AC_CONFIG_SRCDIR(LICENSE)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADER(include/Prague/acconfig.hh:config/acconfig.hh.in)

AC_CANONICAL_HOST

AC_PROG_MAKE_SET
FRESCO_PROG_CC_CXX
FRESCO_FLAG_SO
FRESCO_CHECK_OPTIMIZE
FRESCO_CHECK_DEBUGGING

dnl ------------------------------------------------------------------
dnl Environment (this needs to go before AC_LANG(C++)
dnl ------------------------------------------------------------------

AC_LANG(C)
AC_HEADER_STDC
AC_CHECK_FUNCS(getenv putenv setenv unsetenv)
AC_LANG(C++)

dnl ------------------------------------------------------------------
dnl General flags
dnl ------------------------------------------------------------------

AC_ARG_ENABLE(tracer,
              AC_HELP_STRING([--enable-tracer],[Compile with tracer support]),
              [AC_DEFINE(TRACER, 1, [Define if you want tracer support.])])

AC_MSG_NOTICE([platform specific system utilities])
AC_CHECK_FUNCS(strsignal)
AC_CHECK_HEADERS(string.h)
AC_CHECK_DECLS([strsignal])
AC_HEADER_SYS_WAIT

AC_SEARCH_LIBS(recv, [socket resolv])

AC_CHECK_LIB(z, gzsetparams, AC_DEFINE(HAVE_ZLIB, 1, [Define if you have the zlib library.]), [AC_MSG_ERROR([update your zlib, please])])

dnl ------------------------------------------------------------------
dnl Thread support
dnl ------------------------------------------------------------------

AC_MSG_NOTICE([Thread Implementation])
FRESCO_THREAD

dnl ------------------------------------------------------------------
dnl Plugin support
dnl ------------------------------------------------------------------

AC_MSG_NOTICE([Dynamic Loading Implementation])
AC_CHECK_HEADERS([dlfcn.h dl.h])
AC_CHECK_LIB(dl, dlopen)
AC_LANG(C)
AC_CHECK_FUNCS([dlopen dlsym dlclose])
AC_CHECK_FUNCS([shl_load shl_unload shl_findsym])
AC_LANG(C++)

dnl ------------------------------------------------------------------
dnl IPC support
dnl ------------------------------------------------------------------

AC_MSG_NOTICE([IPC Implementation])
AC_CHECK_HEADERS([sys/socket.h select.h sys/select.h netinet/in_systm.h netinet/ip.h])
AC_CHECK_HEADERS([pty.h sys/stropts.h libutil.h])
AC_CHECK_HEADERS([arpa/inet.h sys/un.h])
AC_CHECK_TYPES([socklen_t],
   [ AC_DEFINE_UNQUOTED(HAVE_SOCKLEN_T, 1, [defined if we have a socklen_t]) ],
   [
     AC_MSG_CHECKING([for socklen_t equivalent])
     prague_socklen_t_equiv=""
     for arg2 in "struct sockaddr" void; do
         for t in int size_t unsigned long "unsigned long"; do
             AC_TRY_COMPILE([
                  #ifdef HAVE_SYS_TYPES_H
                  #include <sys/types.h>
                  #endif
                  #ifdef HAVE_SYS_SOCKET_H
                  #include <sys/socket.h>
                  #endif

                  int getpeername (int, $arg2 *, $t *);
               ],[
                  $t len;
                  getpeername(0,0,&len);
               ],[
                  prague_socklen_t_equiv="$t"
                  break
               ]
             )
         done
     done

     if test ".$prague_socklen_t_equiv" = "." ; then
         AC_MSG_ERROR([Cannot find a type to use in place of socklen_t])
     fi

     AC_MSG_RESULT($prague_socklen_t_equiv)
     AC_DEFINE_UNQUOTED(SOCKLEN_T_EQUIV, $prague_socklen_t_equiv,
			[type to use in place of socklen_t if not defined])
   ],
   [#include <sys/types.h>
#include <sys/socket.h>])
AC_SEARCH_LIBS(socket, [socket])
AC_SEARCH_LIBS(inet_aton, [socket resolv])
AC_SEARCH_LIBS(login, [util])
AC_LANG(C)
AC_CHECK_FUNCS([openpty _getpty])
AC_LANG(C++)

no_dev_ptmx=1

if test -z "$no_dev_ptmx" ; then
  AC_CHECK_FILE("/dev/ptmx", [AC_DEFINE(HAVE_DEV_PTMX, 1, [Define if you have /dev/ptmx.]) have_dev_ptmx=1])
fi
AC_CHECK_FILE("/dev/ptc", [AC_DEFINE(HAVE_DEV_PTS_AND_PTC, 1, [Define if you have /dev/ptc.]) have_dev_ptc=1])

dnl ------------------------------------------------------------------
dnl Output substitution
dnl ------------------------------------------------------------------

AC_MSG_NOTICE([Output Substitution])

AC_CONFIG_FILES([config/local.mk config/synopsis.py])
AC_CONFIG_FILES([config/Prague-config], [chmod +x config/Prague-config])
AC_CONFIG_FILES([config/Prague.spec])
AC_CONFIG_FILES([bin/Prague-config:config/Prague-build-config.in], [chmod +x bin/Prague-config])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_FILES([demo/Makefile demo/Sys/Makefile demo/Thread/Makefile demo/IPC/Makefile
                 demo/Filter/Makefile demo/Network/Makefile])

mkdir -p lib
mkdir -p bin

AC_OUTPUT
