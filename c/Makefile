OPTIONS = -g -O0 -fsanitize=address

main.elf: main.c singly_ll.c doubly_ll.c xored_ll.c
	cc $(OPTIONS) main.c singly_ll.c doubly_ll.c xored_ll.c -o main.elf

clean:
	rm -rf main.elf
