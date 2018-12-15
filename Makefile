export VER = release
export CXX = g++
export AR  = ar

SRCS     = $(wildcard src/*.cpp)
OBJS     = $(SRCS:src/%.cpp=obj/%.o)
DEPS     = $(SRCS:src/%.cpp=obj/%.d)

TARGET   = lib/libwzq.a

CXXFLAGS = -Wall -MMD
INCFLAGS = -Iinclude
ARFLAGS  = -cr

ifeq ($(VER), debug)
	CXXFLAGS += -O0 -g -pg -DDEBUG
	LDFLAGS  += -pg
else
	CXXFLAGS += -O3 -DNDEBUG
	LDFLAGS  += -static
endif

.PHONY : all clean cleanall test
all : $(TARGET)

test : $(TARGET)
	make -C test

$(TARGET) : $(OBJS) | lib
	$(AR) $(ARFLAGS) $@ $(OBJS)

lib obj :
	mkdir -p $@

-include $(DEPS)
obj/%.o : src/%.cpp | obj
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c -o $@ $<

clean :
	make -C test clean
	rm -rf obj

cleanall :
	make -C test cleanall
	rm -rf lib obj