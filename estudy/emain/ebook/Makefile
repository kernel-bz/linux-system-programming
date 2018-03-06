#---------------------------------------------------------------------
#	File:		/ebook/Makefile
#	Author:		정재준(rgbi3307@nate.com)
#	최초코딩일: 2009-04-22
#	최근수정일: 2009-04-22
#---------------------------------------------------------------------

COMPILER	= mipsel-linux-
KERNELDIR   = /home/jungjj/mips/kernel/linux-2.6.21/
INCLUDEDIR  = ../../include/
LIBDIR	    = ../../lib/

TARGET		= ebook.bin
TARGET2		= ../../bin/ebook

OBJS		= ebook.o word_page.o display.o
OBJS	   += $(LIBDIR)devio.a
OBJS	   += $(LIBDIR)ds.a
OBJS	   += $(LIBDIR)fileio.a

SRCS		= $(OBJS:.o=.c)

#---------------------------------------------------------------------
INCLUDE	    = -I.  -I../include
INCLUDE    += -I$(KERNELDIR)include
INCLUDE    += -I$(INCLUDEDIR)devio
INCLUDE    += -I$(INCLUDEDIR)ds
INCLUDE    += -I$(INCLUDEDIR)fileio
INCLUDE    += -I$(INCLUDEDIR)ebook

#LIBDIRS   += -L$(KERNELDIR)lib

CC_FLAGS	= $(INCLUDE) -W -Wall -O2
#LD_FLAGS	= 
LIBS        = -lm -lpng12 -ljpeg -lz -lpthread

#---------------------------------------------------------------------
CC			= $(COMPILER)gcc
CPP			= $(COMPILER)g++
AR			= $(COMPILER)ar
AR2			= $(COMPILER)ar rcv  
RANLIB		= $(COMPILER)ranlib
LD			= $(COMPILER)ld
NM			= $(COMPILER)nm
STRIP		= $(COMPILER)strip
OBJCOPY		= $(COMPILER)objcopy
CP			= cp
MV			= mv

#--------------------------------------------------------------------
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)
	$(CP)  $(TARGET) $(TARGET2)

#
# 컴파일 실행
#	
%.o:%.c
	@echo "C Compiling $< ..."
	$(CC) -c $(CC_FLAGS) -o $@ $< 
	@echo "------------------------------------------------------------"


#--------------------------------------------------------------------
dep:
	$(CC) -M $(INCLUDE) $(SRCS) > .depend

clean:
	rm -f *.o
	rm -f $(TARGET)

distclean: clean
	rm -f $(TARGET2)
	rm -rf .depend	

new : 
	$(MAKE) clean 
	$(MAKE) 

ifeq (.depend,$(wildcard .depend))
include .depend
endif

