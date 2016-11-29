
target ?= debug
TARGET_DEFAULT = $(target)

PROGRAMS = maniganc

maniganc_BUILD_DIR=.build
maniganc_C_SRCS = interactive.c $(wildcard core/*.c) 
maniganc_C_SRCS += $(sort $(wildcard buildin/*.c) buildin/buildin_procedures.c)
maniganc_C_INCLUDES = core/include buildin lib/std/include lib/debug/include 

include ../mk/default_targets.mk
include ../mk/makeprog.mk
include ../mk/build_info.mk

# automatically make buildin procedure header
$(maniganc_OBJS):: buildin/buildin_procedures.h

buildin/buildin_procedures.h: $(filter-out buildin/buildin_procedures.c,$(wildcard buildin/*.c))
	make -C buildin

clean-buildin:
	make -C buildin clean

clean: clean-buildin


