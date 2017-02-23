CC= gcc
source= main.c join.c
depend= join.h common.h
libs= -lpthread -lrt
all:
	$(CC) -std=gnu99 -Wall -o main $(source) $(libs)
clean:
	rm main
