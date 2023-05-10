TARGET = hello_world
OBJ = src/$(TARGET).o

CXXFLAGS += -I/usr/include -I/axis/opencv/usr/include
CPPFLAGS = -Os -pipe -std=c++17

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $< $(CPPFLAGS) -o $@ && $(STRIP) --strip-unneeded $@

clean:
	$(RM) *.o $(TARGET)
