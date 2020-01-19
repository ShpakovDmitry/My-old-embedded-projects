mytest.exe: mytest.o myssdlib.o
	gcc mytest.o myssdlib.o -o mytest.exe
mytest.o: mytest.c myssdlib.o
	gcc -c mytest.c
myssdlib.o: myssdlib.c myssdlib.h
	gcc -c myssdlib.c
