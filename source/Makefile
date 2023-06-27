data : main.o mem_allocators.o
	gcc main.o mem_allocators.o -o data

main.o: main.c
	gcc -c -g -pthread main.c -o main.o

mem_allocators.o: mem_allocators.c
	gcc -c -g -pthread mem_allocators.c -o mem_allocators.o

clean:
	rm data *.o