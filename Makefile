OPTIONS = -g -O0 -fsanitize=address

algo: main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c priority_queue.c hash_table.c disjoint_set.c
	cc $(OPTIONS) main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c priority_queue.c hash_table.c disjoint_set.c -o algo

clean:
	rm -rf algo
