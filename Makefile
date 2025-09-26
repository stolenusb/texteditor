-include Makefile.config

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
BIN = texto
LDFLAGS = -lncurses
CXXFLAGS = -Wall -Wextra -std=c++17 -Isrc/include

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

install:
	@mkdir -p $(PREFIX)/
	cp $(BIN) $(PREFIX)/texto
	echo "Installed to $(PREFIX)/texto"

clean:
	rm -rf build/ bin/