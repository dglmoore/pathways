TARGETS=bin/string bin/mystring bin/iterable

all: $(TARGETS)

bin/%: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^

clean:
	rm $(TARGETS)

.PHONY: clean
