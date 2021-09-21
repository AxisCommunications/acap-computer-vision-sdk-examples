# Application Name
TARGET := capture_app

# Determine the base path
BASE := $(abspath $(patsubst %/,%,$(dir $(firstword $(MAKEFILE_LIST)))))

# Find cpp files
OBJECTS := $(patsubst %.cpp, %.o, $(wildcard $(BASE)/src/*.cpp))

CXX = arm-linux-gnueabihf-gcc
STRIP = arm-linux-gnueabihf-strip

CXXFLAGS += -I/usr/include -I/axis/opencv/usr/include
CPPFLAGS = -Os -pipe -std=c++17

LDLIBS += -L $(BASE)/lib \
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf \
 -L /axis/opencv/usr/lib -L /axis/openblas/usr/lib
LDLIBS += -lm -lstdc++ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio
LDLIBS += -lvdostream -lfido -lcapaxis -lstatuscache -lopenblas

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $< $(CPPFLAGS) $(LDLIBS) -o $@ && $(STRIP) --strip-unneeded $@

clean:
	$(RM) *.o $(TARGET)
