SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=udpserver
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	gcc -o $@ $^ 
.PHONY:clean
clean:
	rm -rf $(OBJS) $(ELF)
