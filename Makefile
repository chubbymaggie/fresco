# Berlin User Environment Makefile
#
# Please do not edit this file directly. All configuration should be
# changed in config/<OS>.mk

# This let's us export all variables across Makefiles. Special variables
# which should not be exported are unexported in rules.mk in the /config
# directory.

TOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
export TOPDIR

SUBDIRS = src

include $(TOPDIR)/config/rules.mk
