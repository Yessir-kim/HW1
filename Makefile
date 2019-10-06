puzzle : number.o
	gcc -o puzzle number.o

number.o : number.c
	gcc -c -o number.o number.c
clean :
	rm *.o puzzle formula
