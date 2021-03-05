# Application Name
TARGET := objdetector

# Function to recursively find files in directory tree
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Find all .o files compiled from protbuf files
PROTO_O := $(call rwildcard, /axis/tf-proto, *.o)

# Determine the base path
BASE := $(abspath $(patsubst %/,%,$(dir $(firstword $(MAKEFILE_LIST)))))

# Find cpp files
OBJECTS := $(patsubst %.cpp, %.o, $(wildcard $(BASE)/src/*.cpp))

CXX = arm-linux-gnueabihf-g++
STRIP = arm-linux-gnueabihf-strip

CXXFLAGS += -I/usr/include -I/usr/include/grpcpp/security -I/axis/tf-proto -I/axis/opencv/include
CPPFLAGS = -Os -pipe -std=c++17

LDLIBS += -L $(BASE)/lib \
 -L /usr/lib/arm-linux-gnueabihf \
 -L /usr/lib/arm-linux-gnueabihf/grpcpp \
 -L /usr/lib/gcc-cross/arm-linux-gnueabihf \
 -L /axis/opencv/lib \
 -L /axis/tf-proto
LDLIBS += -lm -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio
LDLIBS += -lprotobuf -lz -lgrpc -lgpr -lgrpc++ -lssl -lcrypto -lcares -lprofiler -lrt

# backward compatible for OpenCV 4.2
# skip vdo libs in this case since we use a dummy videoio lib with no vdo dependency when building
ifndef OPENCV_VERSION
$(warning "OPENCV_VERSION is not set. Using OpenCV 4.2")
else
ifneq ($(OPENCV_VERSION),4.2)
LDLIBS += -L /axis/stubs \
 -L /axis/device-api/lib
LDLIBS += -lvdostream -lfido -lcapaxis -lstatuscache
endif
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $< $(CPPFLAGS) $(LDLIBS) $(PROTO_O) -o $@ && $(STRIP) --strip-unneeded $@

clean:
	$(RM) *.o $(TARGET)

