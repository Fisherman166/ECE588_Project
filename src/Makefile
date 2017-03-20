CC= gcc
source= main.c join.c
depend= join.h common.h
libs= -lpthread -lrt
all:
	$(CC) -std=gnu99  -O3 -Wall  -o database_join $(source) $(libs)
	
run:
	perl run_proj.pl
	
clean:
	rm database_join
