TARGET  := main
CC      := gcc
CFLAGS  := --std=gnu99 -D_GNU_SOURCE -Wall -g -O0
LDFLAGS := -lreadline
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)
DEPS    := $(SRCS:.c=.dep)

all: $(TARGET)

$(TARGET): $(OBJS)

%.o: %.dep

%.dep: %.c Makefile
	$(CC) $(CFLAGS) -MM -MF $@ $<

clean:
	rm -rf $(TARGET) $(OBJS) $(DEPS)

.PHONY: clean

-include $(DEPS)

