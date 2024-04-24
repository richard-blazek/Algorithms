all: disjoint_set.f95 priority_queue.f95
	gfortran disjoint_set.f95 -o disjoint_set.elf
	gfortran priority_queue.f95 -o priority_queue.elf

test: all
	./disjoint_set.elf
	./priority_queue.elf

clean:
	rm -rf main.elf
