OPTIONS = -g -O0 -fsanitize=address

main: main.o sort.o singly_ll.o doubly_ll.o xored_ll.o stack.o queue.o deque.o priority_queue.o hash_table.o disjoint_set.o
	cc $(OPTIONS) main.o sort.o singly_ll.o doubly_ll.o xored_ll.o stack.o queue.o deque.o priority_queue.o hash_table.o disjoint_set.o -o main

main.o: main.c
	cc $(OPTIONS) -c main.c -o main.o

sort.o: sort.c
	cc $(OPTIONS) -c sort.c -o sort.o

singly_ll.o: singly_ll.c
	cc $(OPTIONS) -c singly_ll.c -o singly_ll.o

doubly_ll.o: doubly_ll.c
	cc $(OPTIONS) -c doubly_ll.c -o doubly_ll.o

xored_ll.o: xored_ll.c
	cc $(OPTIONS) -c xored_ll.c -o xored_ll.o

deque.o: deque.c
	cc $(OPTIONS) -c deque.c -o deque.o

stack.o: stack.c
	cc $(OPTIONS) -c stack.c -o stack.o

queue.o: queue.c
	cc $(OPTIONS) -c queue.c -o queue.o

priority_queue.o: priority_queue.c
	cc $(OPTIONS) -c priority_queue.c -o priority_queue.o

hash_table.o: hash_table.c
	cc $(OPTIONS) -c hash_table.c -o hash_table.o

disjoint_set.o: disjoint_set.c
	cc $(OPTIONS) -c disjoint_set.c -o disjoint_set.o

clean:
	rm -rf *.o main
