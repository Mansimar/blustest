//
//  BPTreeNode.hpp
//  BPTreeNode_Declaration
//
//  Name: Zhang Dong
//  UFID: 69633923
//  Email: zdong@ufl.edu
//

#ifndef BPTreeNode_hpp
#define BPTreeNode_hpp

#include <stdio.h>
#include <cstddef>
#include <iostream>

using namespace std;

class BPTreeNode 
{
    protected:
        
        int order;
        int *keys;
        int keyNum;
        
        bool isLeaf;
        
        BPTreeNode *parent;
        
        BPTreeNode *prev;
        BPTreeNode *next;
        
    public:
        
        BPTreeNode (int n);
        ~BPTreeNode ();
        
        bool IsLeaf ();
        int *GetKeys ();
        int GetKeyNum ();
        void DecreaseKeyNum ();
        int GetKeyIndex (int key);
        BPTreeNode *GetParent ();
        void SetParent (BPTreeNode *node);
        BPTreeNode *GetNext ();
        void SetNext (BPTreeNode *node);
        BPTreeNode *GetPrev ();
        void SetPrev (BPTreeNode *node);
        
        virtual void Insert (int key, float value) {}
        virtual void Insert (int key, BPTreeNode* rightChild) {}
        virtual void Insert (int key, BPTreeNode* leftChild, BPTreeNode* rightChild) {}
        virtual void Search (int key) {}
        virtual void Search (int key1, int key2) {}
        virtual void Delete (int key) {}
        virtual void Merge (BPTreeNode* rightNode) {}
        virtual BPTreeNode *Split(int &key) { return NULL; }
        virtual BPTreeNode **GetChildren () { return NULL; }
        virtual float *GetValues () { return NULL; }      
};


// internal node
class InternalNode : public BPTreeNode
{
    private:
        
        BPTreeNode **children;
        
    public:
        
        InternalNode (int n);
        ~InternalNode ();
        
        void Insert (int key, BPTreeNode* rightChild);
        void Insert (int key, BPTreeNode* leftChild, BPTreeNode* rightChild);
        void Delete (int key);
        void Merge (BPTreeNode* rightNode);
        BPTreeNode *Split (int &key);
        BPTreeNode **GetChildren ();
    
};


// leaf node
class LeafNode : public BPTreeNode
{
    private:
        
        float *values;
        
    public:
        
        LeafNode (int n);
        ~LeafNode ();
        
        void Insert (int key, float value);
        void Delete (int key);
        void Merge (BPTreeNode* rightNode);
        BPTreeNode *Split (int &key);
        float *GetValues (); 
};

#endif /* BPTreeNode_hpp */