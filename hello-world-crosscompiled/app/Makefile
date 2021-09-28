TARGET = hello_world
OBJ = src/$(TARGET).o

CXX = arm-linux-gnueabihf-gcc
STRIP = arm-linux-gnueabihf-strip

CXXFLAGS += -I/usr/include -I/axis/opencv/usr/include
CPPFLAGS = -Os -pipe -std=c++17

BASE := $(abspath $(patsubst %/,%,$(dir $(firstword $(MAKEFILE_LIST)))))
LDLIBS += -L $(BASE)/lib \
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf \
 -L /axis/opencv/usr/lib
LDLIBS += -lm -lstdc++ -lopencv_core

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $< $(CPPFLAGS) $(LDLIBS) -o $@ && $(STRIP) --strip-unneeded $@

clean:
	$(RM) *.o $(TARGET)
