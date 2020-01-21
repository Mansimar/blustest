# BplusTree
B+ Tree Implementation for COP 5536 project assignment.

# Usage
```
mkdir bptree
git clone https://github.com/zdong1995/BplusTree.git bptree
make
./bplustree SampleInput.txt
```
# Summary
1. BPTree.cpp & BPTree.hpp

Definition and declaration of B+ Tree operations, including:

- Initialize(3) : void Initialize (int m)
- Insert(21, 0.3534) :void Insert (int key, float value)
- Delete(108) : void Delete (int key)
- Search(234) : bool Search (int key, float &value)
- Search(23, 99) : bool *Search (int key1, int key2, float *&values, int
&num)

2. BPTreeNode.cpp & BPTreeNode.hpp

Definition and declaration of B+ Tree structure for operations, including:

- BPTreeNode: Insert, Search, Delete, Merge, Split, GetChildren
- InternalNode: Insert, Delete, Merge, Split, GetChildren
- LeafNode: Insert, Delete, Merge, Split, GetValue

3. main.cpp

The main entrance of code. Including file input and output processing.
