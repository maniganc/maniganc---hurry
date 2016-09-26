
CHECKER = ./test/simpleUnitTest.sh
CHECKED = ./scheme

TEST_0 = $(sort $(wildcard test/tests_step0/*.scm))

debug:
	@make -C src maniganc
	@cp src/build/maniganc/debug/maniganc ./scheme

release:
	@make -C src maniganc target=release
	@cp src/build/maniganc/release/maniganc ./scheme

test-0: release
	$(CHECKER) -e $(CHECKED) $(TEST_0)

.PHONY: debug release test-all all
