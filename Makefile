CPP=clang++
#CPP=g++

BUILD_DIR := ./build
TEST_DIR := ./tests

CFLAGS := -Wall -Werror -std=c++23 -Ofast

# mkdir
ifeq ($(OS),Windows_NT)
$(shell if not exist build mkdir build)
RM := del build\* /Q
else
$(shell mkdir -p $(BUILD_DIR))
RM := rm -rf $(BUILD_DIR) platypus
endif

all: platypus

SRCS=$(wildcard *.cpp)

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
