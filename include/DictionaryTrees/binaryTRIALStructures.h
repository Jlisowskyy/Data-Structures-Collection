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
private:

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
    binaryTRIE(const std::initializer_list<uint32_t>& init) {
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
private:

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

        // no cleaning is necessary abort
        if (!_deleteRecu(key, bPos-1, &(*root)->next[cleanBranch])) return false;

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

    static constexpr uint _extractBit(const uint32_t key, const uint pos) {
        return (key >> pos) & 1;
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    inline static bool deleteResult = false;
    node* _root{};
};

template<uint maxBit = 3>
class binaryPATRICIA{
    // ------------------------------
    // Class inner types
    // ------------------------------

    static constexpr size_t NextCount = 2;

    struct node {
        node() = default;
        explicit node(const uint32_t key) : elem{key} {}

        node* next[NextCount]{};
        uint32_t elem{};
        u_char bit{};

        friend std::ostream& operator<<(std::ostream& out, const node& n) {
            return out << std::bitset<maxBit+1>(n.elem);
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

    binaryPATRICIA() = default;

    binaryPATRICIA(const std::initializer_list<uint32_t>& init){
        for (const auto bitSequence: init) {
            Insert(bitSequence);
        }
    }

    ~binaryPATRICIA() {
        _cleanPatricia(_root);
    }

    // ------------------------------
    // class interaction
    // ------------------------------

    bool Insert(const uint32_t key) {
        return _insert(key);
    }

    bool Delete(const uint32_t key) {

        return false;
    }

    bool Contains(const uint32_t key) {
        return _search(key)->elem == key;
    }

    std::ostream& Print(std::ostream& out = std::cout) const {
        return _printRecu(out, _root, 0);
    }

    // ------------------------------
    // private methods
    // ------------------------------
private:

    static void _cleanPatricia(const node* n) {
        if (n->next[left]->bit < n->bit)
            _cleanPatricia(n->next[left]);
        if (n->next[right]->bit < n->bit)
            _cleanPatricia(n->next[right]);

        delete n;
    }

    static std::ostream& _printRecu(std::ostream& out, const node* const n, const uint depth) {
        if (n->next[left]->bit < n->bit)
            _printRecu(out, n->next[left], depth+4);
        out << std::string(depth, ' ') << *n << std::endl;
        if (n->next[right]->bit < n->bit)
            _printRecu(out, n->next[right], depth+4);

        return out;
    }

    bool _insert(const uint32_t key) {

        if (!_root) {
            node* sentinel = new node(0);
            sentinel->next[0] = sentinel->next[1] = sentinel;

            _root = new node(key);
            _root->bit = maxBit;
            _root->next[_extractBit(key, maxBit)] = _root;
            _root->next[1-_extractBit(key, maxBit)] = sentinel;

            return true;
        }

        // Finding spot to place new key
        node* elemToFind = _search(key);

        // If already exists abort
        if (elemToFind->elem == key) return false;

        // Find position of first differing bit - nobki tutaj nie rozumieja
        const uint32_t differingBits = key ^ elemToFind->elem;
        uint firstDifferingBit = __builtin_clz(differingBits)^31;

        // Find exact parent to which new key should be linked, between pp and p
        node* p = _root;
        node* pp{};

        do {
            pp = p;
            p = p->next[_extractBit(key, p->bit)];
        }while(pp->bit > p->bit && p->bit > firstDifferingBit);

        // add new node between pp and p
        node* nNode = new node(key);
        nNode->bit = firstDifferingBit;
        nNode->next[_extractBit(key, firstDifferingBit)] = nNode;
        nNode->next[1 - _extractBit(key, firstDifferingBit)] = p;
        pp->next[_extractBit(key, pp->bit)] = nNode;

        return true;
    }

    node* _search(const uint32_t key) {
        node* p = _root;
        node* pp{};

        do {
            pp = p;
            p = p->next[_extractBit(key, p->bit)];
        }while(pp->bit > p->bit);

        return p;
    }

    static constexpr uint _extractBit(const uint32_t key, const uint pos) {
        return (key >> pos) & 1;
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    node* _root{};
};


#endif //BINARYTRIALSTRUCTURES_H
