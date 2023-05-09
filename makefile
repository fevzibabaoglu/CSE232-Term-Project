all : main
main : my_mproc.o
	gcc my_mproc.o -o my_mproc.exe

my_proc.o: my_mproc.c
	gcc -c my_mproc.c

rm:
	rm -f *.o my_mproc.exe