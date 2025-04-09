CXX=g++
CXXFLAGS  = -Wall -g
SRC = $(wildcard src/*.cc)
OBJ = $(SRC:src/%.cc=bin/%.o)
EXEC = bin/gbcemu

$(shell mkdir -p bin)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

bin/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(EXEC)