TARGET=build/pathways_unittest
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=build/obj/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -g -pg -o $@ $^

build/obj/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -std=c++17 -Wall -Wextra -pedantic -g -pg -I../include -c -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	@rm -rf build

.PHONY: clean
