#ifndef BPTreeNode_hpp
#define BPTreeNode_hpp

#include <stdio.h>
#include <cstddef>
#include <iostream>

using namespace std;

template<typename KeyType>
class BPTreeNode {
protected:
    int order;
    KeyType *keys;
    int keyNum;

    bool isLeaf;

    BPTreeNode<KeyType> *parent;

    BPTreeNode<KeyType> *prev;
    BPTreeNode<KeyType> *next;

public:

    BPTreeNode(int n) {
        order = n;
        // Initialize values of parent, previous and next node to be Null
        parent = NULL;
        prev = NULL;
        next = NULL;
        // Initialize values of keys
        keys = new KeyType[n];
        keyNum = 0;
    }

    ~BPTreeNode() {
        delete[] keys;
    }

    bool IsLeaf() {
        return isLeaf;
    }

    KeyType *GetKeys() {
        return keys;
    }

    int GetKeyNum() {
        return keyNum;
    }

    void DecreaseKeyNum() {
        keyNum--;
    }

    int GetKeyIndex(KeyType key) {
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

    BPTreeNode<KeyType> *GetParent() {
        return parent;
    }

    void SetParent(BPTreeNode<KeyType> *node) {
        parent = node;
    }

    BPTreeNode *GetNext() {
        return next;
    }

    void SetNext(BPTreeNode<KeyType> *node) {
        next = node;
    }

    BPTreeNode *GetPrev() {
        return prev;
    }

    void SetPrev(BPTreeNode<KeyType> *node) {
        prev = node;
    }

    virtual void Insert(KeyType key, float value) {}

    virtual void Insert(KeyType key, BPTreeNode<KeyType> *rightChild) {}

    virtual void Insert(KeyType key, BPTreeNode<KeyType> *leftChild, BPTreeNode<KeyType> *rightChild) {}

    virtual void Search(KeyType key) {}

    virtual void Search(KeyType key1, KeyType key2) {}

    virtual void Delete(KeyType key) {}

    virtual void Merge(BPTreeNode<KeyType> *rightNode) {}

    virtual BPTreeNode<KeyType> *Split(int &key) { return NULL; }

    virtual BPTreeNode<KeyType> **GetChildren() { return NULL; }

    virtual float *GetValues() { return NULL; }
};


// internal node
template<typename KeyType>
class InternalNode : public BPTreeNode<KeyType> {
private:
    BPTreeNode<KeyType> **children;

public:

    InternalNode(int n) : BPTreeNode<KeyType>(n) {
        this->isLeaf = false;
        children = new BPTreeNode<KeyType> *[n + 1];
    }

    ~InternalNode() {
        delete[] children;
    }

    void Insert(KeyType key, BPTreeNode<KeyType> *rightChild) {
        int index = this->GetKeyIndex(key);
        // move every key and child after index
        for (int i = this->keyNum; i > index; i--) {
            this->keys[i] = this->keys[i - 1];
            children[i + 1] = children[i];
        }

        this->keys[index] = key;
        children[index + 1] = rightChild;
        rightChild->SetNext(children[index]->GetNext());
        rightChild->SetPrev(children[index]);
        children[index]->SetNext(rightChild);

        if (rightChild->GetNext()) {
            rightChild->GetNext()->SetPrev(rightChild);
        }

        this->keyNum++;
    }

    void Insert(KeyType key, BPTreeNode<KeyType> *leftChild, BPTreeNode<KeyType> *rightChild) {
        this->keys[this->keyNum] = key;
        rightChild->SetPrev(leftChild);
        rightChild->SetNext(leftChild->GetNext());
        leftChild->SetNext(rightChild);

        if (rightChild->GetNext()) {
            rightChild->GetNext()->SetPrev(rightChild);
        }

        children[this->keyNum++] = leftChild;
        children[this->keyNum] = rightChild;
    }

