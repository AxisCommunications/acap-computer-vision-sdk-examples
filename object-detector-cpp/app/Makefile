# Application Name
TARGET := objdetector

# Function to recursively find files in directory tree
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Find all .o files compiled from protbuf files
PROTO_O := $(call rwildcard, /axis/tfproto, *.o)

# Determine the base path
BASE := $(abspath $(patsubst %/,%,$(dir $(firstword $(MAKEFILE_LIST)))))

# Find cpp files
OBJECTS := $(patsubst %.cpp, %.o, $(wildcard $(BASE)/src/*.cpp))

CXX = $(TARGET_TOOLCHAIN)-g++
CXXFLAGS += -I/usr/include -I/usr/include/grpcpp/security -I/axis/tfproto -I/axis/openblas/usr/include -I/axis/opencv/usr/include -I/build-root/usr/include
CPPFLAGS = --sysroot=/build-root $(ARCH_CFLAGS) -Os -pipe -std=c++17
STRIP=$(TARGET_TOOLCHAIN)-strip

LDLIBS += -L $(BASE)/lib \
 -L /axis/opencv/usr/lib \
 -L /axis/openblas/usr/lib
LDLIBS += -lm -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopenblas -lgfortran
LDLIBS += -lprotobuf -lz -lgrpc -lgpr -lgrpc++ -lssl -lcrypto -lcares -lprofiler -lrt
LDLIBS += -lvdostream -lfido -lcapaxis -lstatuscache

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $< $(CPPFLAGS) $(LDLIBS) $(PROTO_O) -o $@ && $(STRIP) --strip-unneeded $@

clean:
	$(RM) *.o $(TARGET)
