
DIRNAME=`basename $(PWD)`

CHECKER = ./test/simpleUnitTest.sh
CHECKED = ./scheme

TEST_0 = $(sort $(wildcard test/tests_step0/*.scm))

debug:
	@make -C src maniganc
	@cp src/build/maniganc/debug/maniganc ./scheme

release:
	@make -C src maniganc target=release
	@cp src/build/maniganc/release/maniganc ./scheme

clean:
	@make -C src clean
	@make -C src clean target=release

test-0: release
	$(CHECKER) -e $(CHECKED) $(TEST_0)

.PHONY: debug release test-all all

tarball : 
	make clean 
	cd .. && tar -czvf scheme-`whoami`-`date +%d-%m-%H-%M`.tgz $(DIRNAME) && cd $(DIRNAME) && mv ../scheme-`whoami`-*.tgz .

