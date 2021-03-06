dnl $Id$
dnl
dnl This source file is a part of the Fresco Project.
dnl Copyright (C) 1999, 2000 Stefan Seefeld <stefan@fresco.org> 
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

AC_PREREQ(2.52)
AC_REVISION($Revision$)
AC_INIT(Fresco, M2, devel@fresco.org)
AC_CONFIG_SRCDIR(LICENSE)
AC_CONFIG_AUX_DIR(config)

echo "The Fresco Windowing System,
Copyright (c) 2000 The Fresco Project <devel@fresco.org>
"

AC_ARG_ENABLE(ggi,
              AC_HELP_STRING([--enable-ggi],[Configure with ggi support]),
              [config_ggi="$enableval"],[config_ggi="yes"])
AC_ARG_ENABLE(sdl,
              AC_HELP_STRING([--enable-sdl],[Configure with sdl support]),
              [config_sdl="$enableval"],[config_sdl="yes"])
AC_ARG_ENABLE(cxx-demos,
              AC_HELP_STRING([--enable-cxx-demos],[Configure c++ demos support]),
              [config_cxx_demos="$enableval"],[config_cxx_demos="yes"])
AC_ARG_ENABLE(python-runtime,
              AC_HELP_STRING([--enable-python-runtime],[Configure python runtime support]),
              [config_python_runtime="$enableval"],[config_python_runtime="yes"])
AC_ARG_ENABLE(python-demos,
              AC_HELP_STRING([--enable-python-demos],[Configure python demos support]),
              [config_python_demos="$enableval"],[config_python_demos="yes"])

if test -d $srcdir/Fresco-Test; then
  AC_CONFIG_SUBDIRS(Fresco-Test)
fi
if test -d $srcdir/Prague; then
  AC_CONFIG_SUBDIRS(Prague)
fi
if test -d $srcdir/Babylon; then
  AC_CONFIG_SUBDIRS(Babylon)
fi
if test -d $srcdir/Fresco-IDL; then
  AC_CONFIG_SUBDIRS(Fresco-IDL)
fi
if test -d $srcdir/Fresco-C++; then
  AC_CONFIG_SUBDIRS(Fresco-C++)
fi
if test -d $srcdir/Berlin; then
  AC_CONFIG_SUBDIRS(Berlin)
fi
if test -d $srcdir/GGI -a ".$config_ggi" = ".yes"; then
  AC_CONFIG_SUBDIRS(GGI)
else
  test -d GGI && rm -f GGI/Makefile
fi
if test -d $srcdir/SDL -a ".$config_sdl" = ".yes"; then
  AC_CONFIG_SUBDIRS(SDL)
else
  test -d SDL && rm -rf SDL/Makefile
fi
if test -d $srcdir/Fresco-Python -a ".$config_python_runtime" = ".yes"; then
  AC_CONFIG_SUBDIRS(Fresco-Python)
else
  test -d Fresco-Python && rm -f Fresco-Python/Makefile
fi
if test -d $srcdir/Fresco-Java -a ".$config_java_runtime" = ".yes"; then
  AC_CONFIG_SUBDIRS(Fresco-Java)
else
  test -d Fresco-Java && rm -f Fresco-Java/Makefile
fi
if test -d $srcdir/Fresco-Perl -a ".$config_perl_runtime" = ".yes"; then
  AC_CONFIG_SUBDIRS(Fresco-Perl)
else
  test -d Fresco-Perl && rm -f Fresco-Perl/Makefile
fi
if test -d $srcdir/Fresco-C++-demos -a ".$config_cxx_demos" = ".yes"; then
  AC_CONFIG_SUBDIRS(Fresco-C++-demos)
else
  test -d Fresco-C++-demos && rm -f Fresco-C++-demos/Makefile
fi
if test -d $srcdir/Fresco-Python-demos -a ".$config_python_demos" = ".yes"; then
  AC_CONFIG_SUBDIRS(Fresco-Python-demos)
else
  test -d Fresco-Python-demos && rm -f Fresco-Python-demos/Makefile
fi
if test -d $srcdir/Documentation; then
  AC_CONFIG_SUBDIRS(Documentation)
fi
if test -d $srcdir/contrib/daVinci; then
  AC_CONFIG_SUBDIRS(contrib/daVinci)
fi

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([config/synopsis.py])
AC_OUTPUT
