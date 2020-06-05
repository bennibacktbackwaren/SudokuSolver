CXX		  := gcc
# Uncomment the subsystem argument to remove the console on windows
CXX_FLAGS := -Wall -Wextra -Wpedantic -std=c99 -O2 #-Wl,--subsystem,windows

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -lSDL2 -lSDL2_ttf
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
