all: main.c manual.c bomb.c bomb_helper.o
	gcc -o main.out main.c
	gcc -o bomb.out bomb.c bomb_helper.o
	gcc -o manual.out manual.c

bomb_helper.o: bomb_helper.c bomb_helper.h
	gcc -c bomb_helper.c bomb_helper.h

run:
	./main.out

clean:
	rm *.out
