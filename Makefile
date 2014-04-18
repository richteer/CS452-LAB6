CC=gcc

main: main.c init_stuff.o
	${CC} $^ -lGL -lGLEW -lglut -lSOIL -g -o $@

init_stuff.o: init_stuff.c init_stuff.h
	${CC} $< -g -c -o $@
