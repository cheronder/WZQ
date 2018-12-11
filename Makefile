VER = debug

SRCS   = $(notdir $(wildcard src/*.cpp))
OBJS   = $(SRCS:%.cpp=obj/%.o)
DEPS   = $(SRCS:%.cpp=obj/%.d)
LIB    = wzq
TARGET = $(LIB:%=lib%.a)

CXX      = g++
AR       = ar

CXXFLAGS = -Wall
INCFLAGS = -Iinclude
LDFLAGS  = -L. -l$(LIB)
ARFLAGS  = -cru

ifeq ($(VER), debug)
	CXXFLAGS += -O0 -g -pg -DDEBUG
	LDFLAGS  += -pg
else
	CXXFLAGS += -O3 -DNDEBUG
	LDFLAGS  += -static
endif

.PHONY : all clean cleanall
all : $(TARGET)

test : obj/test.o $(TARGET)
	$(CXX) $< $(LDFLAGS) -o $@

$(TARGET) : $(filter obj/WZQ%.o, $(OBJS))
	$(AR) $(ARFLAGS) $@ $^

-include $(DEPS)
obj/%.o : src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

obj/%.d : src/%.cpp
	set -e; mkdir -p obj; rm -f $@; \
	$(CXX) -MM $< $(INCFLAGS) > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$

clean :
	rm -rf obj

cleanall :
	rm -rf test $(TARGET) obj