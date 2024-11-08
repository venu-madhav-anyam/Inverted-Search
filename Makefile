inverted_search.out : main.o functions.o 
	gcc -o inverted_search.out main.o functions.o

main.o : main.c
	gcc -c main.c

functions.o : functions.c 
	gcc -c	functions.c

clean :
	rm -f *.o