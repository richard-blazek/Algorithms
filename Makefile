OPTIONS = -g -O0 -fsanitize=address

algo: main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c priority_queue.c hash_table.c fortran_algo.f95
	cc $(OPTIONS) main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c priority_queue.c hash_table.c -o algo
	gfortran fortran_algo.f95 -o fortran_algo

clean:
	rm -rf algo
