dnl
dnl This source file is a part of the Berlin Project.
dnl Copyright (C) 1999 Stefan Seefeld <seefelds@magellan.umontreal.ca> 
dnl http://www.berlin-consortium.org
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
dnl BERLIN_GGIMESA_CHECK(mandatory-flag)
dnl
dnl Checks if GGIMesa is found. If it is, $ac_cv_lib_GGIMesa is set to "yes".

AC_DEFUN([BERLIN_GGIMESA_CHECK],[

	AC_LANG_SAVE
	AC_LANG_C

	AC_ARG_WITH(ggimesa-prefix,
		[  --with-ggimesa-prefix=PFX Prefix for GGIMesa],[
		ggimesa_prefix="$withval"])

	dnl Check for GGIMesa includes
	if test x$ggimesa_prefix != x ; then
		GL_INCLUDES=-I$ggimesa_prefix/include
	fi
	save_CPPFLAGS="$CPPFLAGS"
	CPPFLAGS="$GL_INCLUDES $CPPFLAGS"
	AC_CHECK_HEADER(GL/ggimesa.h,,no_gl=yes)
	CPPFLAGS="$save_CPPFLAGS"

	dnl Check for GGIMesa libs
	if test x$no_gl = x ; then

		if test x$gl_eprefix != x ; then
			GL_LIBS=-L$gl_eprefix/lib
		fi
		save_LDFLAGS="$LDFLAGS"
		LDFLAGS="$GL_LIBS $LDFLAGS"
		AC_CHECK_LIB(GL, GGIMesaCreateContext, :, no_gl=yes)
		LDFLAGS="$save_LDFLAGS"
	fi

	if test x$no_gl != x ; then

		ac_cv_lib_GGIMesa=no		
		dnl Abort or warn?
		if test ".$1" = .mandatory ; then
			AC_MSG_ERROR(OpenGL library was not found!)
		else
			AC_MSG_WARN(OpenGL library was not found!)
		fi
	else
		ac_cv_lib_GGIMesa=yes
		GL_LIBS="$GL_LIBS -lGL -lGLU"
	fi

	AC_SUBST(GL_INCLUDES)
	AC_SUBST(GL_LIBS)

	AC_LANG_RESTORE
])