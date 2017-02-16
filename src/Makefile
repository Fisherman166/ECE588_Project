CC= gcc
source= main.c parallel_join.c serial_join.c
depend= parallel_join.h serial_join.h
libs= -lpthread -lrt
all:
	$(CC) -std=gnu99 -Wall -o main $(source) $(libs)
#$(CC) -std=gnu99 $(libs) $(source)  $(depend) -o join
