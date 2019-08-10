TARGETS=bin/string bin/mystring bin/iterable

all: $(TARGETS)

bin/scaling: src/scaling.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^ -lmgl

bin/%: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^

clean:
	rm $(TARGETS)

.PHONY: clean
