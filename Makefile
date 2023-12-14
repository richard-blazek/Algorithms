OPTIONS = -g -O0

main: main.o sort.o link1_list.o link2_list.o stack.o queue.o deque.o priority_queue.o hash_table.o
	cc $(OPTIONS) main.o sort.o link1_list.o link2_list.o stack.o queue.o deque.o priority_queue.o hash_table.o -o main

main.o: main.c
	cc $(OPTIONS) -c main.c -o main.o

sort.o: sort.c
	cc $(OPTIONS) -c sort.c -o sort.o

link1_list.o: link1_list.c
	cc $(OPTIONS) -c link1_list.c -o link1_list.o

link2_list.o: link2_list.c
	cc $(OPTIONS) -c link2_list.c -o link2_list.o

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

clean:
	rm -rf *.o main
