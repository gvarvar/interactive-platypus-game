CPP=clang++
#CPP=g++

BUILD_DIR := ./build
TEST_DIR := ./tests

# -Wall turns on all warnings, -c means compile as obj

# TODO: Some shit to change the flags like make debug and make release
# Release (level 3 optimise, strip data): -O3 -s
# Debug ( -g3 -ggdb )
CFLAGS := -std=c++23 -m64 -pthread -Wall -g3 -ggdb -c
# CFLAGS := -Wall -Werror -std=c++14 -O

# mkdir
ifeq ($(OS),Windows_NT)
$(shell if not exist build mkdir build)
RM := del build\* /Q
else
$(shell mkdir -p $(BUILD_DIR))
RM := rm -rf $(BUILD_DIR) platypus
endif

all: platypus

debug: CFLAGS += -g3 -Og 
debug: odrive_can
	valgrind --leak-check=full --track-origins=yes ./odrive_can < ./tests/complex.env

test: odrive_can
	./test.sh

#SRCS=main.cpp BlockerGame.cpp Board.cpp HumanPlayer.cpp Player.cpp
SRCS=$(wildcard *.cpp)

# This converts the list of main.cpp,blocker.cpp to main.o and blocker.o so dont have to edit 1e13 sgit
OBJS=$(addprefix $(BUILD_DIR)/,$(subst .cpp,.o,$(SRCS)))

# dependency checkings
deps := $(patsubst %.o,%.d,$(OBJS))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

platypus: $(OBJS)
	$(CPP) $^ -o platypus

$(BUILD_DIR)/%.o: %.cpp
	$(CPP) $(CFLAGS) $< $(DEPFLAGS) -c -o $@

clean:
	$(RM)
