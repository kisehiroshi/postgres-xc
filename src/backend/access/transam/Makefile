#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for access/transam
#
# IDENTIFICATION
#    src/backend/access/transam/Makefile
#
#-------------------------------------------------------------------------

subdir = src/backend/access/transam
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = clog.o transam.o varsup.o xact.o xlog.o xlogutils.o rmgr.o slru.o subtrans.o multixact.o twophase.o twophase_rmgr.o gtm.o

include $(top_srcdir)/src/backend/common.mk

# ensure that version checks in xlog.c get recompiled when catversion.h changes
xlog.o: xlog.c $(top_srcdir)/src/include/catalog/catversion.h

libpg-fe.h:
	$(LN_S) $(top_builddir)/contrib/gtm/client/libpg-fe.h $(top_srcdir)/src/include/
