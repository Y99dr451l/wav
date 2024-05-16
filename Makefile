ifeq ($(OS),Windows_NT)
	CXX = g++
	RM = del
else
	CXX = x86_64-w64-mingw32-g++
	RM = rm
endif
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O3
LDFLAGS = 

SRC = main.cpp wav.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = wav.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

debug: CXXFLAGS += -g
debug: $(EXEC)

clean:
	$(RM) $(OBJ) $(EXEC)