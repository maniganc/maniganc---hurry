
target ?= debug
TARGET_DEFAULT = $(target)

PROGRAMS = maniganc

maniganc_BUILD_DIR=.build
maniganc_C_SRCS = maniganc.c $(wildcard MgCore/*.c) 
maniganc_C_SRCS += $(sort $(wildcard MgStd/*/*.c) MgStd/buildin_procedures.c)
maniganc_C_INCLUDES = MgCore/include MgStd lib/std/include lib/debug/include 

include ../mk/default_targets.mk
include ../mk/makeprog.mk
include ../mk/build_info.mk

# automatically make MgStd procedure header
$(maniganc_OBJS):: MgStd/buildin_procedures.h

MgStd/buildin_procedures.h: $(filter-out MgStd/buildin_procedures.c,$(wildcard MgStd/*/*.c))
	make -C MgStd

clean-MgStd:
	make -C MgStd clean

clean: clean-MgStd


