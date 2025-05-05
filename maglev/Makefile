# compiler
CXX = g++

# compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# target executable
TEST_TARGET = test_maglev

# source files
TEST_SOURCES = test.cpp forwarder.cpp

# header files
HEADERS = forwarder.h

# default target - build and run test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# build the test executable
$(TEST_TARGET): $(TEST_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES)

# clean build files
clean:
	rm -f $(TEST_TARGET)

.PHONY: test clean