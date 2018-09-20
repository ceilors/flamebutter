CXX=g++
CFLAGS=-std=c++14 -Wall $(shell pkg-config --cflags libpng)
LFLAGS=$(shell pkg-config --libs libpng)
app=flamebutter
objects := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

all: $(objects)
	$(CXX) $(objects) $(LFLAGS) -o $(app)
$(objects): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
clean:
	-$(RM) $(app) $(objects)