
DIRNAME=`basename $(PWD)`

CHECKER = ./test/simpleUnitTest.sh

TEST_0 = $(sort $(wildcard test/tests_step0/*.scm))
TEST_1_SIMPLE = $(sort $(wildcard test/tests_step1/simple/*.scm))
TEST_1_EVOLVED = $(sort $(wildcard test/tests_step1/evolved/*.scm))
TEST_2 = $(sort $(wildcard test/tests_step2/*.scm))

debug:
	@make -C src maniganc
	@cp src/.build/maniganc/debug/maniganc ./scheme

release:
	@make -C src maniganc target=release
	@cp src/.build/maniganc/release/maniganc ./scheme

clean:
	@make -C src clean
	@make -C src clean target=release

test-0: release
	$(CHECKER) -e ./scheme-parse $(TEST_0)

test-1-simple: release
	$(CHECKER) -e ./scheme-parse $(TEST_1_SIMPLE)

test-1-evolved: release
	$(CHECKER) -e ./scheme-parse $(TEST_1_EVOLVED)

test-2: release
	$(CHECKER) -e ./scheme-file $(TEST_2)

.PHONY: debug release test-all all

tarball : 
	make clean 
	cd .. && tar -czvf scheme-`whoami`-`date +%d-%m-%H-%M`.tgz $(DIRNAME) && cd $(DIRNAME) && mv ../scheme-`whoami`-*.tgz .

