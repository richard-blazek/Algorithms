all: linked_lists.c one_way.c two_way.c xor_way.c
	cc -fsanitize=address linked_lists.c one_way.c two_way.c xor_way.c -o linked_lists.elf

test: all
	./linked_lists.elf

clean:
	rm -rf linked_lists.elf
