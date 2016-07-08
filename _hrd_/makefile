a.out: a.o hrd.o
	gcc a.o hrd.o
a.o: a.c hrd.h
	gcc -c a.c
hrd.o: hrd.c hrd.h
	gcc -c hrd.c
clean:
	rm -f *.o *.exe *.out