    void Delete(KeyType key) {
        int index = this->GetKeyIndex(key);
        this->DecreaseKeyNum();

        if (this->keys[index] > key) {
            for (; index < this->keyNum; index++) {
                this->keys[index] = this->keys[index + 1];
                children[index] = children[index + 1];
            }

            children[index] = children[index + 1];

        } else {

            for (; index < this->keyNum; index++) {
                this->keys[index] = this->keys[index + 1];
                children[index + 1] = children[index + 2];
            }
        }

        if (this->keyNum < this->order / 2) {
            if (this->next && this->next->GetKeyNum() > this->order / 2) {
                this->next->DecreaseKeyNum();
                KeyType up = this->next->GetKeys()[0];
                BPTreeNode<KeyType> *borrowed = this->next->GetChildren()[0];
                borrowed->SetParent(this);
                BPTreeNode<KeyType> *leftParent = this->parent;
                BPTreeNode<KeyType> *rightParent = this->next->GetParent();

                while (leftParent != rightParent) {
                    leftParent = leftParent->GetParent();
                    rightParent = rightParent->GetParent();
                }

                int pindex = leftParent->GetKeyIndex(up);
                if (leftParent->GetKeys()[pindex] < up) {
                    pindex--;
                }

                KeyType down = leftParent->GetKeys()[pindex];
                leftParent->GetKeys()[pindex] = up;
                this->keys[this->keyNum++] = down;
                children[this->keyNum] = borrowed;

                for (int i = 0; i < this->next->GetKeyNum(); i++) {
                    this->next->GetKeys()[i] = this->next->GetKeys()[i + 1];
                    this->next->GetChildren()[i] = this->next->GetChildren()[i + 1];

                }

                this->next->GetChildren()[this->next->GetKeyNum()] = this->next->GetChildren()[this->next->GetKeyNum() + 1];

            } else if (this->prev && this->prev->GetKeyNum() > this->order / 2) {
                this->prev->DecreaseKeyNum();
                KeyType up = this->prev->GetKeys()[this->prev->GetKeyNum()];
                BPTreeNode<KeyType> *borrowed = this->prev->GetChildren()[this->prev->GetKeyNum() + 1];
                borrowed->SetParent(this);
                BPTreeNode<KeyType> *leftParent = this->prev->GetParent();
                BPTreeNode<KeyType> *rightParent = this->parent;

                while (leftParent != rightParent) {
                    leftParent = leftParent->GetParent();
                    rightParent = rightParent->GetParent();
                }

                int pindex = leftParent->GetKeyIndex(up);
                KeyType down = leftParent->GetKeys()[pindex];
                leftParent->GetKeys()[pindex] = up;

                for (int i = this->keyNum; i > 0; i--) {
                    this->keys[i] = this->next->GetKeys()[i + 1];
                    children[i + 1] = children[i];
                }

                children[1] = children[0];
                this->keys[0] = down;
                children[0] = borrowed;

            } else if (this->next) {
                this->Merge(this->next);
            } else if (this->prev) {
                this->prev->Merge(this);
            }
        }
    }

    void Merge(BPTreeNode<KeyType> *rightNode) {
        KeyType *rightKeys = rightNode->GetKeys();
        BPTreeNode<KeyType> **rightChildren = rightNode->GetChildren();
        BPTreeNode<KeyType> *rightParent = rightNode->GetParent();
        BPTreeNode<KeyType> *leftParent = this->parent;

        while (leftParent != rightParent) {
            leftParent = leftParent->GetParent();
            rightParent = rightParent->GetParent();
        }

        int pindex = leftParent->GetKeyIndex(rightKeys[0]);

        if (pindex == leftParent->GetKeyNum() || leftParent->GetKeys()[pindex] > rightKeys[0]) {
            pindex--;
        }

        KeyType down = leftParent->GetKeys()[pindex];

        if (this->keyNum == 0) {
            this->keys[this->keyNum] = down;
            for (int i = 0; i <= rightNode->GetKeyNum(); i++) {
                this->keys[this->keyNum + i + 1] = rightKeys[i];
                children[this->keyNum + i + 1] = rightChildren[i];
                children[this->keyNum + i + 1]->SetParent(this);
            }
        } else {
            this->keys[this->keyNum - 1] = down;

            for (int i = 0; i <= rightNode->GetKeyNum(); i++) {
                this->keys[this->keyNum + i] = rightKeys[i];
                children[this->keyNum + i + 1] = rightChildren[i];
                children[this->keyNum + i + 1]->SetParent(this);

            }

        }

        this->keyNum += 1 + rightNode->GetKeyNum();
        this->next = rightNode->GetNext();

        if (this->next) {
            this->next->SetPrev(this);
        }

        rightNode->GetParent()->Delete(down);

        delete rightNode;
    }

    BPTreeNode<KeyType> *Split(int &key) {
        // New right node
        InternalNode *rightNode = new InternalNode(this->order);
        // The key to be moved into the the parent
        key = this->keys[this->keyNum / 2];
        // Copy every key and child after keyNum / 2 to rightNode
        for (int i = this->keyNum / 2 + 1; i < this->keyNum; i++) {
            rightNode->keys[i - this->keyNum / 2 - 1] = this->keys[i];
            rightNode->children[i - this->keyNum / 2 - 1] = this->children[i];
        }

        rightNode->keyNum = this->keyNum - this->keyNum / 2 - 1;
        rightNode->children[rightNode->keyNum] = this->children[this->keyNum];
        this->keyNum = this->keyNum / 2;

        for (int i = 0; i <= rightNode->GetKeyNum(); i++) {
            rightNode->children[i]->SetParent(rightNode);
        }

        return rightNode;
    }

