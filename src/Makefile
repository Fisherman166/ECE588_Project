CC= gcc
source= main.c join.c
depend= join.h common.h
libs= -lpthread -lrt
all:
	$(CC) -std=gnu99 -Wall -o database_join $(source) $(libs)
clean:
	rm database_join
