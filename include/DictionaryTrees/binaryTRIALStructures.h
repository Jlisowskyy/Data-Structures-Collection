//
// Created by Jlisowskyy on 1/13/24.
//

#ifndef BINARYTRIALSTRUCTURES_H
#define BINARYTRIALSTRUCTURES_H

/*          Disclaimer
 *  this file contains temporary tests and examples positional search structures
 *
 *
 *
 */

#include <cinttypes>
#include <iostream>
#include <bitset>

#include "../arrayBasedTreeHelpers.h"

template<uint maxBit = 3>
class binaryRST {
    // ------------------------------
    // Class inner types
    // ------------------------------
    static constexpr size_t NextCount = 2;

    struct node {
        node()  = default;
        explicit node(const uint32_t key) : elem{key} {}

        node* next[NextCount]{};
        uint32_t elem{};

        friend std::ostream& operator<<(std::ostream& out, const node& n) {
            out << std::bitset<maxBit+1>(n.elem);
            return out;
        }
    };

    enum childs {
        left = 0,
        right = 1,
    };

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    binaryRST() = default;

    binaryRST(const std::initializer_list<uint32_t>& init) {
        for (const auto bitSequence: init) {
            Insert(bitSequence);
        }    
    }
    
    ~binaryRST() {
        CleanArrBasedTree<node, NextCount>(_root);
    }


    // ------------------------------
    // class interaction
    // ------------------------------

    bool Insert(const uint32_t key) {
        return _insert(key);
    }

    bool Contains(const uint32_t key) {
        return *_search(key) != nullptr;
    }

    bool Delete(const uint32_t key) {
        return _delete(key);
    }

    void Print() const {
        SimplestArrBasedRecursivePrint<node, NextCount>(_root, 0);
    }

    // ------------------------------
    // private methods
    // ------------------------------

    bool _insert(const uint32_t key) {
        node** n = _search(key);

        if (*n) return false;

        *n = new node(key);
        return true;
    }

    node** _search(const uint32_t key) {
        node** pt = &_root;

        int bPos = maxBit;
        while (*pt && (*pt)->elem != key) {
            pt = &((*pt)->next[_extractBit(key, bPos--)]);
        }

        return pt;
    }

    bool _delete(const uint32_t key) {
        node** pt = &_root;

        // searching for node to remove
        int bPos = maxBit;
        while (*pt && (*pt)->elem != key) {
            pt = &((*pt)->next[_extractBit(key, bPos--)]);
        }

        // signaling that key was not found
        if (!*pt) return false;

        // looking for fitting node to replace old one
        node** ptToParent = pt;
        node* ptToRemove = *pt;

        // generating random branch to chose from
        uint32_t randomBits = key * 0xFA93B84B + 0x000000ff;
        while((*pt)->next[left] || (*pt)->next[right]) {
            // generating random branch to chose from
            uint32_t nextBranch = randomBits & 1ull;

            if (!(*pt)->next[nextBranch])
                // we went to empty branch, then go to other one
                pt = &((*pt)->next[(nextBranch+1) & 1ull]);
            else
                // we hit non empty branch
                pt = &((*pt)->next[nextBranch]);

            randomBits *= 0xFA93B84B;
            randomBits += 0x000000ff;
        }

        if (*pt != ptToRemove)
            // our removed element is not a leaf
        {
            node* replacementnode = *pt;

            // Firstly ensuring removed parent dependency
            *pt = nullptr;

            // copying subtress pointers belonging to removed element
            replacementnode->next[left] = ptToRemove->next[left];
            replacementnode->next[right] = ptToRemove->next[right];

            // setting up a replacement node
            *ptToParent = replacementnode;

            // removing old connection to replacment node
        }
        else
            // Removed element is a leaf
            *ptToParent = nullptr;

        // removing
        delete ptToRemove;

        return true;
    }

private:

    static unsigned _extractBit(const uint32_t key, const int pos) {
        return (key >> pos) & 1;
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    node* _root{};
};

template<uint maxBit = 3>
class binaryTRIE{
    // ------------------------------
    // Class inner types
    // ------------------------------
    static constexpr size_t NextCount = 2;