    BPTreeNode<KeyType> **GetChildren() {
        return children;
    }

};


// leaf node
template<typename KeyType>
class LeafNode : public BPTreeNode<KeyType> {
private:

    float *values;

public:

    LeafNode(int n) : BPTreeNode<KeyType>(n) {
        this->isLeaf = true;
        values = new float[n];
    }

    ~LeafNode() {
        delete[] values;
    }

    void Insert(KeyType key, float value) {
        int index = this->GetKeyIndex(key);

        for (int i = this->keyNum; i > index; i--) {
            this->keys[i] = this->keys[i - 1];
            values[i] = values[i - 1];
        }

        this->keys[index] = key;
        values[index] = value;
        this->keyNum++;
    }

    void Delete(KeyType key) {
        int index = this->GetKeyIndex(key);

        if (this->keys[index] == key) {
            this->DecreaseKeyNum();
            for (; index < this->keyNum; index++) {
                this->keys[index] = this->keys[index + 1];
                values[index] = values[index + 1];
            }
        } else {
            cout << key << " not found!" << endl;
            return;
        }

        if (this->keyNum < this->order / 2) {
            if (this->next && this->next->GetKeyNum() > this->order / 2) {
                this->next->DecreaseKeyNum();
                this->keys[this->keyNum] = this->next->GetKeys()[0];
                values[this->keyNum++] = this->next->GetValues()[0];

                for (int i = 0; i < this->next->GetKeyNum(); i++) {
                    this->next->GetKeys()[i] = this->next->GetKeys()[i + 1];
                    this->next->GetValues()[i] = this->next->GetValues()[i + 1];
                }

                BPTreeNode<KeyType> *leftParent = this->parent;
                BPTreeNode<KeyType> *rightParent = this->next->GetParent();

                while (leftParent != rightParent) {
                    leftParent = leftParent->GetParent();
                    rightParent = rightParent->GetParent();
                }

                int pindex = leftParent->GetKeyIndex(this->keys[this->keyNum - 1]);

                if (leftParent->GetKeys()[pindex] != this->keys[this->keyNum - 1]) {
                    pindex--;
                }

                leftParent->GetKeys()[pindex] = this->next->GetKeys()[0];

            } else if (this->prev && this->prev->GetKeyNum() > this->order / 2) {

                this->prev->DecreaseKeyNum();

                for (int i = this->keyNum; i > 0; i--) {
                    this->keys[i] = this->keys[i - 1];
                    values[i] = values[i - 1];
                }

                this->keys[0] = this->prev->GetKeys()[this->prev->GetKeyNum()];
                values[0] = this->prev->GetValues()[this->prev->GetKeyNum()];
                this->keyNum++;
                BPTreeNode<KeyType> *leftParent = this->prev->GetParent();
                BPTreeNode<KeyType> *rightParent = this->parent;

                while (leftParent != rightParent) {
                    leftParent = leftParent->GetParent();
                    rightParent = rightParent->GetParent();
                }

                int pindex = leftParent->GetKeyIndex(this->keys[0]);
                leftParent->GetKeys()[pindex] = this->keys[0];

            } else if (this->next) {
                this->Merge(this->next);
            } else if (this->prev) {
                this->prev->Merge(this);
            }
        }
    }

    void Merge(BPTreeNode<KeyType> *rightNode) {
        KeyType *rightKeys = rightNode->GetKeys();
        float *rightValues = rightNode->GetValues();
        KeyType down = rightKeys[0];

        for (int i = 0; i < rightNode->GetKeyNum(); i++) {
            this->keys[this->keyNum + i] = rightKeys[i];
            values[this->keyNum + i] = rightValues[i];
        }

        this->keyNum += rightNode->GetKeyNum();
        this->next = rightNode->GetNext();

        if (this->next) {
            this->next->SetPrev(this);
        }

        rightNode->GetParent()->Delete(down);
        delete rightNode;
    }

    BPTreeNode<KeyType> *Split(int &key) {
        // New right node
        LeafNode *rightNode = new LeafNode(this->order);
        // The key to be moved into the parent
        key = this->keys[this->keyNum / 2];
        // Copy every key and value after keyNum / 2 to rightNode
        for (int i = this->keyNum / 2; i < this->keyNum; i++) {
            rightNode->keys[i - this->keyNum / 2] = this->keys[i];
            rightNode->values[i - this->keyNum / 2] = values[i];
        }

        rightNode->keyNum = this->keyNum - this->keyNum / 2;
        this->keyNum = this->keyNum / 2;
        return rightNode;
    }

    float *GetValues() {
        return values;
    }
};

#endif /* BPTreeNode_hpp */
