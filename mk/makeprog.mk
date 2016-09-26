
# PROGRAMS : list of final executable names
# nameprog_C_SRCS :  objs
# nameprog_LIBS : libs
# nameprog_LDFLAGS : LDFLAGS
# nameprog_CFLAGS : CFLAGS
# nameprog_TARGET : target
# nameprog_BUILD_DIR : output dir

.PHONY: all

all: $(PROGRAMS)

define MAKEPROG_TEMPLATE
# default build dir
ifeq ($(strip $$($(1)_BUILD_DIR)),)
$(1)_BUILD_DIR = build
endif

# default target 
ifeq ($(strip $$($(1)_TARGET)),)
$(1)_TARGET = $$(TARGET_DEFAULT)
endif

# final build dir
$(1)_BUILD_DIR_F = $$($(1)_BUILD_DIR)/$(1)/$$($(1)_TARGET)

# create obj paths
$(1)_OBJS = $$(addprefix $$($(1)_BUILD_DIR_F)/,$$($(1)_C_SRCS:.c=.o))

## run executable
run_$(1): $(1)
	./$$($(1)_BUILD_DIR_F)/$(1)

## build executable
# final ldflags
$(1)_LDFLAGS_F = $$($(1)_LDFLAGS) $$($$($(1)_TARGET)_LDFLAGS)
# rule
# forward rule (override default with an useless command)
$(1): $$($(1)_BUILD_DIR_F)/$(1)
	@true
$$($(1)_BUILD_DIR_F)/$(1): $$($(1)_OBJS) $$($(1)_LIBS)
	@mkdir -p $$(@D)/
	$(CC) -o $$@ $$($(1)_LDFLAGS_F) $$($(1)_OBJS) $$(addprefix -l,$$($(1)_LIBS))

## compile c files
# final cflags
$(1)_CFLAGS_F = $$($(1)_CFLAGS) \
                $$($$($(1)_TARGET)_CFLAGS) \
                $$(BUILD_INFO_CFLAGS) \
                $$(addprefix -I,$$($(1)_C_INCLUDES))
# rule
$$($(1)_OBJS):: $$($(1)_BUILD_DIR_F)/%.o:%.c
	@mkdir -p $$(@D)/
	$(CC) -c -o $$@ $$($(1)_CFLAGS_F) $$<

# save objects name for cleaning 
ALL_OBJS += $$($(1)_OBJS)

# save final program path for 'clean' rule
ALL_PROGRAMS += $$($(1)_BUILD_DIR_F)/$(1)

endef

# generate build rules for each program
$(foreach prog,$(PROGRAMS),$(eval $(call MAKEPROG_TEMPLATE,$(prog))))

clean:
	rm -f $(ALL_OBJS) $(ALL_PROGRAMS)


