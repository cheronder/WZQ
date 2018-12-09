all : test
.PHONY : all

SRCS = $(wildcard WZQ*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CXXFLAGS = -Wall -O2 -g -pg
CXXC = g++
test : $(OBJS) test.o
	$(CXXC) $(CXXFLAGS) $^ -o $@
test.o : test.cpp
	$(CXXC) -c $(CXXFLAGS) $^ -o $@

$(OBJS) : %.o : %.cpp %.h
	$(CXXC) -c $(CXXFLAGS) $< -o $@

clean :
	rm -rf test *.o
.PHONY : clean

