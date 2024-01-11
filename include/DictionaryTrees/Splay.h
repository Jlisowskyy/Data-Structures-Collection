//
// Created by Jlisowskyy on 1/10/24.
//

#ifndef SPLAY_H
#define SPLAY_H

#include <cstdlib>
#include <string>
#include <utility>
#include <sstream>
#include <iostream>

#include "_AVLcore.h"

template<
    class prioT,
    class itemT,
    class predT
> class SplayTree {
    // ------------------------------
    // Class inner types
    // ------------------------------

    using mPair = std::pair<prioT, itemT>;
    struct node;
    using AVLCore = _AVLCore<node>;

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    SplayTree() = default;

    template<typename indexableT>
    SplayTree(const indexableT& indexableContainer, const size_t size) {
        for (size_t i = 0; i < size; ++i)
            Add(indexableContainer[i]);
    }

    SplayTree(std::initializer_list<mPair> init) {
        for (auto pair: init) {
            Add(pair);
        }
    }

    ~SplayTree() {
        _cleanTree(_root);
    }

    // ------------------------------
    // Class public methods
    // ------------------------------

    void Add(const prioT& prio, const itemT& item) {
        if (!_root) _root = new node(std::make_pair(prio, item));
        else _insert(std::make_pair(prio, item));
    }

    void Add(const mPair&  pair) {
        if (!_root) _root = new node(pair);
        else _insert(pair);
    }

    bool Contains(const prioT& prio) {
        if (!_root) return false;
        _splay(prio, _root);

        return prio == *_root ? true : false;
    }

    bool IsEmpty() const { return _root != nullptr; }

    // Should only be invoked after Contains returned true or we checked that structure is not empty
    itemT& GetLastSearched() {
        return _root->content.second;
    }

    // Should only be invoked after Contains returned true or we checked that structure is not empty
    const itemT& GetLastSearched() const {
        return _root->content.second;
    }

    bool Delete(const prioT& prio) {
        return _root == nullptr ? false : _delete(prio);
    }

    void Print() const {
        _printTree(_root, 0);
    }

    // ------------------------------
    // Private class methods
    // ------------------------------
private:

    // TODO: comments
    bool _delete(const prioT& prio) {
        _splay(prio, _root);

        if (prio != *_root) return false;
        const node* n = _root;

        if (!_root->left) {
            _root = _root->right;
        }
        else {
            _splay(prio, _root->left);
            _root->left->right = _root->right;
            _root = _root->left;
        }

        delete n;
        return true;
    }

    void _insert(const mPair& pair) {
        // Structure reconstruction
        _splay(pair.first, _root);

        if (pair.first > *_root)
            // old root pushed into left subtree
        {
            node* n = new node(pair);

            // moving root to desired place
            n->left = _root;
            n->right = _root->right;
            _root->right = nullptr;

            // assigning new root
            _root = n;
        }
        else if (*_root > pair.first)
            // old root pushed into right subtree
        {
            node* n = new node(pair);

            // moving root to desired place
            n->right = _root;
            n->left = _root->left;
            _root->left = nullptr;

            // assigning new root
            _root = n;
        }
    }

    static void _LZig(node*& n) {
        AVLCore::LL(n);
    }

    static void _LZigZig(node*& n) {
        AVLCore::LL(n);
        AVLCore::LL(n);
    }

    static void _LZigZag(node*& n) {
        AVLCore::LR(n);
    }

    static void _RZig(node*& n) {
        AVLCore::RR(n);
    }

    static void _RZigZig(node*& n) {
        AVLCore::RR(n);
        AVLCore::RR(n);
    }

    static void _RZigZag(node*& n) {
        AVLCore::RL(n);
    }

    static void _splay(const prioT prio, node* root) {

        // elements of interest should be on the left side
        if (*root > prio) {

            // empty branch
            if (!root->left) return;

            if (*root->left > prio)
                // deeper left side
            {
                if (root->left->left)
                    // if exists process depth two rotation
                {
                    _splay(prio, root->left->left);
                    _LZigZig(root);
                }
                else
                    // if not process depth one rotation
                    _LZig(root);
            }
            else if (prio > *root->left)
                // deeper right side
            {
                if (root->left->right)
                    // if exists process depth two rotation
                {
                    _splay(prio, root->left->right);
                    _LZigZag(root);
                }
                else
                    // if not process depth one rotation
                    _LZig(root);
            }
            else
                // we found our element roll back
                _LZig(root);

        }
        // elements of interest should be on the right side
        else if (prio > *root) {

            // empty branch abandon work
            if (!root->right) return;

            if (*root->right > prio)
                // deeper left side
            {
                if (root->right->left)
                    // if exists process depth two rotation
                {
                    _splay(prio, root->right->left);
                    _RZigZag(root);
                }
                else
                    // if not process depth one rotation
                    _RZig(root);
            }
            else if (prio > *root->right)
                // deeper right side
            {
                if (root->right->right)
                    // if exists process depth two rotation
                {
                    _splay(prio, root->right->right);
                    _RZigZig(root);
                }
                else
                    // if not process depth one rotation
                    _RZig(root);
            }
            else
                // we found our element roll back
                _RZig(root);
        }
    }

    static void _cleanTree(node* n) {
        if (!n) return;

        _cleanTree(n->left);
        _cleanTree(n->right);

        delete n;
    }

    static size_t _findLargestPrintsize(node* n) {
        if (!n) return 0;

        const size_t lSize = _findLargestPrintsize(n->left);
        const size_t rSize = _findLargestPrintsize(n->right);

        std::ostringstream str{};
        str << n->content.first;

        // chosing maximal size
        return std::max(str.str().length(), std::max(lSize, rSize));
    }

    static void _printTree(node* n, const int depth) {
        if (!n) return;

        _printTree(n->left, depth + 4);
        std::cout << std::string(depth, ' ') << n->content.first << std::endl;
        _printTree(n->right, depth + 4);
    }

    // ----------------------------------
    // innter types implementations
    // ----------------------------------

    struct node {
        mPair content;

        node* left{};
        node* right{};

        node() = default;
        node(const mPair& pair): content{pair} {}
        node(const itemT& nItem, const prioT& nPrio): content{nItem, nPrio} {}
        ~node() = default;

        friend bool operator>(const node& a, const node& b) {
            return pred(a.content.first, b.content.first);
        }

        friend bool operator>(const node& a, const prioT& b) {
            return pred(a.content.first, b);
        }

        friend bool operator>(const prioT& a, const node& b) {
            return pred(a, b.content.first);
        }

        friend bool operator==(const node& a, const node& b){
            return !(a > b || b > a);
        }

        friend bool operator==(const prioT& a, const node& b) {
            return !(a > b || b > a);
        }

        friend bool operator==(const node& a, const prioT& b) {
            return !(a > b || b > a);
        }

        friend bool operator!=(const node& a, const node& b){
            return !(a==b);
        }

        friend bool operator!=(const prioT& a, const node& b) {
            return !(a==b);
        }

        friend bool operator!=(const node& a, const prioT& b) {
            return !(a==b);
        }

    private:
        static constexpr predT pred{};
    };

    // ------------------------------
    // Class fields
    // ------------------------------

    node* _root{};
};

#endif //SPLAY_H