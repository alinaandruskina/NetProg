CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = daytime-client
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o *.out

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test
