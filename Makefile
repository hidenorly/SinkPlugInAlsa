# compiler env.
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	CXX=ccache clang++
	LDLIBS=-ldl -lasound
	CXXFLAGS=-std=c++2a -MMD -MP -Wall -fPIC
	SHARED_CXXFLAGS= -fPIC -shared
endif
ifeq ($(UNAME),Darwin)
	CXX=ccache clang++
	LDLIBS=-stdlib=libc++
	CXXFLAGS=-std=c++2a -MMD -MP -Wall
#	SHARED_CXXFLAGS= -flat_namespace -dynamiclib
	SHARED_CXXFLAGS=-dynamiclib
endif

LDFLAGS=-pthread

# project config
EXO_DIR ?= ./src
AFW_DIR=../audioframework
INC_DIR=$(AFW_DIR)/include
LIB_DIR=$(AFW_DIR)/lib
LIB_SINK_DIR=$(LIB_DIR)/sink-plugin
OBJ_DIR=./out

# --- source code config --------------
EXO_SRCS = $(wildcard $(EXO_DIR)/*.cpp)

# --- the object files config --------------
EXO_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(EXO_SRCS:.cpp=.o)))

# --- Build for sink example(shared) ------------
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	EXO_SO_TARGET = $(LIB_SINK_DIR)/libsink_alsa.so
	AFW_SO_TARGET = $(LIB_DIR)/libafw.so
endif
ifeq ($(UNAME),Darwin)
	EXO_SO_TARGET = $(LIB_SINK_DIR)/libsink_alsa.dylib
	AFW_SO_TARGET = $(LIB_DIR)/libafw.dylib
endif
EXO_DEPS = $(EXO_OBJS:.o=.d)

default: $(EXO_SO_TARGET)
.PHONY: default

$(EXO_SO_TARGET): $(EXO_OBJS)
	@[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)
	@[ -d $(LIB_SINK_DIR) ] || mkdir -p $(LIB_SINK_DIR)
	$(CXX) $(LDFLAGS) $(SHARED_CXXFLAGS) $(EXO_OBJS) -o $@ $(LDLIBS) $(AFW_SO_TARGET)

$(EXO_OBJS): $(EXO_SRCS)
	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $(EXO_DIR)/$(notdir $(@:.o=.cpp)) -o $@

-include $(EXO_DEPS)

# --- clean up ------------------------
clean:
	rm -f $(EXO_OBJS) $(EXO_SO_TARGET)
