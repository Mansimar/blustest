
CC = g++
CFLAG = -g -c

bplustree : BPTreeNode.o BPTree.o main.o
	$(CC) -o bplustree BPTreeNode.o BPTree.o main.o

main.o : main.cpp
	$(CC) $(CFLAG) main.cpp

BPTree.o : BPTree.cpp
	$(CC) $(CFLAG) BPTree.cpp

BPTreeNode.o : BPTreeNode.cpp
	$(CC) $(CFLAG) BPTreeNode.cpp

clean: 
	rm -f *.o
	rm -f bplustree