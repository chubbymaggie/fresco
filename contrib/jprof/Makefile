#! gmake
#
# The contents of this file are subject to the Mozilla Public License
# Version 1.1 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License
# at http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and
# limitations under the License.
#
# The Initial Developer of the Original Code is Kipp E.B. Hickman.
#
# Autoconf version of original Makefile
# Fri Sep 24 23:44:10 PDT 1999 <mcafee@netscape.com>
#

MKSHLIB   = g++ -fPIC -shared
MKPROGRAM = g++

DEPTH		= ../..
topsrcdir	= ../..
srcdir		= .
VPATH		= .

#include $(DEPTH)/config/autoconf.mk

PROGRAM = jprof

CPPSRCS = \
	bfd.cpp    \
	coff.cpp   \
	elf.cpp	   \
	leaky.cpp  \
	strset.cpp \
	intcnt.cpp \
	$(NULL)

LIBS = \
	-lbfd \
	-liberty \
	-ldl \
	$(NULL)

# Stuff to build the library used to wrap malloc
LIBMALLOC_CPPSRCS = libmalloc.cpp
LIBMALLOC_OBJECTS = $(LIBMALLOC_CPPSRCS:.cpp=.o)
LIBMALLOC = libjprof.so

OBJECTS = $(CPPSRCS:.cpp=.o)

EXPORTS         = \
                jprof.h \
                $(NULL)

EXPORTS         := $(addprefix $(srcdir)/, $(EXPORTS))


# include $(topsrcdir)/config/config.mk

OTHER_LIBRARIES = $(LIBMALLOC)
TARGETS := $(PROGRAM) $(OTHER_LIBRARIES)

#include $(topsrcdir)/config/rules.mk

# Make sure all depends on files that rules.mk doesn't know about.
all:: $(PROGRAM) $(OTHER_LIBRARIES)

# Make sure install depends on files that rules.mk doesn't know about.
install:: $(OTHER_LIBRARIES)

# Make sure libs depends on files that rules.mk doesn't know about.
libs:: $(OTHER_LIBRARIES)

clobber::
	rm -f $(LIBMALLOC_OBJECTS)
	rm -f $(LIBMALLOC)

clean::
	rm -f $(LIBMALLOC_OBJECTS) $(OBJECTS)

$(LIBMALLOC): $(LIBMALLOC_OBJECTS)
	rm -f $@
	$(MKSHLIB) -o $@ $(LIBMALLOC_OBJECTS)

$(PROGRAM): $(OBJECTS)
	rm -f $@
	$(MKPROGRAM) -o $@ $(OBJECTS) $(LIBS)

test:
	@echo LIBMALLOC = $(LIBMALLOC)
	@echo TARGETS = $(TARGETS)

libs::
	$(INSTALL) -m 555 $(OTHER_LIBRARIES) $(DIST)/lib
	$(INSTALL) -m 555 $(OTHER_LIBRARIES) $(DIST)/bin
