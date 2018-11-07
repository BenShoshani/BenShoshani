Project:main.o functions.o
	gcc -o Project main.o functions.o
main.o: main.c header1.h
	gcc -c main.c
functions.o: functions.c header1.h
	gcc -c functions.c	
