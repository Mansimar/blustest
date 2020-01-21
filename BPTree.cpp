//
//  BPTree.cpp
//  BPTree
//
//  Name: Zhang Dong
//  UFID: 69633923
//  Email: zdong@ufl.edu
//

#include "BPTree.hpp"
#include <stdio.h>

#ifdef WIN32
	#include <io.h>
#endif

#ifdef LINUX
	#include <unistd.h>
#endif

//  Define constructor for BPTree
BPTree :: BPTree ()
{
    root = NULL;
    order = 0;
}

//  Define destructor for BPTree
BPTree :: ~BPTree ()
{
    delete(root);
}

//  Define function Initialize(3)
void BPTree :: Initialize (int m) 
{
    order = m;
}

//  Define function Search_Recursive to implement recursive search for (node, key)
BPTreeNode *BPTree :: Search_Recursive (BPTreeNode *node, int key)
{ 
    if (node->IsLeaf ()) {  
        //  If the pointer of node is leafnode, return it
        return node;
    } else {
        //  If the pointer of node is not leadnode, get the key
        int index = node->GetKeyIndex (key);
        if (index < node->GetKeyNum () && key == node->GetKeys ()[index]) {
            // If key is found, move pointer to right and increase index
            return Search_Recursive (node->GetChildren ()[index + 1], key);
        } else {
            // If key is not found, move pointer to left
            return Search_Recursive (node->GetChildren ()[index], key);
        }
    }
}

//  Define function Insert(21, 0.3534)
void BPTree :: Insert (int key, float value) 
{
    if (!root) {
        root = new LeafNode (order);
    }
    // Define boolean for loops
    int midKey = 0;
    bool needNewRoot = false;
    BPTreeNode *rightNode = nullptr;
    BPTreeNode *node = Search_Recursive (root, key);
    node->Insert (key, value);
    
    //  Get parent and split tree
    while (node->GetKeyNum () == order) {  
        BPTreeNode *parent = node->GetParent ();
        rightNode = node->Split (midKey);
        if (parent) { 
            // If parent exists, insert
            parent->Insert (midKey, rightNode);
            rightNode->SetParent (parent);
            node = parent; 
        } else { 
            // Else break and deal with it outside
            needNewRoot = true;
            break; 
        }
    }
    // Generate a new root and replace the old one
    if (needNewRoot) {
        InternalNode *newRoot = new InternalNode (order);
        newRoot->Insert (midKey, node, rightNode);
        node->SetParent (newRoot);
        rightNode->SetParent (newRoot);
        
        root = newRoot;
    }
}

//  Define function Delete(108)
void BPTree :: Delete (int key)
{
    BPTreeNode *node = Search_Recursive (root, key);

    node->Delete (key);
    
    if (root->GetKeyNum () == 0) {
        root = root->GetChildren ()[0];
        delete root->GetParent ();
        root->SetParent (NULL);  
    }   
}

//  Define function Search(234)
bool BPTree :: Search (int key, float &value)
{
    if (root->GetKeyNum () == 0) {
        // The tree is empty
        return false;
    }
    
    LeafNode *node = (LeafNode *)Search_Recursive (root, key);
    int index = node->GetKeyIndex (key);
    
    if (key == node->GetKeys ()[index]) {
        value = node->GetValues ()[index];
        return true;
    } else {
        return false;  
    }
}

//  Define function Search(23, 99)
bool *BPTree :: Search (int key1, int key2, float *&values, int &num) 
{
    num = 0;
    int buffSize = 1;
    bool *status = new bool[2];
    
    status[0] = false;
    status[1] = false;
    
    BPTreeNode *node1 = Search_Recursive (root, key1);
    BPTreeNode *node2 = Search_Recursive (root, key2);
    // cout << node1 << " " << node2 << endl;
    int index1 = node1->GetKeyIndex (key1);
    int index2 = node2->GetKeyIndex (key2);
    
    if (index1 < node1->GetKeyNum () && key1 == node1->GetKeys ()[index1]) {
        status[0] = true;   
    }
    
    if (index2 < node2->GetKeyNum () && key2 == node2->GetKeys ()[index2]) {
        status[1] = true;
    }
    
    if (key1 < key2) {
        int index = index1;
        values = new float[buffSize];
        if ((!index1) < node1->GetKeyNum ()) {
            index = 0;
            node1 = node1->GetNext ();
        }
        
        while (node1 != node2->GetNext ()) {
            for (; index < node1->GetKeyNum (); index++) {
                if (node1->GetKeys ()[index] <= key2) {
                    if (num == buffSize) {
                        buffSize *= 2;
                        float *newValues = new float[buffSize];
                        
                        for (int i = 0; i < buffSize / 2; i++) {
                            newValues[i] = values[i];
                        }
                        
                        delete[] values;
                        values = newValues;
                    }
                    
                    values[num++] = node1->GetValues ()[index];
                    // cout << values[num - 1] << endl;
                } else {
                    break; 
                }
            }
            
            node1 = node1->GetNext ();
            index = 0;  
        }  
    } else {
        values = new float[2];

        if (status[0]) {
            values[0] = node1->GetValues ()[index1]; 
        }
        
        if (status[1]) {
            values[1] = node2->GetValues ()[index2];
        }
        
        num = 2;   
    }
    return status;
}