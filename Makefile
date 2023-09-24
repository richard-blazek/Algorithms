OPTIONS='-g'

main: main.o sort.o link1_list.o link2_list.o
	cc $(OPTIONS) main.o sort.o link1_list.o link2_list.o -o main

main.o: main.c
	cc $(OPTIONS) -c main.c -o main.o

sort.o: sort.c
	cc $(OPTIONS) -c sort.c -o sort.o

link1_list.o: link1_list.c
	cc $(OPTIONS) -c link1_list.c -o link1_list.o

link2_list.o: link2_list.c
	cc $(OPTIONS) -c link2_list.c -o link2_list.o
