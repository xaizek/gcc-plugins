GCC_PLUGIN_DIR := $(shell gcc -print-file-name=plugin)
ifeq ($(GCC_PLUGIN_DIR),)
    $(error Failed to discover GCC plugin directory)
endif

NAME := $(notdir $(abspath .))

CXX := g++
CXX_FLAGS := -std=c++11 -fno-rtti -fPIC -O3 -I$(GCC_PLUGIN_DIR)/include
LD_FLAGS := -shared -Wl,--version-script,version.map

SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)

$(NAME).so $(OBJ): Makefile

$(NAME).so: $(OBJ) version.map
	$(CXX) $(LD_FLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(NAME).so
