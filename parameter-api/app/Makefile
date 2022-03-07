# Application Name
TARGET := parameter

# Paths
OUT_PATH ?= $(CURDIR)
API_PATH := $(CURDIR)/apis
SRC_PATH := $(CURDIR)/src
GRPC_CPP_PLUGIN := grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= $(shell which $(GRPC_CPP_PLUGIN))

# Function to recursively find files in directory tree
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Find cpp files
OBJECTS := $(patsubst %.cpp, %.o, $(wildcard $(SRC_PATH)/*.cpp))

# Build files
PROTOBUF_FILES := $(call rwildcard, $(API_PATH),*.proto)
PROTOBUF_H := $(patsubst %.proto,%.pb.h,$(patsubst $(API_PATH)%,$(SRC_PATH)%,$(PROTOBUF_FILES)))
PROTOBUF_O := $(patsubst %.pb.h,%.pb.o,$(PROTOBUF_H))
PROTOBUF_GRPC_O := $(patsubst %.pb.h,%.grpc.pb.o,$(PROTOBUF_H))

CXX = $(TARGET_TOOLCHAIN)-g++
CXXFLAGS += -I/usr/include/grpcpp/security -I/build-root/usr/include
CPPFLAGS = --sysroot=/build-root $(ARCH_CFLAGS) -Os -pipe -std=c++17
STRIP=$(TARGET_TOOLCHAIN)-strip

# Compiler flags
LDLIBS += -lprotobuf -lgrpc -lgpr -lgrpc++ 
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(PROTOBUF_H) $(PROTOBUF_O) $(PROTOBUF_GRPC_O) $(OBJECTS)
	$(CXX) $< $(CPPFLAGS) $(PROTOBUF_O) $(PROTOBUF_GRPC_O) $(OBJECTS) $(LDLIBS) -o $@ && $(STRIP) --strip-unneeded $@

# Protobuf object files
%.pb.o: %.pb.cc
	$(CXX) -c $(CXXFLAGS) -I$(OUT_PATH) $^ -o $@

# Generate protobuf gRPC source files
$(SRC_PATH)/%.grpc.pb.cc $(SRC_PATH)/%grpc.pb.h: $(API_PATH)/%.proto |
	protoc $(PKG_CONFIG_CFLAGS_I) \
	-I$(API_PATH) \
	--grpc_out=$(SRC_PATH) \
	--plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

# Generate protobuf source files
$(SRC_PATH)/%.pb.cc $(SRC_PATH)/%.pb.h: $(API_PATH)/%.proto | 
	protoc $(PKG_CONFIG_CFLAGS_I) -I$(API_PATH) --cpp_out=$(SRC_PATH) $<

clean:
	$(RM) *.o $(TARGET)
