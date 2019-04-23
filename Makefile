CXX     := g++
CFLAGS  := -std=c++14 -Wall -Wextra $(shell pkg-config --cflags libpng freetype2)
LFLAGS  := $(shell pkg-config --libs libpng freetype2)
target  := flamebutter
source  := cpp
header  := hpp
headers := $(wildcard ./src/*.$(header))
sources := $(wildcard ./src/*.$(source))
objects := $(sources:.$(source)=.o)
deps    := $(objects:.o=.deps)

build: format $(objects)
	$(CXX) $(objects) $(LFLAGS) -o $(target)

%.o: %.$(source)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) $(target) $(objects)

format: $(sources) $(headers)
	@for src in $^ ; do \
		var=$$(clang-format "$$src" | diff "$$src" - | wc -l) ; \
		if [ $$var -ne 0 ] ; then \
			echo "$$src didn't respect the coding style (diff: $$var lines)" ; \
			clang-format -style=file -i "$$src"; \
		fi ; \
	done