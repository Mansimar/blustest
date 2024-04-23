
CC = g++
CFLAG = -g -c

bplustree : main.o
	$(CC) -o bplustree main.o

main.o : main.cpp
	$(CC) $(CFLAG) main.cpp

# BPTree.o : BPTree.cpp
# 	$(CC) $(CFLAG) BPTree.cpp

# BPTreeNode.o : BPTreeNode.cpp
# 	$(CC) $(CFLAG) BPTreeNode.cpp

clean: 
	rm -f *.o
	rm -f bplustree