mysh: mysh.o
	gcc -o mysh mysh.o -lm
	
mysh.o: mysh.c
	gcc -O -Wall -c mysh.c

clean:
	rm -f mysh.o mysh
