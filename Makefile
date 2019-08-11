TARGETS=bin/string bin/mystring bin/iterable

all: $(TARGETS)

bin/scaling: cmd/scaling.cpp cmd/args.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^ -lmgl

bin/entropy: cmd/entropy.cpp cmd/args.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^ -lmgl

bin/%: cmd/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -O3 -o $@ $^

clean:
	rm $(TARGETS)

.PHONY: clean
