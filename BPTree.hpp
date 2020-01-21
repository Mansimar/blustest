//
//  BPTree.hpp
//  BPTree_Declaration
//
//  Name: Zhang Dong
//  UFID: 69633923
//  Email: zdong@ufl.edu
//

#ifndef BPTree_hpp
#define BPTree_hpp

#include "BPTreeNode.hpp"

class BPTree 
{   
    private:
    
        int order;
        BPTreeNode *root;
        BPTreeNode *Search_Recursive (BPTreeNode *node, int key);
        
    public:
    
        BPTree ();
        ~BPTree ();
        //  Operations for Initialize, Insert, Delete and Search
        void Initialize (int m);
        void Insert (int key, float value);
        void Delete (int key);
        bool Search (int key, float &value);
        bool *Search (int key1, int key2, float *&values, int &num);
    
};

#endif /* BPTree_hpp */