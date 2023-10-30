// Author: Jakub Lisowski

#ifndef MY_OWN_BTREE_H
#define MY_OWN_BTREE_H

#include <iostream>
#include <iomanip>

template<typename T>
class btree {
public:
    class treeNode {

        T *data;
        treeNode *left = nullptr;
        treeNode *right = nullptr;
        treeNode *parent = nullptr;
        friend btree;
    public:
        unsigned displaySpaces = 8;
        static int space;

        const T &operator()() { return *data; }

        treeNode(const T &Data, treeNode *parent) : parent{parent} {
            this->data = new T(Data);
        }

        treeNode(T *Data, treeNode &parent) : parent{parent}, data{Data} {}

        ~treeNode() {
            delete data;
        }

        friend std::ostream &operator<<(std::ostream &out, treeNode &Input) {
            return out << Input.data;
        }

        void displayLeftOrder(std::ostream &out) {
            if (left) {
                space += displaySpaces;
                left->displayLeftOrder(out);
            } else structuredDisplay(nullptr, space + displaySpaces);

            structuredDisplay(data, space);

            if (right) {
                space += displaySpaces;
                right->displayLeftOrder(out);
            } else structuredDisplay(nullptr, space + displaySpaces);

            space -= displaySpaces;
        }

        void displayRightOrder(std::ostream &out) {
            if (right) {
                space += displaySpaces;
                right->displayRightOrder(out);
            } else structuredDisplay(nullptr, space + displaySpaces);

            structuredDisplay(data, space);

            if (left) {
                space += displaySpaces;
                right->displayRightOrder(out);
            } else structuredDisplay(nullptr, space + displaySpaces);

            space -= displaySpaces;
        }

    private:

        void displayUnderlyingNodes(std::ostream &out) {
            if (left) out << *left->data << ' ';
            if (right) out << *right->data << ' ';
        }

        void structuredDisplay(T *input, unsigned dist) {
            for (unsigned i = 0; i < dist; ++i) std::cout << ' ';

            if (!input) std::cout << "_ ";
            else std::cout << *input << ' ';

            std::cout << std::endl;
        }

        void clear() {
            if (left) left->clear();
            if (right) right->clear();

            delete left;
            delete right;
        }
    };

private:
    treeNode *root = nullptr;
    unsigned size = 0;
    unsigned lsize = 0;
    unsigned rsize = 0;

public:
    bool isEmpty() { return !root; }

    unsigned &Size() { return size; }

    unsigned &Height() { return lsize > rsize ? lsize : rsize; }

    btree() = default;

    btree(const btree &other) {
        *this = other;
    }

    ~btree() {
        clear();
    }

    void display(std::ostream &out, void (treeNode::*func)(std::ostream &)) {
        std::cout << std::setw(5);
        btree::treeNode::space = 0;
        if (root) (root->*func)(out);
        std::cout << std::setw(0);
    }

    void displaySize() {
        std::cout << "Amount of nodes stored in tree: " << size << std::endl;
        std::cout << "Height of left branch: " << lsize << std::endl;
        std::cout << "Height of right branch: " << rsize << std::endl;
    }

    btree &operator=(const btree &other) {
        if (&other == this) return *this;

        clear();
        copyBranch(other.root);

        return *this;
    }

    const btree &insertTree(const btree &other) {
        copyBranch(other.root);
        return *this;
    }

    const btree &moveTree(btree &other) {
        moveBranchWoutCopy(other.root);
        other.lsize = other.rsize = other.size = 0;
        other.root = nullptr;
        return *this;
    }

    const btree &mergeTree(btree *other) {
        return *this;
    }

    bool insert(const T &DataIn) {
        // Ordered by value. Size is totally random 
        return _insert(new treeNode(DataIn, nullptr));
    }

    bool insert(const T *DataIn) {
        return _insert(new treeNode(DataIn, nullptr));
    }

    bool insert(treeNode *node) {
        return _insert(node);
    }

    bool remove(const T &SearchValue) {
        return _remove(SearchValue);
    }

    bool remove(T SearchValue) {
        return _remove(SearchValue);
    }

private:
    void clear() {
        if (!root) return;
        btree::treeNode::space = 0;
        root->clear();
        delete root;
        root = nullptr;
    }

    void copyBranch(treeNode *branch) {
        if (!branch) return;
        insert(*branch->data);
        if (branch->right) copyBranch(branch->right);
        if (branch->left) copyBranch(branch->left);
    }

    void moveBranchWoutCopy(treeNode *branch) {
        if (!branch) return;

        if (branch->right) {
            moveBranchWoutCopy(branch->right);
            branch->right = nullptr;
        }

        if (branch->left) {
            moveBranchWoutCopy(branch->left);
            branch->left = nullptr;
        }

        insert(branch);
    }

    bool _insert(treeNode *ToAttach) {
        treeNode *temp = root, *next = root;
        T *DataIn = ToAttach->data;
        unsigned *Update;

        if (!root) {
            size = lsize = rsize = 1;
            root = ToAttach;
            ToAttach->parent = nullptr;
            return true;
        }

        if (*DataIn > *temp->data) Update = &rsize;
        else if (*DataIn == *temp->data) return false;
        else Update = &lsize;

        do {
            temp = next;

            if (*DataIn > *temp->data) {
                next = temp->right;
            } else if (*DataIn == *temp->data) return false;
            else {
                next = temp->left;
            }
        } while (next);

        if (*DataIn > *temp->data) {
            if (!temp->left) ++(*Update);
            temp->right = ToAttach;
        } else {
            if (!temp->right) ++(*Update);
            temp->left = ToAttach;
        }

        ToAttach->parent = temp;
        ++size;
        return true;
    }

    bool _remove(T &ToSearch) {
        btree::treeNode *temp = root;

        while (temp) {
            if (ToSearch == *temp->data) {
                if (temp == root) {
                    --rsize;
                    --lsize;
                    --size;
                    root = nullptr;
                } else {
                    if (temp->parent->left == temp) temp->parent->left = nullptr;
                    else temp->parent->right = nullptr;
                }

                if (temp->right) insert(temp->right);
                if (temp->left) insert(temp->left);

                delete temp;

                return true;
            } else if (ToSearch > *temp->data) {
                temp = temp->right;
            } else {
                temp = temp->left;
            }
        }

        return false;
    }
};

#endif