a.exe: binary_search_tree.o a.o
	gcc a.o binary_search_tree.o
a.o: a.c binary_search_tree.h
	gcc -c a.c
binary_search_tree.o: binary_search_tree.c binary_search_tree.h
	gcc -c binary_search_tree.c
clean:
	rm binary_search_tree.o a.o a.out
