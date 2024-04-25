all: disjoint_set.f95 hash_table.f95 priority_queue.f95
	gfortran disjoint_set.f95 -o disjoint_set.elf
	gfortran hash_table.f95 -o hash_table.elf
	gfortran priority_queue.f95 -o priority_queue.elf
	rm *.mod

test: all
	./disjoint_set.elf
	./hash_table.elf
	./priority_queue.elf

clean:
	rm -rf *.elf
