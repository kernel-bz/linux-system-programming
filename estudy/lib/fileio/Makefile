#---------------------------------------------------------------------
#	File:		/lib/fileio/Makefile
#	Author:		정재준(rgbi3307@nate.com)
#	최초코딩일: 2009-05-28
#	최근수정일: 2009-05-28
#---------------------------------------------------------------------

COMPILER	= mipsel-linux-
KERNELDIR   = /home/jungjj/mips/kernel/linux-2.6.21/

TARGET		= lib_fileio.a
TARGET2		= ../fileio.a

OBJS		= sound.o
#OBJS	   += gx.o  gxbdf.o

SRCS		= $(OBJS:.o=.c)

#---------------------------------------------------------------------
INCLUDE	    = -I.  -I../../include/fileio/
INCLUDE   += -I$(KERNELDIR)include
LIBDIRS	    = 
#LIBDIRS   += -L$(KERNELDIR)lib

CC_FLAGS	= $(INCLUDE) -W -Wall -O2
LD_FLAGS	= $(LIBDIRS)
LIBS        = -lpthread

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
#
# 라이브러리 생성(TARGET --> TARGET2)
#
$(TARGET): $(OBJS)
	$(AR2) $(TARGET) $(OBJS)
	$(CP)  $(TARGET) $(TARGET2)
	@echo "------------------------------------------------------------"
	
#
# 컴파일 실행
#
#%.o:%.S
#	@echo "ASM Compiling $< ..."
#	$(CC) -c $(CC_FLAGS) -D__ASSEMBLY__  -o $@ $< 
#	@echo "------------------------------------------------------------"
	
%.o:%.c
	@echo "C Compiling $< ..."
	$(CC) -c $(CC_FLAGS) -o $@ $< 
	@echo "------------------------------------------------------------"
	
#%.o:%.cc
#	@echo "C++ Compiling $< ..."
#	$(CC) -c $(CC_FLAGS) -o $@ $< 
#	@echo "------------------------------------------------------------"


#--------------------------------------------------------------------
dep:
	$(CC) -M $(INCLUDE) $(SRCS) > .depend

clean:
	rm -f *.o
#	rm -f *.s
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
