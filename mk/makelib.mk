
.PHONY: all

all: $(LIBRAIRIES)

define MAKELIB_TEMPLATE
# default build dir
ifeq ($(strip $$($(1)_BUILD_DIR)),)
$(1)_BUILD_DIR = build
endif

# default target 
ifeq ($(strip $$($(1)_TARGET)),)
$(1)_TARGET = $$(TARGET_DEFAULT)
endif

# final build dir
$(1)_BUILD_DIR_F = $$($(1)_BUILD_DIR)/$$($(1)_TARGET)

# create obj paths
$(1)_OBJS = $$(addprefix $$($(1)_BUILD_DIR_F)/,$$($(1)_C_SRCS:.c=.o))

## build lib
# forward rule (override it with an useless command)
$(1): $$($(1)_BUILD_DIR_F)/$(1).a
	@true
$$($(1)_BUILD_DIR_F)/$(1).a: $$($(1)_OBJS)
	@mkdir -p $$($(1)_BUILD_DIR_F)
	ar rcs $$@ $$($(1)_OBJS)

## compile c files
# final cflags
$(1)_CFLAGS_F = $$($(1)_CFLAGS) \
                $$($$($(1)_TARGET)_CFLAGS) \
                $$(BUILD_INFO_CFLAGS) \
                $$(addprefix -I,$$($(1)_C_INCLUDE))
# rule
$$($(1)_OBJS): $$($(1)_BUILD_DIR_F)/%.o:%.c
	@mkdir -p $$($(1)_BUILD_DIR_F)
	$(CC) -c -o $$@ $$($(1)_CFLAGS_F) $$<

# save objects name for cleaning 
ALL_OBJS += $$($(1)_OBJS)

# save final program path for 'clean' rule
ALL_LIBS += $$($(1)_BUILD_DIR_F)/$(1).a
endef

# generate build rules for each program
$(foreach lib,$(LIBRAIRIES),$(eval $(call MAKELIB_TEMPLATE,$(lib))))

clean:
	rm -f $(ALL_OBJS) $(ALL_LIBS)


