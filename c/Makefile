OPTIONS = -g -O0 -fsanitize=address

main.elf: main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c hash_table.c
	cc $(OPTIONS) main.c sort.c singly_ll.c doubly_ll.c xored_ll.c stack.c queue.c deque.c hash_table.c -o main.elf

clean:
	rm -rf main.elf
