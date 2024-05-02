all: test_linked_list.c singly_linked_list.c doubly_linked_list.c xorptr_linked_list.c
	cc -fsanitize=address test_linked_list.c singly_linked_list.c doubly_linked_list.c xorptr_linked_list.c -o test_linked_list.elf

test: all
	./test_linked_list.elf

clean:
	rm -rf test_linked_list.elf
