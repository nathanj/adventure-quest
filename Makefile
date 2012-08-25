TARGET  := main
CC      := gcc
CFLAGS  := --std=gnu99 -D_GNU_SOURCE -Wall -Wextra -Werror -g -O0 -MMD
LDFLAGS := -lncurses -lm
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)
DEPS    := $(SRCS:.c=.d)

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)

.PHONY: clean

sinclude $(DEPS)
