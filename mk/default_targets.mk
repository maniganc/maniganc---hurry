
TARGET_DEFAULT ?= debug

COMMON_CFLAGS += -fdump-rtl-expand
COMMON_CFLAGS += -std=gnu11
COMMON_CFLAGS += -Wall -Wshadow -Wimplicit-function-declaration
COMMON_CFLAGS += -Wredundant-decls -Wstrict-prototypes
COMMON_CFLAGS += -fno-common -ffunction-sections -fdata-sections

COMMON_CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes  -fno-common -ffunction-sections -fdata-sections -Wno-unused-parameter

# debug 
debug_CFLAGS ?= $(COMMON_CFLAGS) -g3 -DDEBUG

#release
release_CFLAGS ?= $(COMMON_CFLAGS) -O2

debug: $(PROGRAMS)

release: $(PROGRAMS)

