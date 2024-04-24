#ifndef BPTree_hpp
#define BPTree_hpp

#include "BPTreeNode.hpp"
#include <queue>

template<typename KeyType>
class BPTree {
private:
    int order;
    BPTreeNode<KeyType> *root;

    BPTreeNode<KeyType> *Search_Recursive(BPTreeNode<KeyType> *node, KeyType key) {
        if (node->IsLeaf()) {
            // If the pointer of node is leaf node, return it
            return node;
        } else {
            // If the pointer of node is not leaf node, get the key
            int index = node->GetKeyIndex(key);
            if (index < node->GetKeyNum() && key == node->GetKeys()[index]) {
                // If key is found, move pointer to right and increase index
                return Search_Recursive(node->GetChildren()[index + 1], key);
            } else {
                // If key is not found, move pointer to left
                return Search_Recursive(node->GetChildren()[index], key);
            }
        }
    }

public:
    BPTree() {
        root = nullptr;
        order = 0;
    };

    ~BPTree() {
        delete (root);
    }

    // Operations for Initialize, Insert, Delete and Search
    void Initialize(int m) {
        order = m;
    }

    void Insert(KeyType key, float value) {
        if (!root) {
            root = new LeafNode<KeyType>(order);
        }
        // Define boolean for loops
        int midKey = 0;
        bool needNewRoot = false;
        BPTreeNode<KeyType> *rightNode = nullptr;
        BPTreeNode<KeyType> *node = Search_Recursive(root, key);
        node->Insert(key, value);

        // Get parent and split tree
        while (node->GetKeyNum() == order) {
            BPTreeNode<KeyType> *parent = node->GetParent();
            rightNode = node->Split(midKey);
            if (parent) {
                // If parent exists, insert
                parent->Insert(midKey, rightNode);
                rightNode->SetParent(parent);
                node = parent;
            } else {
                // Else break and deal with it outside
                needNewRoot = true;
                break;
            }
        }
        // Generate a new root and replace the old one
        if (needNewRoot) {
            InternalNode<KeyType> *newRoot = new InternalNode<KeyType>(order);
            newRoot->Insert(midKey, node, rightNode);
            node->SetParent(newRoot);
            rightNode->SetParent(newRoot);

            root = newRoot;
        }
    }

    void Delete(KeyType key) {
        BPTreeNode<KeyType> *node = Search_Recursive(root, key);

        node->Delete(key);

        if (root->GetKeyNum() == 0) {
            root = root->GetChildren()[0];
            delete root->GetParent();
            root->SetParent(NULL);
        }
    }

    bool Search(KeyType key, float &value) {
        if (root->GetKeyNum() == 0) {
            // The tree is empty
            return false;
        }

        LeafNode<KeyType> *node = (LeafNode<KeyType> *) Search_Recursive(root, key);
        int index = node->GetKeyIndex(key);

        if (key == node->GetKeys()[index]) {
            value = node->GetValues()[index];
            return true;
        } else {
            return false;
        }
    }

    bool *Search(KeyType key1, KeyType key2, float *&values, int &num) {
        num = 0;
        int buffSize = 1;
        bool *status = new bool[2];

        status[0] = false;
        status[1] = false;

        BPTreeNode<KeyType> *node1 = Search_Recursive(root, key1);
        BPTreeNode<KeyType> *node2 = Search_Recursive(root, key2);
        // cout << node1 << " " << node2 << endl;
        int index1 = node1->GetKeyIndex(key1);
        int index2 = node2->GetKeyIndex(key2);

        if (index1 < node1->GetKeyNum() && key1 == node1->GetKeys()[index1]) {
            status[0] = true;
        }

        if (index2 < node2->GetKeyNum() && key2 == node2->GetKeys()[index2]) {
            status[1] = true;
        }

        if (key1 < key2) {
            int index = index1;
            values = new float[buffSize];
            if ((!index1) < node1->GetKeyNum()) {
                index = 0;
                node1 = node1->GetNext();
            }

            while (node1 != node2->GetNext()) {
                for (; index < node1->GetKeyNum(); index++) {
                    if (node1->GetKeys()[index] <= key2) {
                        if (num == buffSize) {
                            buffSize *= 2;
                            float *newValues = new float[buffSize];

                            for (int i = 0; i < buffSize / 2; i++) {
                                newValues[i] = values[i];
                            }

                            delete[] values;
                            values = newValues;
                        }

                        values[num++] = node1->GetValues()[index];
                        // cout << values[num - 1] << endl;
                    } else {
                        break;
                    }
                }

                node1 = node1->GetNext();
                index = 0;
            }
        } else {
            values = new float[2];

            if (status[0]) {
                values[0] = node1->GetValues()[index1];
            }

            if (status[1]) {
                values[1] = node2->GetValues()[index2];
            }

            num = 2;
        }
        return status;
    }

    // Function to perform level order traversal of the B+ tree and print its structure
    void printTreeStructure() {
        if (!root)
            return;

        queue<BPTreeNode<KeyType> *> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();

            // Print keys of nodes at the current level
            for (int i = 0; i < levelSize; ++i) {
                BPTreeNode<KeyType> *node = q.front();
                q.pop();

                for (int j = 0; j < node->GetKeyNum(); ++j)
                    cout << node->GetKeys()[j] << " ";

                // Enqueue children of the current node
                if (!node->IsLeaf()) {
                    InternalNode<KeyType> *internal = dynamic_cast<InternalNode<KeyType> *>(node);
                    for (int j = 0; j <= internal->GetKeyNum(); ++j)
                        q.push(internal->GetChildren()[j]);
                }
            }

            // Move to the next level
            cout << endl;
        }
    }

};

#endif /* BPTree_hpp */
