#ifndef MY_OWN_BTREE_H
#define MY_OWN_BTREE_H

#include <iostream>
#include <iomanip>

template<typename T>
class btree{
public:
    class treenode {

        T* Data;
        treenode* left = nullptr;
        treenode* right = nullptr;
        treenode* parent = nullptr;
        friend btree;
    public:
        unsigned DisplaySpaces = 8;
        static int space;

        const T& operator()() { return *Data; }
        treenode(const T& Data, treenode* parent) : parent{ parent } {
            this->Data = new T(Data);
        }
        treenode(T* Data, treenode& parent) : parent{ parent }, Data{ Data } {}

        ~treenode() {
            delete Data;
        }

        friend std::ostream& operator<<(std::ostream& out, treenode& Input) {
            return out << Input.Data;
        }

        void DisplayLeftOrder(std::ostream& out) {
            if (left) {
                space += DisplaySpaces;
                left->DisplayLeftOrder(out);
            }
            else StructurizedDisplay(nullptr, space + DisplaySpaces);

            StructurizedDisplay(Data, space);

            if (right) {
                space += DisplaySpaces;
                 right->DisplayLeftOrder(out);
            }
            else StructurizedDisplay(nullptr, space + DisplaySpaces);

            space -= DisplaySpaces;
        }

        void DisplayRightOrder(std::ostream& out) {
            if (right) {
                space += DisplaySpaces;
                right->DisplayRightOrder(out);
            }
            else StructurizedDisplay(nullptr, space + DisplaySpaces);

            StructurizedDisplay(Data, space);

            if (left) {
                space += DisplaySpaces;
                right->DisplayRightOrder(out);
            }
            else StructurizedDisplay(nullptr, space + DisplaySpaces);

            space -= DisplaySpaces;
        }

    private:

        void DisplayUnderlyingNodes(std::ostream& out) {
            if (left) out << *left->Data << ' ';
            if (right) out << *right->Data << ' ';
        }

        void StructurizedDisplay(T* input, unsigned dist) {
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
    treenode *root = nullptr;
    unsigned size = 0;
    unsigned lsize = 0;
    unsigned rsize = 0;

public:
    bool IsEmpty() { return !root; }
    unsigned& Size() { return size; }
    unsigned& Height() { return lsize > rsize ? lsize : rsize; }
    
    btree() {}
    btree(const btree& other) {
        *this = other;
    }
    ~btree() {
        clear();
    }
    void Display(std::ostream& out, void (treenode::*func)(std::ostream&)) {
        std::cout << std::setw(5);
        btree::treenode::space = 0;
        if (root) (root->*func)(out);
        std::cout << std::setw(0);
    }

    void DisplaySize() {
        std::cout << "Amount of nodes stored in tree: " << size << std::endl;
        std::cout << "Height of left branch: " << lsize << std::endl;
        std::cout << "Height of right branch: " << rsize << std::endl;
    }

    const btree& operator=(const btree& other) {
        clear();
        CopyBranch(other.root);

        return *this;
    }

    const btree& InsertTree(const btree& other) {
        CopyBranch(other.root);
        return *this;
    }

    const btree& MoveTree( btree& other) {
        MoveBranchCopyless(other.root);
        other.lsize = other.rsize = other.size = 0;
        other.root = nullptr;
        return *this;
    }

    const btree& MergeTree(btree* other) {
        return *this;
    }

    bool insert(const T& DataIn) {
        // Ordered by value. Size is totally random 
        return _Insert(new treenode(DataIn, nullptr));
    }

    bool insert(const T* DataIn) {
        return _Insert(new treenode(DataIn, nullptr)); }

    bool insert(treenode* node) {
        return _Insert(node);
    }

    bool remove(const T& SearchValue) {
        return _remove(SearchValue);
    }

    bool remove(T SearchValue) {
        return _remove(SearchValue);
    }

private:
    void clear() {
        if (!root) return;
        btree::treenode::space = 0;
        root->clear();
        delete root;
        root = nullptr;
    }

    void CopyBranch(treenode* branch) {
        if (!branch) return;
        insert(*branch->Data);
        if (branch->right) CopyBranch(branch->right);
        if (branch->left) CopyBranch(branch->left);
    }

    void MoveBranchCopyless(treenode* branch) {
        if (!branch) return;

        if (branch->right) {
            MoveBranchCopyless(branch->right);
            branch->right = nullptr;
        }

        if (branch->left) {
            MoveBranchCopyless(branch->left);
            branch->left = nullptr;
        }

        insert(branch);
    }

    bool _Insert(treenode* ToAttach) {
        treenode* temp = root, * next = root;
        T* DataIn = ToAttach->Data;
        unsigned* Update;

        if (!root) {
            size = lsize = rsize = 1;
            root = ToAttach;
            ToAttach->parent = nullptr;
            return true;
        }

        if (*DataIn > *temp->Data) Update = &rsize;
        else if (*DataIn == *temp->Data) return false;
        else Update = &lsize;

        do {
            temp = next;

            if (*DataIn > *temp->Data) {
                next = temp->right;
            }
            else if (*DataIn == *temp->Data) return false;
            else {
                next = temp->left;
            }
        } while (next);

        if (*DataIn > *temp->Data) {
            if (!temp->left) ++(*Update);
            temp->right = ToAttach; 
        }
        else {
            if (!temp->right) ++(*Update);
            temp->left = ToAttach;
        }

        ToAttach->parent = temp;
        ++size;
        return true;
    }

    bool _remove(T& ToSearch) {
        btree::treenode* temp = root;

        while (temp) {
            if (ToSearch == *temp->Data) {
                if (temp == root) {
                    --rsize;
                    --lsize;
                    --size;
                    root = nullptr;
                }
                else {
                    if (temp->parent->left == temp) temp->parent->left = nullptr;
                    else temp->parent->right = nullptr;
                }

                if (temp->right) insert(temp->right);
                if (temp->left) insert(temp->left);

                delete temp;

                return true;
            }
            else if (ToSearch > *temp->Data) {
                temp = temp->right;
            }
            else {
                temp = temp->left;
            }
        }

        return false;
    }
};

#endif