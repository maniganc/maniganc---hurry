
check_valgrind ?= 0
ifneq ($(check_valgrind),0)
opt_check_valgrind = --check-valgrind 1
endif


checker=./pyranoya.py $(opt_check_valgrind) --test 

all: all-test

tests=$(wildcard MgStd/*/*.scm)

all-test: $(tests) build-maniganc
	$(checker) $(tests)

build-maniganc:
	make -C ..