    struct node {
        node() = default;
        explicit node(const uint32_t key) : elem{key} {}

        node* next[NextCount]{};
        uint32_t elem{};

        friend std::ostream& operator<<(std::ostream& out, const node& n) {
            if (n.next[left] == nullptr && n.next[right] == nullptr)
                return out << std::bitset<maxBit+1>(n.elem);
            return out << 'X';
        }
    };

    enum childs {
        left,
        right
    };

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    binaryTRIE() = default;
    binaryTRIE(const std::initializer_list<uint32_t>& init): binaryTRIE() {
        for (const auto bitSequence: init) {
            Insert(bitSequence);
        }
    }
    ~binaryTRIE() {
        CleanArrBasedTree<node, NextCount>(_root);
    }

    // ------------------------------
    // class interaction
    // ------------------------------

    bool Insert(const uint32_t key) {
        return _insert(key);
    }

    bool Delete(const uint32_t key) {
        deleteResult = false;
        _deleteRecu(key, maxBit, &_root);

        return deleteResult;
    }

    bool Contains(const uint32_t key) {
        return _search(key);
    }

    void Print() const {
        SimplestArrBasedRecursivePrint<node, 2>(_root, 0);
    }

    // ------------------------------
    // private methods
    // ------------------------------

    static bool _deleteRecu(const uint32_t key, const uint bPos, node** root) {
        // encountered empty field abort
        if (!*root) return false;

        // reached leaf
        if (!(*root)->next[left] && !(*root)->next[right]) {
            if ((*root)->elem != key) return false;

            delete *root;
            *root = nullptr;

            deleteResult = true;

            return true;
        }

        // if leaf is not reached perform search
        const uint cleanBranch = _extractBit(key, bPos);
        const bool result = _deleteRecu(key, bPos-1, &(*root)->next[cleanBranch]);

        // no cleaning is necessary abort
        if (result == false) return false;

        const uint otherBranch = (cleanBranch+1)&1;

        // if branch below was cleaned replaced parent with other branch
        if (!(*root)->next[cleanBranch]) {
            *root = (*root)->next[otherBranch];
            return true;
        }

        // clean branch was reduced and we should chech whether other branch is null
        if (!(*root)->next[otherBranch]) {
            *root = (*root)->next[cleanBranch];
            return true;
        }

        return false;
    }

    bool _insert(const uint32_t key) {
        node ** p = &_root;

        // looking for key inside structure
        int bPos = maxBit;
        while((*p) && ((*p)->next[left] || (*p)->next[right])) {
            p = &((*p)->next[_extractBit(key, bPos--)]);
        }

        // if found abort
        if (*p && (*p)->elem == key) return false;

        if (!*p)
            // enocuntered empty slot fitting without hesitation
        {
            *p = new node(key);
            return true;
        }

        // if not perform branching on these one
        node* oldNode = *p;
        node* newNode = new node(key);
        *p = new node{};

        while(_extractBit(oldNode->elem, bPos) == _extractBit(key, bPos))
            // we need to perform another branching
        {
            node* nNode = new node();

            (*p)->next[_extractBit(key, bPos)] = nNode;
            p = &((*p)->next[_extractBit(key, bPos--)]);
        }

        (*p)->next[_extractBit(oldNode->elem, bPos)] = oldNode;
        (*p)->next[_extractBit(key, bPos)] = newNode;

        return true;
    }

    bool _search(const uint32_t key) {
        node ** p = &_root;

        int bPos = maxBit;
        while((*p) && ((*p)->next[left] || (*p)->next[right])) {
            p = &((*p)->next[_extractBit(key, bPos--)]);
        }

        return *p && (*p)->elem == key;
    }

private:

    static constexpr uint _extractBit(const uint32_t key, const uint pos) {
        return (key >> pos) & 1;
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    inline static bool deleteResult = false;
    node* _root{};
};



#endif //BINARYTRIALSTRUCTURES_H
