##################################################
#							-- Configuration --								 #
##################################################

CC = g++
ECHO = echo
RM = rm -f

SRC = src
BIN = bin
INCLUDE = -I src/include

EXECUTABLE = parse327
SOURCES = runner.cpp FileParser.cpp
CLEAN = $(EXECUTABLE) $(BIN) submissions/ .tags* CHANGELOG

CFLAGS = -Wall -Werror -ggdb -funroll-loops -std=c++11
LDFLAGS = -lncurses

##################################################
#							 -- Processing -- 								 #
##################################################

OBJS = $(SOURCES:%.cpp=$(BIN)/%.o)
SRC_FILES = $(SOURCES:%=$(SRC)/%)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN)/%.o: $(SRC)/%.cpp
	@$(ECHO) Compiling $<
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(BIN)/$*.o

clean:
	@$(ECHO) Removing all generated files.
	@$(RM) -r $(CLEAN)
