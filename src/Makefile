
target ?= debug
TARGET_DEFAULT = $(target)

PROGRAMS = maniganc test

maniganc_C_SRCS = interactive.c $(wildcard core/*.c)
maniganc_C_INCLUDES = core/include std/include debug/include

test_C_SRCS = test.c core/vector_instanced.c
test_C_INCLUDES = core/include std/include debug/include

include ../mk/default_targets.mk
include ../mk/makeprog.mk
include ../mk/build_info.mk


# autocompletion with irony on emacs, using .clang_complete
# + flycheck
# + semantic
CLANG_CFLAGS := $(addprefix -I, $(abspath $(maniganc_C_INCLUDES))) \
                $($(maniganc_TARGET)_CFLAGS)
PAR_O :=(
PAR_C :=)
project.el:
	echo "(setq irony--compile-options \
	(list \
$(addsuffix "\"", $(addprefix "\"",$(CLANG_CFLAGS))) \
	))" > project.el

	echo "(setq flycheck-clang-include-path \
	(list \
$(addsuffix "\"", $(addprefix "\"",$(abspath $(maniganc_C_INCLUDES)))) \
	))" >> project.el

	echo " \
$(addprefix $(PAR_O)semantic-add-system-include \", $(addsuffix \"$(PAR_C)\n,$(abspath $(maniganc_C_INCLUDES)))) \
	" >> project.el


.PHONY: project.el

