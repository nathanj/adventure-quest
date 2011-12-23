CC=gcc
CFLAGS=--std=gnu99 -D_GNU_SOURCE -Wall -g -O0
LDFLAGS=-lreadline

all: main

main: main.o armor.o

