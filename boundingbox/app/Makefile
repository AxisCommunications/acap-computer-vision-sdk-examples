BINS := bbox_example
OBJS := bbox_example.o

PKGS = bbox
PKG_CFLAGS := $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags $(PKGS))
PKG_LDLIBS := $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs $(PKGS))

CXXFLAGS += -I$(LIB_DIR)
CXXFLAGS += -Ofast
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += $(PKG_CFLAGS)
CXXFLAGS += $(CXXFLAGS-y)

LDFLAGS += -Ofast
LDFLAGS += -L$(LIB_DIR) -shared
LDLIBS  += $(PKG_LDLIBS)

all: $(BINS)

bbox_example: $(OBJS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJS) $(LDLIBS)

clean:
	-$(RM) $(BINS) *.o *~
