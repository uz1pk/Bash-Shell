myShell: myShell.o
	gcc -Wpedantic -std=gnu99 myShell.o -o myShell

myShell.o: myShell.c myShell.h
	gcc -Wpedantic -std=gnu99 -c myShell.c

clean:
	rm -rf *.o myShell