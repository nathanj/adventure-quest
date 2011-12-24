TARGET  := main
CC      := gcc
CFLAGS  := --std=gnu99 -D_GNU_SOURCE -Wall -g -O0 -MMD
LDFLAGS := -lreadline
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)
DEPS    := $(wildcard *.d)

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)

.PHONY: clean

ifneq ($(DEPS),)
include $(DEPS)
endif

