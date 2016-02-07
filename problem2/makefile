p2output: p2.o rbtree.o
	gcc p2.o rbtree.o -lrt -o p2output
p2.o: p2.c rbtree.h
	gcc -c p2.c
rtbree.o: rbtree.c rbtree.h
	gcc -c rbtree.c
clean:
	rm -f p2.o rbtree.o p2output *~
