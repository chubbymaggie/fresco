dnl $Id$
dnl
dnl This source file is a part of the Berlin Project.
dnl Copyright (C) 2000 H�vard Skinnemoen <skinnemo@itk.ntnu.no>
dnl http://www.berlin-consortium.org/
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

dnl This adds support for loading/storing configure parameters to a file.
dnl (like $top/config/config.param), to avoid the need for passing the
dnl same parameters to various configure scripts in the tree.
dnl
dnl Usage: AC_CONFIG_LOAD(file [, fatal])
dnl 	-- loads configuration parameters from `file'. If fatal == 1, abort
dnl 	   if `file' is not found
AC_DEFUN([AC_CONFIG_LOAD],[
	if test ! -f $1 ; then
		ifelse($2,,:,AC_MSG_ERROR(Configuration parameter file $1 not found))
	else
		. $1
	fi
])

dnl Usage: AC_CONFIG_STORE(file)
dnl 	-- stores configuration parameters into `file'
AC_DEFUN([AC_CONFIG_STORE],[
	if test ! ".$ac_config_param_vars" = . ; then
		echo "# Automatically generated by configure -- DO NOT EDIT!" > $1
		echo >> $1
		for key in $ac_config_param_vars ; do
                        eval the_value=\$$key
			echo $key=\"$the_value\" >> $1
		done
	else
		AC_MSG_WARN(No configuration parameters registered!)
	fi
])

dnl Usage: AC_REGISTER_PARAM(name)
dnl 	-- registers a variable for storage
AC_DEFUN([AC_REGISTER_PARAM],[
	ac_config_param_vars="$ac_config_param_vars $1"
])