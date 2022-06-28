CXX				:= g++
INCLUDE_DIRS	:= -I./include
CXX_FLAGS		:= -g -O3 $(INCLUDE_DIRS)

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= exec

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

init:
	mkdir -p $(BIN)
	mkdir -p $(SRC)
	mkdir -p $(INCLUDE)
	mkdir -p $(LIB)

exec: $(BIN)/$(EXECUTABLE)

run: exec
	@echo "\n\nExecutando aplicação...\n\n"
	$(BIN)/$(EXECUTABLE)
	@echo "\n\nEncerrando aplicação...\n\n"

clean:
	@echo "\nRemovendo executavel...\n"
	rm $(BIN)/$(EXECUTABLE)
	@echo "\nRemovido com sucesso...\n"
