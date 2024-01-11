//
// Created by Jlisowskyy on 1/10/24.
//

#ifndef SPLAY_H
#define SPLAY_H

#include <cstdlib>
#include <utility>

#include "_SplayCore.h"
#include "../bTreeHelpers.h"

template<
    class keyT,
    class itemT,
    class predT
> class SplayTree {
    // ------------------------------
    // Class inner types
    // ------------------------------

    using mPair = std::pair<keyT, itemT>;
    using node = basicNode<keyT, itemT, predT>;
    using SplayCore = _SplayCore<keyT, node>;

    // ------------------------------
    // Class creation
    // ------------------------------
public:

    SplayTree() = default;

    template<class indexableT>
    SplayTree(const indexableT& indexableContainer, const size_t size) {
        for (size_t i = 0; i < size; ++i)
            Add(indexableContainer[i]);
    }

    SplayTree(std::initializer_list<mPair> init) {
        for (auto pair: init) {
            Add(pair);
        }
    }

    SplayTree(const SplayTree& other): _root{CloneTree(other._root)} {}
    SplayTree(SplayTree&& other) noexcept: _root{other._root} {
        other._root = nullptr;
    }

    SplayTree& operator=(const SplayTree& other) {
        if (this == &other) return *this;

        CleanTree(_root);
        _root = CloneTree(other._root);

        return *this;
    }

    SplayTree& operator=(SplayTree&& other)  noexcept {
        if (this == &other) return *this;

        CleanTree(_root);
        _root = other._root;
        other._root = nullptr;

        return *this;
    }

    ~SplayTree() {
        CleanTree(_root);
    }

    // ------------------------------
    // Class public methods
    // ------------------------------

    void Add(const keyT& key, const itemT& item) {
        if (!_root) _root = new node(std::make_pair(key, item));
        else _insert(std::make_pair(key, item));
    }

    void Add(const mPair&  pair) {
        if (!_root) _root = new node(pair);
        else _insert(pair);
    }

    [[nodiscard]] bool Contains(const keyT& key) {
        if (!_root) return false;
        SplayCore::splay(key, _root);

        return key == *_root ? true : false;
    }

    [[nodiscard]] bool IsEmpty() const { return _root != nullptr; }

    // Should only be invoked after Contains returned true or we checked that structure is not empty
    itemT& GetLastSearched() {
        return _root->content.second;
    }

    // Should only be invoked after Contains returned true or we checked that structure is not empty
    const itemT& GetLastSearched() const {
        return _root->content.second;
    }

    bool Delete(const keyT& key) {
        return _root == nullptr ? false : _delete(key);
    }

    void Print() const {
        SimplestRecursivePrint(_root, 0);
    }

    // ------------------------------
    // Private class methods
    // ------------------------------
private:

    // TODO: comments
    bool _delete(const keyT& key) {
        SplayCore::splay(key, _root);

        if (key != *_root) return false;
        const node* n = _root;

        if (!_root->left) {
            _root = _root->right;
        }
        else {
            SplayCore::splay(key, _root->left);
            _root->left->right = _root->right;
            _root = _root->left;
        }

        delete n;
        return true;
    }

    void _insert(const mPair& pair) {
        // Structure reconstruction
        SplayCore::splay(pair.first, _root);

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

    // ------------------------------
    // Class fields
    // ------------------------------

    node* _root{};
};

#endif //SPLAY_H