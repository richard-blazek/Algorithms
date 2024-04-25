all:
	gfortran deque.f95 -o deque.elf
	gfortran disjoint_set.f95 -o disjoint_set.elf
	gfortran hash_table.f95 -o hash_table.elf
	gfortran priority_queue.f95 -o priority_queue.elf
	gfortran sorting.f95 -o sorting.elf
	rm *.mod

test: all
	./deque.elf
	./disjoint_set.elf
	./hash_table.elf
	./priority_queue.elf
	./sorting.elf

clean:
	rm -rf *.elf
