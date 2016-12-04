
checker=./pyranoya.py --test

all: all-test

all-test: conditional.scm.test misc.scm.test environment.scm.test

%.scm.test: %.scm
	$(checker) $<


