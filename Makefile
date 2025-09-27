-include Makefile.config

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
BIN = bin/texto
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
	@mkdir -p $(PREFIX)/bin
	cp $(BIN) $(PREFIX)/bin/texto
	echo "Installed to $(PREFIX)/bin/texto"

clean:
	rm -rf build/ bin/