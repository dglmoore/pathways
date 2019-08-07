TARGET=bin/pathways
SOURCES=$(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -Iinclude -O3 -o $@ $^

clean:
	rm $(TARGET)

.PHONY: clean
