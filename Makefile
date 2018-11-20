CC = g++
CFLAGS = -g -O2 -c -Wall -std=c++0x

LDFLAGS = -g -Wall 
EXE = File

#HDR = src/graphics.h src/easygl_constants.h

CPP_FILES:= $(wildcard src/*.cpp)
OBJ_FILES:= $(patsubst src/%.cpp,bin/%.o,$(CPP_FILES))

CFLAGS += $(shell pkg-config --cflags freetype2) # evaluates to the correct include flags for the freetype headers

default: all

all: bin $(OBJ_FILES) $(EXE)

bin:
	mkdir -p bin

bin/%.o : src/%.cpp
	$(CC) $(CFLAGS) $(INC) -o $@ $<

$(EXE) : $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $(EXE)

clean:
	rm -r bin
