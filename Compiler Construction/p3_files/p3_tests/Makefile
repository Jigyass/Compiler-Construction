TESTFILES := $(wildcard *.cmm)
TESTS := $(TESTFILES:.cmm=.test)

.PHONY: all

all: $(TESTS)

%.test:
	@rm -f $*.unparse $*.err
	@touch $*.unparse $*.err
	@echo "TEST $*"
	@../cmmc $*.cmm -u $*.unparse 2> $*.err ;\
	PROG_EXIT_CODE=$$?;\
	if [ $$PROG_EXIT_CODE != 0 ]; then \
		echo "cmmc error:"; \
		cat $*.err; \
		exit 1; \
	fi; \
	diff -B --ignore-all-space $*.unparse $*.unparse.expected; \
	STDOUT_DIFF_EXIT=$$?;\
	diff -B --ignore-all-space $*.err $*.err.expected; \
	STDERR_DIFF_EXIT=$$?;\
	FAIL=$$(($$STDOUT_DIFF_EXIT || $$STDERR_DIFF_EXIT));\
	exit $$FAIL || echo "All tests passed"

clean:
	rm -f *.unparse *.err
