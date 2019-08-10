TARGETS=bin/string bin/mystring

all: bin/string bin/mystring

bin/%: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^

clean:
	rm $(TARGETS)

.PHONY: clean
