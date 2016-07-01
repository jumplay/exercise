a.exe: red_black_tree.o a.o
	gcc a.o red_black_tree.o
a.o: a.c red_black_tree.h debug.h common.h
	gcc -c a.c
red_black_tree.o: red_black_tree.c red_black_tree.h common.h
	gcc -c red_black_tree.c
clean:
	rm red_black_tree.o a.o a.out
