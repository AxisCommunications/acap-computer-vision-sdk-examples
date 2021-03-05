# Application Name
TARGET := capture_app

# Determine the base path
BASE := $(abspath $(patsubst %/,%,$(dir $(firstword $(MAKEFILE_LIST)))))

# Find cpp files
OBJECTS := $(patsubst %.cpp, %.o, $(wildcard $(BASE)/src/*.cpp))

CXX = arm-linux-gnueabihf-gcc
STRIP = arm-linux-gnueabihf-strip

CXXFLAGS += -I/usr/include -I/axis/opencv/include -I/axis/device-api
CPPFLAGS = -Os -pipe -std=c++17

LDLIBS += -L $(BASE)/lib \
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf \
 -L /axis/opencv/lib
LDLIBS += -lm -lstdc++ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio

# backward compatible for OpenCV 4.2
# skip vdo libs in this case since we use a dummy videoio lib with no vdo dependency when building
ifndef OPENCV_VERSION
$(warning "OPENCV_VERSION is not set. Using OpenCV 4.2")
else
ifneq ($(OPENCV_VERSION),4.2)
LDLIBS += \
 -L /axis/device-api/lib \
 -L /axis/stubs
LDLIBS += -lvdostream -lfido -lcapaxis -lstatuscache
endif
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $< $(CPPFLAGS) $(LDLIBS) -o $@ && $(STRIP) --strip-unneeded $@

clean:
	$(RM) *.o $(TARGET)
