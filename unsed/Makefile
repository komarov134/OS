all : CFLAGS = -Wall
all : main

all-shared : CFLAGS = -Wall
all-shared : main-shared

main-shared : libreadlines.so readlines-main.c readlines.h
	gcc $(CFLAGS) -L. -Wl,-rpath=. -o readlines-main-shared readlines-main.c -lreadlines

libreadlines.so : libreadlines.o
	gcc -shared -o libreadlines.so libreadlines.o

libreadlines.o : readlines.c readlines.h
	gcc $(CFLAGS) -c -fpic readlines.c -o libreadlines.o

debug : CFLAGS = -Wall -g
debug : main

main : readlines.o readlines-main.o
	gcc -o readlines-main readlines.o readlines-main.o

readlines.o : readlines.c readlines.h
	gcc $(CFLAGS) -c readlines.c

readlines-main.o : readlines-main.c readlines.h
	gcc $(CFLAGS) -c readlines-main.c

clean : 
	rm -f readlines.o readlines-main.o libreadlines.o
