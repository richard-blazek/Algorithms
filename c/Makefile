OPTIONS = -g -O0 -fsanitize=address

main: main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c priority_queue.c hash_table.c
	cc $(OPTIONS) main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c priority_queue.c hash_table.c -o main

clean:
	rm -rf main
