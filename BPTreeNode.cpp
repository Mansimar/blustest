//
//  BPTreeNode.cpp
//  BPTreeNode
//
//  Name: Zhang Dong
//  UFID: 69633923
//  Email: zdong@ufl.edu
//

#include "BPTreeNode.hpp"

//  Define constructor of BPTreeNode
BPTreeNode :: BPTreeNode (int n)
{
    order = n;
    //  Initialize values of parent, previour and next node to be Null
    parent = NULL;
    prev = NULL;
    next = NULL;
    //  Initialize values of keys
    keys = new int[n];
    keyNum = 0;
}

//  Define destructor of BPTreeNode
BPTreeNode :: ~BPTreeNode ()
{
    delete[] keys;
}

bool BPTreeNode :: IsLeaf()
{
    return isLeaf;
}

int *BPTreeNode :: GetKeys()
{
    return keys;
}

int BPTreeNode :: GetKeyNum ()
{
    return keyNum;
}

void BPTreeNode :: DecreaseKeyNum ()
{
    keyNum--;
}

int BPTreeNode :: GetKeyIndex (int key)
{
    int left = 0;
    int right = keyNum - 1;
    int mid;
    
    while (left < right) {
        mid = (left + right) / 2;
        
        if (keys[mid] == key) {
            return mid; 
        } else if (keys[mid] > key) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    if (left == right) {
        if (keys[left] >= key) {  
            return left; 
        } else { 
            return left + 1; 
        }
    }

    return left;
}

BPTreeNode *BPTreeNode :: GetParent ()
{
    return parent;
}

void BPTreeNode :: SetParent (BPTreeNode *node)
{
    parent = node;
}

BPTreeNode *BPTreeNode :: GetNext()
{
    return next;
}

void BPTreeNode :: SetNext (BPTreeNode *node)
{
    next = node;
}

BPTreeNode *BPTreeNode :: GetPrev ()
{
    return prev;
}

void BPTreeNode :: SetPrev (BPTreeNode *node)
{
    prev = node;
}

//  Define internalnode
InternalNode :: InternalNode (int n) : BPTreeNode (n)
{
    isLeaf = false;
    children = new BPTreeNode*[n + 1];
}

InternalNode :: ~InternalNode ()
{
    delete[] children;
}

// Insert a new key and node into the current internal node
void InternalNode :: Insert (int key, BPTreeNode* rightChild)
{
    int index = GetKeyIndex (key);
    // move every key and child after index
    for (int i = keyNum; i > index; i--) {
        keys[i] = keys[i - 1];
        children[i + 1] = children[i];
    }
    
    keys[index] = key;
    children[index + 1] = rightChild;
    rightChild->SetNext(children[index]->GetNext ());
    rightChild->SetPrev(children[index]);
    children[index]->SetNext (rightChild);

    if (rightChild->GetNext ()) {
        rightChild->GetNext ()->SetPrev(rightChild); 
    }
    
    keyNum++; 
}

// Used only when creating new InternalNode
void InternalNode :: Insert (int key, BPTreeNode* leftChild, BPTreeNode* rightChild)
{
    keys[keyNum] = key;
    rightChild->SetPrev (leftChild);
    rightChild->SetNext (leftChild->GetNext ());
    leftChild->SetNext (rightChild);

    if (rightChild->GetNext ()) {
        rightChild->GetNext ()->SetPrev (rightChild);
    }
    
    children[keyNum++] = leftChild;
    children[keyNum] = rightChild;
}

void InternalNode :: Delete (int key)
{
    int index = GetKeyIndex (key);
    DecreaseKeyNum ();
    
    if (keys[index] > key) {
        for (; index < keyNum; index++) { 
            keys[index] = keys[index + 1];
            children[index] = children[index + 1];
        }
        
        children[index] = children[index + 1];
        
    } else {
        
        for (; index < keyNum; index++) {
            keys[index] = keys[index + 1];
            children[index + 1] = children[index + 2];
        }  
    }
    
    if (keyNum < order / 2) {
        if (next && next->GetKeyNum () > order / 2) {
            next->DecreaseKeyNum ();
            int up = next->GetKeys ()[0];
            BPTreeNode *borrowed = next->GetChildren ()[0];
            borrowed->SetParent (this);
            BPTreeNode *leftParent = parent;
            BPTreeNode *rightParent = next->GetParent ();
            
            while (leftParent != rightParent) {
                leftParent = leftParent->GetParent ();
                rightParent = rightParent->GetParent ();
            }
            
            int pindex = leftParent->GetKeyIndex (up);
            if (leftParent->GetKeys ()[pindex] < up) {
                pindex--;
            }
            
            int down = leftParent->GetKeys ()[pindex];
            leftParent->GetKeys ()[pindex] = up;
            keys[keyNum++] = down;
            children[keyNum] = borrowed;
            
            for (int i = 0; i < next->GetKeyNum(); i++) {
                next->GetKeys ()[i] = next->GetKeys ()[i + 1];
                next->GetChildren ()[i] = next->GetChildren ()[i + 1];
                
            }
            
            next->GetChildren ()[next->GetKeyNum()] = next->GetChildren ()[next->GetKeyNum() + 1];
            
        } else if (prev && prev->GetKeyNum () > order / 2) {
            prev->DecreaseKeyNum ();
            int up = prev->GetKeys ()[prev->GetKeyNum ()];
            BPTreeNode *borrowed = prev->GetChildren ()[prev->GetKeyNum () + 1];
            borrowed->SetParent (this);
            BPTreeNode *leftParent = prev->GetParent ();
            BPTreeNode *rightParent = parent;
            
            while (leftParent != rightParent) {
                leftParent = leftParent->GetParent ();
                rightParent = rightParent->GetParent ();
            }
            
            int pindex = leftParent->GetKeyIndex (up);
            int down = leftParent->GetKeys ()[pindex];
            leftParent->GetKeys ()[pindex] = up;
            
            for (int i = keyNum; i > 0; i--) {
                keys[i] = next->GetKeys ()[i + 1];
                children[i + 1] = children[i];
            }
            
            children[1] = children[0];
            keys[0] = down;
            children[0] = borrowed;
            
        } else if (next) {
            Merge (next);
        } else if (prev) {
            prev->Merge (this);
        }
    }
}

void InternalNode :: Merge (BPTreeNode* rightNode)
{
    int *rightKeys = rightNode->GetKeys ();
    BPTreeNode **rightChildren = rightNode->GetChildren ();
    BPTreeNode *rightParent = rightNode->GetParent ();
    BPTreeNode *leftParent = parent;
    
    while (leftParent != rightParent) {
        leftParent = leftParent->GetParent ();
        rightParent = rightParent->GetParent ();
    }
    
    int pindex = leftParent->GetKeyIndex (rightKeys[0]);
    
    if (pindex == leftParent->GetKeyNum () || leftParent->GetKeys ()[pindex] > rightKeys[0]) {
        pindex--;
    }
    
    int down = leftParent->GetKeys ()[pindex];
    
    if (keyNum == 0) {
        keys[keyNum] = down;
        for (int i = 0; i <= rightNode->GetKeyNum (); i++) { 
            keys[keyNum + i + 1] = rightKeys[i];
            children[keyNum + i + 1] = rightChildren[i];
            children[keyNum + i + 1]->SetParent (this);
        }
    } else {
        keys[keyNum - 1] = down;
        
        for (int i = 0; i <= rightNode->GetKeyNum (); i++) {
            keys[keyNum + i] = rightKeys[i];
            children[keyNum + i + 1] = rightChildren[i];
            children[keyNum + i + 1]->SetParent (this);
            
        }
        
    }
    
    keyNum += 1 + rightNode->GetKeyNum ();
    next = rightNode->GetNext ();

    if (next) {   
        next->SetPrev (this);
    }

    rightNode->GetParent ()->Delete (down);
    
    delete rightNode;
}

BPTreeNode *InternalNode :: Split (int &key)
{
    // New right node
    InternalNode *rightNode = new InternalNode (order);
    // The key to be moved into the the parent
    key = keys[keyNum / 2];
    // Copy every key and child after keyNum / 2 to rightNode
    for (int i = keyNum / 2 + 1; i < keyNum; i++) {
        rightNode->keys[i - keyNum / 2 - 1] = keys[i];
        rightNode->children[i - keyNum / 2 - 1] = children[i];
    }
    
    rightNode->keyNum = keyNum - keyNum / 2 - 1;
    rightNode->children[rightNode->keyNum] = children[keyNum];
    keyNum = keyNum / 2;
    
    for (int i = 0; i <= rightNode->GetKeyNum (); i++) {
        rightNode->children[i]->SetParent (rightNode);
    }
    
    return rightNode;
}

BPTreeNode **InternalNode :: GetChildren ()
{
    return children;
}

LeafNode :: LeafNode (int n) : BPTreeNode (n) 
{
    isLeaf = true;
    values = new float[n];
}

LeafNode :: ~LeafNode ()
{
    delete[] values;
}

void LeafNode :: Insert (int key, float value)
{
    int index = GetKeyIndex (key);
    
    for (int i = keyNum; i > index; i--) {
        keys[i] = keys[i - 1];
        values[i] = values[i - 1];
    }
    
    keys[index] = key;
    values[index] = value;
    keyNum++;
}

void LeafNode :: Delete (int key) 
{
    int index = GetKeyIndex (key);
    
    if (keys[index] == key) {
        DecreaseKeyNum ();
        for (; index < keyNum; index++) {  
            keys[index] = keys[index + 1];
            values[index] = values[index + 1];
        }
    } else {
        cout << key << " not found!" << endl;
        return;
    }
    
    if (keyNum < order / 2) {
        if (next && next->GetKeyNum () > order / 2) {
            next->DecreaseKeyNum ();
            keys[keyNum] = next->GetKeys ()[0];
            values[keyNum++] = next->GetValues ()[0];
            
            for (int i = 0; i < next->GetKeyNum (); i++) {
                next->GetKeys ()[i] = next->GetKeys ()[i + 1];
                next->GetValues ()[i] = next->GetValues ()[i + 1];
            }
            
            BPTreeNode *leftParent = parent;
            BPTreeNode *rightParent = next->GetParent ();
            
            while (leftParent != rightParent) {
                leftParent = leftParent->GetParent ();
                rightParent = rightParent->GetParent ();
            }
            
            int pindex = leftParent->GetKeyIndex (keys[keyNum - 1]);

            if (leftParent->GetKeys ()[pindex] != keys[keyNum - 1]) {
                pindex--;
            }
            
            leftParent->GetKeys ()[pindex] = next->GetKeys ()[0];
            
        } else if (prev && prev->GetKeyNum () > order / 2) {
            
            prev->DecreaseKeyNum ();
            
            for (int i = keyNum; i > 0; i--) { 
                keys[i] = keys[i - 1];
                values[i] = values[i - 1];
            }
            
            keys[0] = prev->GetKeys ()[prev->GetKeyNum ()];
            values[0] = prev->GetValues ()[prev->GetKeyNum ()];
            keyNum++;
            BPTreeNode *leftParent = prev->GetParent ();
            BPTreeNode *rightParent = parent;
            
            while (leftParent != rightParent) {
                leftParent = leftParent->GetParent ();
                rightParent = rightParent->GetParent (); 
            }
            
            int pindex = leftParent->GetKeyIndex (keys[0]);
            leftParent->GetKeys ()[pindex] = keys[0];

        } else if (next) {  
            Merge(next); 
        } else if (prev) {
            prev->Merge (this);
        }
    }
}

void LeafNode :: Merge (BPTreeNode* rightNode) {

    int *rightKeys = rightNode->GetKeys ();
    float *rightValues = rightNode->GetValues ();
    int down = rightKeys[0];

    for (int i = 0; i < rightNode->GetKeyNum (); i++) {
        keys[keyNum + i] = rightKeys[i];
        values[keyNum + i] = rightValues[i];
    }
    
    keyNum += rightNode->GetKeyNum ();
    next = rightNode->GetNext ();

    if (next) {
        next->SetPrev (this);
    }
    
    rightNode->GetParent ()->Delete (down);
    delete rightNode;
}

BPTreeNode *LeafNode :: Split (int &key) {
    // New right node
    LeafNode *rightNode = new LeafNode (order);
    // The key to be moved into the parent
    key = keys[keyNum / 2];
    // Copy every key and value after keyNum / 2 to rightNode
    for (int i = keyNum / 2; i < keyNum; i++) {
        rightNode->keys[i - keyNum / 2] = keys[i];
        rightNode->values[i - keyNum / 2] = values[i];
    }
    
    rightNode->keyNum = keyNum - keyNum / 2;
    keyNum = keyNum / 2;
    return rightNode;
}

float *LeafNode :: GetValues() {
    return values;
}
